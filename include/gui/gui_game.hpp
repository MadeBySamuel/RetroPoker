#pragma once

#include <functional>
#include <memory>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

#include "game.hpp"
#include "gui_utils.hpp"

class Game_GUI : public Gui_base {

    private:
        sf::Music& music;
        sf::RenderWindow& window;
        std::unique_ptr<Game> engine;

        void placeCard(tgui::Panel::Ptr parent, const Card& card, float x, float y, float w, float h);
        void placeFaceDownPair(tgui::Panel::Ptr parent, float leftX, float topY, float cardW, float cardH, float gap);
        void placePlayerName(tgui::Panel::Ptr parent, const std::string& name, float leftX, float topY, float cardH, float guiW, float guiH, const GuiTheme& color);

    public:
        Game_GUI(tgui::Gui& gui, sf::RenderWindow& window, sf::Music& music);
        std::function<void()> onExit;
        void game();
};
