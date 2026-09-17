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
    
    ScreenState screen = ScreenState::Game_GUI;
    
    Login login(gui, music, window);
    MainMenu mainmenu(gui, music, window);
    Game_GUI game(gui, music, window);
    Gui_base gui_base(gui,music);

    
    // menič aktualneho módu

    //login.login_screen();
    mainmenu.showMainMenu();

    Gui_base* currentPointer = &mainmenu;
    std::string loggedUsername = "";

    login.onLogin = [&](std::string username){
        std::cout << username;
        loggedUsername = username;
        currentPointer = &mainmenu;
        screen = ScreenState::MainMenu;
        mainmenu.loggedUsername = username;
        mainmenu.showMainMenu(); 
    };


    mainmenu.onGame = [&](int playerCount, int smallBlind){
        currentPointer = &game;
        screen = ScreenState::Game_GUI;
        game.game(playerCount, smallBlind);
    };

    mainmenu.onLogout = [&]{
        loggedUsername.clear();
        mainmenu.loggedUsername.clear();
        currentPointer = &login;
        screen = ScreenState::Login;
        login.login_screen();
    };

    game.onExit = [&]{
        currentPointer = &mainmenu;
        screen = ScreenState::MainMenu;
        mainmenu.showMainMenu();
    };

    std::string last_time_text;
        while(window.isOpen()){
            while(const auto event = window.pollEvent()){
                gui.handleEvent(*event);
                if(event->is<sf::Event::Closed>()){
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

            if(game.restartPending && game.restartClock.getElapsedTime().asSeconds() >= 3.f){
                game.restartPending = false;
                game.nextHand();
            } 

            if(game.botWaiting && game.botChooseClock.getElapsedTime().asSeconds() >= game.timeForBot){
                game.nextBot();
            }



            
            gui.draw();
            window.display();
        }


    
  

    return 0;
}
