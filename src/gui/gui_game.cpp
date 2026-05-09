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
#include "constants.hpp"


class Game_GUI : public Gui_base{
    
    private:
        sf::Music& music;
        sf::RenderWindow& window;

    public:
        Game_GUI(tgui::Gui& gui, sf::RenderWindow& window, sf::Music& music) : Gui_base(gui), window(window), music(music) {}
        std::function<void()> onExit;


    void game(){
        GuiTheme color;
        auto root = tgui::Panel::create();
        root->getRenderer()->setTextureBackground("assets/images/backgrounds/game_background.jpeg");
        gui.add(root);

        bar(root);
    }
};