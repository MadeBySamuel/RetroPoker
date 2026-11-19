#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

#include "game.hpp"
#include "gui_utils.hpp"
#include "table_layout.hpp"
#include "hand_evaluator.hpp"
#include "campaign.hpp"

class Game_GUI : public Gui_base {

    private:
        sf::RenderWindow& window;
        std::unique_ptr<Game> engine;
        int campaignStage = -1;
        bool seriesFinished = false;
        void publishCampaignBalance();
        void finishSeries(campaign::SeriesResult result);

        tgui::Panel::Ptr communityPanel;
        struct CommunityCardReveal {
            tgui::Picture::Ptr picture;
            tgui::Vector2f targetPosition;
            float startTime = 0.f;
            bool finished = false;
        };
        std::vector<CommunityCardReveal> communityCardReveals;
        sf::Clock communityRevealClock;
        tgui::Label::Ptr potLabel;
        tgui::Label::Ptr blindStatusLabel;
        void refreshBlindStatus();
        tgui::Label::Ptr winnerReasonLabel;
        tgui::Label::Ptr balanceLabel;
        tgui::Label::Ptr balanceAmountLabel;
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
        std::vector<tgui::Label::Ptr> playerNameLabels;
        std::vector<tgui::Label::Ptr> choiceLabels;
        std::vector<tgui::Label::Ptr> currentBetLabels;
        std::vector<std::string> outLogMessages;
        std::vector<tgui::Label::Ptr> outLogLabels;
        std::vector<bool> loggedOutPlayers;


        tgui::Button::Ptr foldButton;
        tgui::Button::Ptr raiseButton;
        tgui::Button::Ptr allInButton;
        tgui::Button::Ptr checkButton;
        tgui::Button::Ptr callButton;
        tgui::Panel::Ptr animatedTurnPanel;
        tgui::Vector2f animatedTurnPanelBasePosition;
        tgui::Vector2f animatedTurnPanelBaseSize;
        tgui::Vector2f turnPanelStartPosition;
        tgui::Vector2f turnPanelStartSize;
        tgui::Vector2f turnPanelTargetPosition;
        tgui::Vector2f turnPanelTargetSize;
        sf::Clock turnPanelClock;
        bool turnPanelMoving = false;

        void createCurrentBetLabel();
        void createMoneyLabelsPlayers();
        void placeCard(tgui::Panel::Ptr parent, const Card& card, float x, float y, float w, float h);
        void placeFaceDownPair(tgui::Panel::Ptr parent, float leftX, float topY, float cardW, float cardH, float gap);
        tgui::Label::Ptr placePlayerName(tgui::Panel::Ptr parent, const std::string& name, const TableSeat& seat, float guiW, float guiH);
        void placeCenteredLabel(const tgui::Label::Ptr& label, float centerX, float topY, float minX, float maxX);

        void refreshCommunity();
        void updateBotWaiting();
        void refreshHud();
        void disableActions();
        void layoutVisibleActionButtons();

    public:
        sf::Clock restartClock;
        sf::Clock botChooseClock;
        bool restartPending = false;
        bool showdownResolved = false;
        int playerTurn = 1;
        bool botWaiting = true;
        float timeForBot = 0.f;
        std::vector<tgui::Panel::Ptr> panels;
        void hideEscapeMenu();
        void escape();

        void nextBot();
        void animateTurnPanel();
        void animateCommunityCards();

        Game_GUI(tgui::Gui& gui, sf::Music& music, sf::RenderWindow& window);

        void placeOutLabel();
        void nextHand();
        void buildTable();
        std::function<void()> onExit;
        std::function<void(int, bool)> onCampaignUpdate;
        void stopSession();
        bool isPaused() const { return modalOverlay && modalOverlay->isVisible(); }
        void showEscapeMenu();
        void game(int playerCount, int smallBlind,
                  int startingMoney = Constants::START_PLAYER_MONEY,
                  int opponentMoney = Constants::START_PLAYER_MONEY, int stage = -1);
        void restartGame();
        void resolveShowdown(bool revealCards = true);
        tgui::Panel::Ptr makeCirclePanel(std::string& name, tgui::Vector2f pos);
        void placeBlind(std::string& name, const std::string& title, tgui::Vector2f pos);
        void revealShowdownCards();
    };
