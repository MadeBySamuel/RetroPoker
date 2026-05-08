#include <iostream> 

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <functional>
#include <vector>

#include "auth.hpp"
#include <argon2.h>
#include <sqlite3.h>
#include "auth.hpp"


#include "gui_utils.hpp"





class Game {
    
    private:
        tgui::Gui& gui;
        sf::Music& music;
        sf::RenderWindow& window;
        std::function<void()> onLogin;

    public:
        Game(tgui::Gui& gui,std::function<void()> onLogin, sf::RenderWindow& window, sf::Music& music) : gui(gui), onLogin(onLogin), window(window), music(music) {}

    void game(){
        auto root = tgui::Panel::create();
        root->getRenderer()->setTextureBackground("assets/images/backgrounds/game_background.jpeg");
        gui.add(root);

    }
};