#include "gui_login.cpp"
#include "gui_main_menu.cpp"
#include "gui_game.cpp"


#include "auth.hpp"
#include "gui_utils.hpp"
#include <sqlite3.h>
#include <iostream>



enum class ScreenState{
    Login,
    MainMenu,
    Game_GUI
};  



void sound(){
    GuiTheme color;
    auto time_label = tgui::Label::create("");
    time_label->getRenderer()->setTextColor(color.down);
    time_label->setPosition("85%", "10%");
    time_label->setTextSize(20);

}


int main(){

    using namespace std::chrono_literals;
    GuiTheme color;

    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;

    sf:: VideoMode desktop = sf::VideoMode::getDesktopMode();

    sf::RenderWindow window(desktop, "Retro Poker", sf::Style::Default, sf::State::Fullscreen, settings);
    window.setFramerateLimit(60);




    tgui::Gui gui{window};

    const auto windowSize = gui.getWindow()->getSize();
    const float guiWidth = static_cast<float>(windowSize.x);
    const float guiHeight = static_cast<float>(windowSize.y);


    sf::Music music("assets/sound/song2.ogg");

    music.play();
    music.setLooping(true);


    music.setVolume(0);
    

    ScreenState screen = ScreenState::Login;
    
    
    Login login(gui, music);
    MainMenu mainmenu(gui, window, music);
    Game_GUI game(gui, window, music);


    login.login_screen();

    Gui_base* currentPointer = &login;

    std::string logged_username = "";

    login.onLogin = [&](std::string username){
        std::cout << username;
        logged_username = username;
        currentPointer = &mainmenu;
        screen = ScreenState::MainMenu;
        mainmenu.logged_username = username;
        mainmenu.showMainMenu(); 
    };


    mainmenu.onGame = [&]{
        currentPointer = &game;
        screen = ScreenState::Game_GUI;
        game.game();
    };

    std::string last_time_text;


        while(window.isOpen()){
            while(const auto event = window.pollEvent()){
                gui.handleEvent(*event);
                if(event->is<sf::Event::Closed>()){
                    window.close();
                }
            }
            currentPointer->update_time();

            gui.draw();
            window.display();
        }


    
  

    return 0;
}
