#include "gui_login.hpp"
#include "gui_main_menu.hpp"
#include "gui_game.hpp"
#include "gui_utils.hpp"

#include "auth.hpp"
#include "gui_utils.hpp"
#include <sqlite3.h>
#include <iostream>



enum class ScreenState{
    Login,
    MainMenu,
    Game_GUI
};  

int main(){

    sqlite3* database = nullptr;
    const bool databaseOpened = sqlite3_open("users.db", &database) == SQLITE_OK;
    std::unique_ptr<sqlite3, decltype(&sqlite3_close)> databaseOwner(database, sqlite3_close);
    campaign::Store campaignStore(database);
    const bool campaignStorageReady = databaseOpened && auth::createUsersTable(database) && campaignStore.initialize();

    using namespace std::chrono_literals;

    sf::ContextSettings settings;
    settings.antiAliasingLevel = 16;

    sf:: VideoMode desktop = sf::VideoMode::getDesktopMode();

    sf::RenderWindow window(desktop, "Retro Poker", sf::Style::Default, sf::State::Fullscreen, settings);
    window.setFramerateLimit(60);

    tgui::Gui gui{window};

    sf::Music music("assets/sound/song2.ogg");

    
    music.play();
    music.setLooping(true);
    music.setVolume(0);
    
    ScreenState screen = ScreenState::MainMenu;
    
    Login login(gui, music, window);
    MainMenu mainmenu(gui, music, window);
    Game_GUI game(gui, music, window);
    Gui_base gui_base(gui,music);

    
    int activeCampaignStage = -1;
    const auto loadCampaign = [&] {
        mainmenu.campaignProfile = {};
        mainmenu.campaignSaveError.clear();
        if (!campaignStorageReady || !campaignStore.load(mainmenu.loggedUsername, mainmenu.campaignProfile)) {
            mainmenu.campaignSaveError = "Campaign storage unavailable. Please restart the game.";
        }
    };
    const auto saveCampaign = [&] {
        if (campaignStorageReady && campaignStore.save(mainmenu.loggedUsername, mainmenu.campaignProfile)) {
            mainmenu.campaignSaveError.clear();
        } else {
            mainmenu.campaignSaveError = "Could not save campaign. Please check users.db and restart the game.";
            std::cerr << mainmenu.campaignSaveError << '\n';
        }
    };
    loadCampaign();

    //login.login_screen();
    login.login_screen();

    Gui_base* currentPointer = &login;
    std::string loggedUsername = "";

    login.onLogin = [&](std::string username){
        std::cout << username;
        loggedUsername = username;
        currentPointer = &mainmenu;
        screen = ScreenState::MainMenu;
        mainmenu.loggedUsername = username;
        loadCampaign();
        mainmenu.showMainMenu(); 
    };


    mainmenu.onGame = [&](int playerCount, int smallBlind){
        activeCampaignStage = -1;
        currentPointer = &game;
        screen = ScreenState::Game_GUI;
        game.game(playerCount, smallBlind);
    };

    mainmenu.onCampaign = [&](int stage) {
        if (!mainmenu.campaignProfile.canEnter(stage) || !mainmenu.campaignSaveError.empty()) return;
        activeCampaignStage = stage;
        const auto& stop = campaign::stages[stage];
        currentPointer = &game;
        screen = ScreenState::Game_GUI;
        game.game(stop.players, stop.smallBlind, mainmenu.campaignProfile.balance, stop.opponentStack, stage);
    };

    game.onCampaignUpdate = [&](int balance, bool won) {
        if (activeCampaignStage < 0) return;
        const auto previous = mainmenu.campaignProfile;
        mainmenu.campaignProfile.recordResult(activeCampaignStage, balance, won);
        if (previous.balance != mainmenu.campaignProfile.balance ||
            previous.completedStages != mainmenu.campaignProfile.completedStages ||
            !mainmenu.campaignSaveError.empty()) saveCampaign();
    };

    mainmenu.onResetCampaign = [&] {
        if (mainmenu.campaignProfile.balance != 0) return;
        mainmenu.campaignProfile = {};
        saveCampaign();
        mainmenu.showMainMenu();
        mainmenu.showCampaign();
    };

    mainmenu.onLogout = [&]{
        loggedUsername.clear();
        mainmenu.loggedUsername.clear();
        currentPointer = &login;
        screen = ScreenState::Login;
        login.login_screen();
    };

    game.onExit = [&]{
        const bool fromCampaign = activeCampaignStage >= 0;
        game.stopSession();
        activeCampaignStage = -1;
        currentPointer = &mainmenu;
        screen = ScreenState::MainMenu;
        mainmenu.showMainMenu();
        if (fromCampaign) mainmenu.showCampaign();
    };

    std::string last_time_text;
        while(window.isOpen()){
            while(const auto event = window.pollEvent()){
                gui.handleEvent(*event);
                if(event->is<sf::Event::Closed>()){
                    if (currentPointer == &game) game.stopSession();
                    window.close();
                }

                if (const auto* key = event->getIf<sf::Event::KeyPressed>()){
                    if (key->code == sf::Keyboard::Key::Escape && currentPointer == &game){
                        game.escape();
                    }
                    if (key->code == sf::Keyboard::Key::Escape && currentPointer == &mainmenu){
                        if (mainmenu.openedSettingsPanel){
                            mainmenu.hide_game_settings();
                        }
                        if(mainmenu.openedGameMenuPanel){
                            mainmenu.handleGameMenuEscape();
                        }
                
                    }
                }

            }
            currentPointer->update_time();

            if(currentPointer == &game && !game.isPaused() && game.restartPending && game.restartClock.getElapsedTime().asSeconds() >= 10.f){
                game.restartPending = false;
                game.nextHand();
            } 

            if(currentPointer == &game && !game.isPaused() && game.botWaiting && game.botChooseClock.getElapsedTime().asSeconds() >= game.timeForBot){
                game.nextBot();
            }

            if (currentPointer == &game) {
                game.animateTurnPanel();
                game.animateCommunityCards();
            }
            
            gui.draw();
            window.display();
        }


    
  

    if (currentPointer == &game) game.stopSession();
    return 0;
}
