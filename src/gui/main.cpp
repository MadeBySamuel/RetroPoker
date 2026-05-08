#include "gui_login.cpp"
#include "gui_main_menu.cpp"


#include "auth.hpp"
#include "gui_utils.hpp"
#include <sqlite3.h>
#include <iostream>



enum class ScreenState{
    Login,
    MainMenu,
    Game
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

    sf::Music music("assets/sound/western_music.ogg");
    
    music.play();
    music.setLooping(true);


    music.setVolume(0);
    

    


    std::string last_time_text;



    while(window.isOpen()){
        while(const auto event = window.pollEvent()){
            gui.handleEvent(*event);
            if(event->is<sf::Event::Closed>()){
                window.close();
            }
        }

        if (screen == ScreenState::Login){
            login.time();
        }
        else if (screen == ScreenState::MainMenu){
            mainmenu.time(color);
        }
        gui.draw();
        window.display();
    }


    
  

    return 0;
}
