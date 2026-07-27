#pragma once

#include <functional>
#include <string>
#include <vector>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

#include "gui_utils.hpp"

class MainMenu : public Gui_base {

    private:
        sf::RenderWindow& window;
        tgui::Panel::Ptr topBar;
        tgui::Label::Ptr time_label;
        std::string last_time_text;

    public:
        MainMenu(tgui::Gui& gui, sf::Music& music, sf::RenderWindow& window);
        std::function<void()> onGame;
        std::function<void()> onLogout;
        std::string loggedUsername;

        void showMainMenu();
        void game_menu(std::vector<tgui::Button::Ptr> buttons);
};
