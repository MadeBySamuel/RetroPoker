#include <algorithm>
#include <iostream>
#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

#include "gui_game.hpp"
#include "table_layout.hpp"

namespace {

constexpr const char* CARD_BACK_PATH = "assets/cards/turned_down_card.png";

unsigned int scaledTextSize(float guiH, float ratio, unsigned int minSize, unsigned int maxSize) {
    return static_cast<unsigned int>(std::clamp(guiH * ratio, static_cast<float>(minSize), static_cast<float>(maxSize)));
}

float seatPairWidth(const TableSeat& seat, float guiW) {
    return (2.f * seat.cardW + seat.gap) * guiW;
}

float seatPairCenterX(const TableSeat& seat, float guiW) {
    return seat.leftX * guiW + seatPairWidth(seat, guiW) * 0.5f;
}

}

Game_GUI::Game_GUI(tgui::Gui& gui, sf::RenderWindow& window, sf::Music& music)
    : Gui_base(gui), window(window), music(music) {}

void Game_GUI::placeCard(tgui::Panel::Ptr parent, const Card& card, float x, float y, float w, float h) {
    if (!card.image) return;
    card.image->setPosition({x, y});
    card.image->setSize({w, h});
    parent->add(card.image);
}

void Game_GUI::placeFaceDownPair(tgui::Panel::Ptr parent, float leftX, float topY, float cardW, float cardH, float gap) {
    auto card1 = tgui::Picture::create(CARD_BACK_PATH);
    auto card2 = tgui::Picture::create(CARD_BACK_PATH);
    card1->setPosition({leftX, topY});
    card1->setSize({cardW, cardH});
    card2->setPosition({leftX + cardW + gap, topY});
    card2->setSize({cardW, cardH});
    parent->add(card1);
    parent->add(card2);
}

void Game_GUI::placeCenteredLabel(tgui::Panel::Ptr parent, const tgui::Label::Ptr& label, float centerX, float topY, float minX, float maxX) {
    const float labelX = std::clamp(centerX - label->getSize().x * 0.5f, minX, maxX - label->getSize().x);
    label->setPosition({labelX, topY});
    parent->add(label);
}

void Game_GUI::placePlayerName(tgui::Panel::Ptr parent, const std::string& name, const TableSeat& seat, float guiW, float guiH, const GuiTheme& color) {
    auto label = tgui::Label::create(name);
    label->setTextSize(scaledTextSize(guiH, 0.018f, 18, 28));
    label->getRenderer()->setTextColor(color.text);
    const float centerX = seatPairCenterX(seat, guiW);
    const float topY = seat.topY * guiH + seat.cardH * guiH + 10.f;
    placeCenteredLabel(parent, label, centerX, topY, 20.f, guiW - 20.f);
}

void Game_GUI::refreshCommunity() {
    communityPanel->removeAllWidgets();
    const auto visible = engine->getVisibleCommunityCards();
    float cx = 0.f;
    for (size_t i = 0; i < visible.size() && i < 5; i++) {
        placeCard(communityPanel, visible[i], cx, 0.f, communityCardW, communityCardH);
        cx += communityCardW + communityGap;
    }
}

void Game_GUI::disableActions() {
    for (const auto& button : actionButtons) {
        button->setEnabled(false);
    }
}

void Game_GUI::refreshHud() {
    auto& players = engine->getPlayers();
    if (humanPlayerIndex >= players.size()) return;

    const float guiW = getGuiWidth();
    auto centerLabel = [&](const tgui::Label::Ptr& label, float cx, float y) {
        float lx = std::clamp(cx - label->getSize().x * 0.5f, 20.f, guiW - 20.f - label->getSize().x);
        label->setPosition({lx, y});
    };

    balanceLabel->setText("Balance: " + std::to_string(players[humanPlayerIndex]->getMoney()) + "$");
    centerLabel(balanceLabel, balanceCenterX, balanceY);

    potLabel->setText("Pot: " + std::to_string(engine->getPot()) + "$");
    centerLabel(potLabel, getGuiWidth() / 2.f, communityY - potLabel->getSize().y - 18.f);

    winnerLabel->setText(engine->getWinnerMessage());
    centerLabel(winnerLabel, getGuiWidth() / 2.f, communityY + communityCardH + 22.f);

    refreshCommunity();

    if (engine->getPhase() == GamePhase::SHOWDOWN) {
        disableActions();
    }
}


void Game_GUI::showEscapeMenu(){


    if (escapeMenu){
        return;
    }
    
    opened = true;

    showOverlay();
    escapeMenu = tgui::Panel::create({Constants::signUpPanelWidth, Constants::signUpPanelHeight});
    escapeMenu->setPosition({(getGuiWidth() - Constants::signUpPanelHeight) / 2.f, (getGuiHeight() - Constants::signUpPanelHeight) / 2.f});
    escapeMenu->getRenderer()->setBackgroundColor(sf::Color(45, 35, 28, 230));
    escapeMenu->getRenderer()->setRoundedBorderRadius(20.0);

    root->add(escapeMenu);

    GuiTheme color;

    auto escapeMenuTitle = tgui::Label::create("Paused game");
    escapeMenuTitle->setPosition({70,20});
    escapeMenuTitle->setTextSize(28);
    escapeMenuTitle->setSize({500, 60});
    escapeMenuTitle->getRenderer()->setBackgroundColor(sf::Color(0,0,0,100));
    escapeMenuTitle->getRenderer()->setTextColor(color.text);

    escapeMenu->add(escapeMenuTitle);


    std::vector<tgui::Button::Ptr> escapeButtons;

    auto backToGameButton = tgui::Button::create("Back to Game");
    backToGameButton->setPosition({70, 100});

    auto restartGameButton = tgui::Button::create("Restart game");
    restartGameButton->setPosition({70, 200});


    auto backToMenuButton = tgui::Button::create({"Back to Menu"});
    backToMenuButton->setPosition({70, 300});


    auto quitGameButton = tgui::Button::create({"Exit Game"});
    quitGameButton->setPosition({70, 400});

    escapeButtons.push_back(backToGameButton);
    escapeButtons.push_back(restartGameButton);
    escapeButtons.push_back(backToMenuButton);
    escapeButtons.push_back(quitGameButton);

    
    for (auto button : escapeButtons){
        
        button->setTextSize(28);
        button->setSize({500, 60});
        button->getRenderer()->setBackgroundColor(sf::Color(0,0,0,100));
        button->getRenderer()->setTextColor(color.text);
        button->getRenderer()->setTextColorHover(color.accent);
        button->getRenderer()->setBackgroundColorDown(color.down);
        button->getRenderer()->setBackgroundColorHover(color.bg);
        button->getRenderer()->setBorders(0);

        escapeMenu->add(button);
    }


    bindSoundOnClick(backToGameButton, [this]{
        hideOverlay();
        root->remove(escapeMenu);
        escapeMenu = nullptr;
    });

    bindSoundOnClick(backToMenuButton, [this]{
        hideOverlay();
        root->remove(escapeMenu);
        escapeMenu = nullptr;
        onExit();
    });

    bindSoundOnClick(quitGameButton, [this]{
        hideOverlay();
        root->remove(escapeMenu);
        escapeMenu = nullptr;
        window.close();
    });

}



void Game_GUI::game() {
    GuiTheme color;

    gui.removeAllWidgets();
    actionButtons.clear();
    tgui::Font::setGlobalFont("assets/fonts/Shelten.ttf");

    engine = std::make_unique<Game>();
    engine->game();

    root = tgui::Panel::create({getGuiWidth(), getGuiHeight()});
    root->getRenderer()->setTextureBackground("assets/images/backgrounds/game_background.png");
    gui.add(root);

    auto topBar = bar();
    volume(topBar, root, color, music);

    const float guiW = getGuiWidth();
    const float guiH = getGuiHeight();

    // Community card panel
    const auto& communitySlot = TableLayout::COMMUNITY;
    const float communityStartX = communitySlot.leftX * guiW;

    communityY = communitySlot.topY * guiH;
    communityCardW = communitySlot.cardW * guiW;
    communityCardH = communitySlot.cardH * guiH;
    communityGap = communitySlot.gap * guiW;

    const float communityPanelW = 5.f * communityCardW + 4.f * communityGap;

    communityPanel = tgui::Panel::create({communityPanelW, communityCardH});
    communityPanel->setPosition({communityStartX, communityY});
    communityPanel->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
    root->add(communityPanel);

    auto& players = engine->getPlayers();
    size_t aiSeat = 0;
    humanPlayerIndex = players.size();

    for (size_t i = 0; i < players.size(); ++i) {
        auto& player = players[i];
        const bool isHuman = player->getName() == "Player";
        if (!isHuman && aiSeat >= TableLayout::OPPONENT_SEATS.size()) continue;

        const auto& seat = isHuman ? TableLayout::HUMAN : TableLayout::OPPONENT_SEATS[aiSeat++];
        const float seatX = seat.leftX * guiW;
        const float seatY = seat.topY * guiH;
        const float cardW = seat.cardW * guiW;
        const float cardH = seat.cardH * guiH;
        const float gap   = seat.gap * guiW;

        if (isHuman) {
            humanPlayerIndex = i;
            const auto& hole = player->getcard2();
            float holeX = seatX;
            for (size_t j = 0; j < hole.size() && j < 2; j++) {
                placeCard(root, hole[j], holeX, seatY, cardW, cardH);
                holeX += cardW + gap;
            }

            balanceLabel = tgui::Label::create("Balance: " + std::to_string(player->getMoney()) + "$");
            balanceLabel->setTextSize(scaledTextSize(guiH, 0.021f, 20, 30));
            balanceLabel->getRenderer()->setTextColor(color.text);

            balanceCenterX = seatPairCenterX(seat, guiW);
            balanceY = std::max(Constants::topBarHeight + 10.f, seatY - balanceLabel->getSize().y - 14.f);
            placeCenteredLabel(root, balanceLabel, balanceCenterX, balanceY, 20.f, guiW - 20.f);

            placePlayerName(root, "You", seat, guiW, guiH, color);
        } else {
            placeFaceDownPair(root, seatX, seatY, cardW, cardH, gap);
            placePlayerName(root, player->getName(), seat, guiW, guiH, color);
        }
    }

    potLabel = tgui::Label::create("Pot: " + std::to_string(engine->getPot()) + "$");
    potLabel->setTextSize(scaledTextSize(guiH, 0.026f, 24, 40));
    potLabel->getRenderer()->setTextColor(color.accent);
    placeCenteredLabel(root, potLabel, guiW / 2.f, communityY - potLabel->getSize().y - 18.f, 20.f, guiW - 20.f);

    winnerLabel = tgui::Label::create("");
    winnerLabel->setTextSize(scaledTextSize(guiH, 0.022f, 22, 34));
    winnerLabel->getRenderer()->setTextColor(color.text);
    placeCenteredLabel(root, winnerLabel, guiW / 2.f, communityY + communityCardH + 22.f, 20.f, guiW - 20.f);
    

    if (humanPlayerIndex < players.size()) {
        const float controlsY = getGuiHeight() - 70.f; 
        constexpr float btnW = 124.f;
        constexpr float btnH = 48.f;
        constexpr float btnGap = 12.f;
        constexpr float startX = 50.f;
        const float y = controlsY - (btnH + btnGap);

        actionStatus = tgui::Label::create("Choose action");
        
        actionStatus->setTextSize(20);
        actionStatus->getRenderer()->setTextColor(color.text);
        actionStatus->setPosition({startX, y + 80});
        root->add(actionStatus);

        auto placeActionButton = [&](const std::string& text, float x, ActionType action, std::string statusMsg, int raiseAmount = 0) {
            
            auto button = tgui::Button::create(text);
            if (action == ActionType::FOLD || action == ActionType::RAISE || action == ActionType::ALLIN){
                button->setPosition({x, controlsY});
            }
            else {
                button->setPosition({x, y});
            }
            button->setSize({btnW, btnH});
            button->setTextSize(20);
            button->getRenderer()->setTextColor(color.text);
            button->getRenderer()->setTextColorDisabled(tgui::Color(190, 178, 150));
            button->getRenderer()->setBackgroundColor(color.panel2);
            button->getRenderer()->setBackgroundColorHover(color.border);
            button->getRenderer()->setBackgroundColorDown(color.down);
            button->getRenderer()->setBackgroundColorDisabled(tgui::Color(74, 66, 58, 230));
            root->add(button);
            actionButtons.push_back(button);
            
            bindSoundOnClick(button, [this, action, msg = std::move(statusMsg), raiseAmount] {
                engine->playerAction(action, raiseAmount);
                actionStatus->setText(msg);
                refreshHud();
            });

            // if ()
        };

        placeActionButton("Fold",startX ,ActionType::FOLD,  "Action: Fold");
        placeActionButton("Raise +10",startX + (btnW + btnGap) * 1.f,  ActionType::RAISE, "Action: Raise", 10);
        placeActionButton("All In",startX + (btnW + btnGap) * 2.f,  ActionType::ALLIN, "Action: All In");

        placeActionButton("Check",startX,  ActionType::CHECK, "Action: Check");
        placeActionButton("Call",startX + (btnW + btnGap) * 1.f,  ActionType::CALL,  "Action: Call");
    }
}
