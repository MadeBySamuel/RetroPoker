#pragma once

#include <functional>
#include <memory>
#include <vector>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

#include "game.hpp"
#include "gui_utils.hpp"
#include "table_layout.hpp"
#include "hand_evaluator.hpp"

class Game_GUI : public Gui_base {

    private:
        sf::RenderWindow& window;
        std::unique_ptr<Game> engine;

        tgui::Panel::Ptr communityPanel;
        tgui::Label::Ptr potLabel;
        tgui::Label::Ptr winnerLabel;
        tgui::Label::Ptr balanceLabel;
        tgui::Label::Ptr actionStatus;
        std::vector<tgui::Button::Ptr> actionButtons;
        tgui::Slider::Ptr raiseSlider;
        tgui::Button::Ptr confirmButton;
        tgui::Label::Ptr raiseMoneyLabel;

        float balanceCenterX = 0.f;
        float balanceY = 0.f;
        float communityY = 0.f;
        float communityCardW = 0.f;
        float communityCardH = 0.f;
        float communityGap = 0.f;
        std::size_t humanPlayerIndex = 0;
        float gapClanker = 70.f;
        float gapPlayer = 90.f;
        float blindChipRadiusClanker = 50.f;
        float blindChipRadiusPlayer = 70.f;
        std::vector<tgui::Label::Ptr> moneyLabels;
    
        void createMoneyLabelsPlayers();
        void placeCard(tgui::Panel::Ptr parent, const Card& card, float x, float y, float w, float h);
        void placeFaceDownPair(tgui::Panel::Ptr parent, float leftX, float topY, float cardW, float cardH, float gap);
        void placePlayerName(tgui::Panel::Ptr parent, const std::string& name, const TableSeat& seat, float guiW, float guiH);
        void placeCenteredLabel(const tgui::Label::Ptr& label, float centerX, float topY, float minX, float maxX);

        void refreshCommunity();
        void refreshHud();
        void disableActions();

    public:
        sf::Clock restartClock;
        bool restartPending = false;
        void hideEscapeMenu();
        void escape();

        Game_GUI(tgui::Gui& gui, sf::Music& music, sf::RenderWindow& window);

        void nextHand();
        void buildTable();
        std::function<void()> onExit;
        void showEscapeMenu();
        void game();
        void restartGame();
        void resolveShowdown();
        tgui::Panel::Ptr makeCirclePanel(std::string& name, tgui::Vector2f pos);
        void placeBlind(std::string& name, const std::string& title, tgui::Vector2f pos);
    };
