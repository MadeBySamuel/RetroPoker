#include "gui_login.cpp"
#include "gui_main_menu.cpp"


#include "auth.hpp"
#include <sqlite3.h>
#include <iostream>



enum class ScreenState{
    Login,
    MainMenu
};  



int main(){

    using namespace std::chrono_literals;



    tgui::Color bg(28, 24, 20);        
    tgui::Color panel(58, 46, 38);    
    tgui::Color panel2(72, 58, 48);    
    tgui::Color accent(212, 160, 23);  
    tgui::Color text(244, 230, 200);   
    tgui::Color hover(201, 106, 43);   
    tgui::Color border(120, 92, 60);   
    tgui::Color down(160, 110, 25);    
    tgui::Color disabled(80, 70, 60);  
    tgui::Color yes_quit(133, 22, 22);


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


    auto time_label = tgui::Label::create("");
    time_label->getRenderer()->setTextColor(down);
    time_label->setPosition("85%", "10%");
    time_label->setTextSize(20);

    MainMenu mainmenu(gui, window);
    ScreenState screen = ScreenState::Login;
    Login login(gui,[&]{
        screen = ScreenState::MainMenu;
        mainmenu.showMainMenu();
    },music);


    
    if (screen == ScreenState::Login){
        login.login_screen();
    }
    


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
        else {
            mainmenu.time();
        }
        gui.draw();
        window.display();
    }


    
  

    return 0;
}
