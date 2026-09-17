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

Game_GUI::Game_GUI(tgui::Gui& gui, sf::Music& music, sf::RenderWindow& window)
    : Gui_base(gui, music),  window(window){}

void Game_GUI::placeCard(tgui::Panel::Ptr parent, const Card& card, float x, float y, float w, float h) {
    if (card.imagePath.empty()) return;

    auto image = tgui::Picture::create(tgui::Texture(card.imagePath, {}, {}, true));
    image->setPosition({x, y});
    image->setSize({w, h});
    parent->add(image);
}

void Game_GUI::placeFaceDownPair(tgui::Panel::Ptr parent, float leftX, float topY, float cardW, float cardH, float gap) {
    const auto createCardBack = [parent, cardW, cardH](float x, float y) {
        auto card = tgui::Panel::create({cardW, cardH});
        card->setPosition({x, y});
        card->getRenderer()->setTextureBackground(CARD_BACK_PATH);
        parent->add(card);
    };

    createCardBack(leftX, topY);
    createCardBack(leftX + cardW + gap, topY);
}

void Game_GUI::placeCenteredLabel(const tgui::Label::Ptr& label, float centerX, float topY, float minX, float maxX) {
    const float labelX = std::clamp(centerX - label->getSize().x * 0.5f, minX, maxX - label->getSize().x);
    label->setPosition({labelX, topY});
}

void Game_GUI::revealShowdownCards() {
    const float guiW = getGuiWidth();
    const float guiH = getGuiHeight();
    auto& players = engine->getPlayers();

    std::size_t aiSeat = 0;

    for (const auto& player : players) {
        if (player->isRealPlayer()) {
            continue; // tvoje karty uz vidis
        }

        const auto& seat = TableLayout::OPPONENT_SEATS[aiSeat++];

        if (!player->isInGame()) {
            continue; // foldnuteho bota neukazuj
        }

        float x = seat.leftX * guiW;
        const float y = seat.topY * guiH;
        const float cardW = seat.cardW * guiW;
        const float cardH = seat.cardH * guiH;
        const float gap = seat.gap * guiW;

        for (const Card& card : player->getcard2()) {
            placeCard(root, card, x, y, cardW, cardH);
            x += cardW + gap;
        }
    }
}


void Game_GUI::createCurrentBetLabel(){
    float guiW = getGuiWidth();
    float guiH = getGuiHeight(); 

    auto& players = engine->getPlayers();


    currentBetLabels.clear();
    currentBetLabels.resize(players.size());
    
    int aiSeat = 0;

    for (std::size_t i = 0; i < players.size(); ++i){ 
        auto& player = players[i];   
        const auto& seat = player->isRealPlayer() ? TableLayout::PLAYER_BET : TableLayout::OPPONENTS_BET[aiSeat++]; 
        auto x = seat.x * guiW; 
        auto y = seat.y * guiH; 

        tgui::Label::Ptr currentBet;
        if (player->isRealPlayer()){
                currentBet = makeLabel("Current bet: " + std::to_string(players[i]->getBetThisRound()) + "$", 24, color.text, root);
        }
        else {
            currentBet = makeLabel(std::to_string(players[i]->getBetThisRound()) + "$", 20, color.text, root);
        }
        currentBet->setPosition(x,y);
        currentBetLabels[i] = currentBet;
    }
}
 

void Game_GUI::createMoneyLabelsPlayers(){
    
    float guiW = getGuiWidth();
    float guiH = getGuiHeight();

    auto& players = engine->getPlayers();

    moneyLabels.clear();
    moneyLabels.resize(players.size());

    std::size_t seatIndex = 0;
    
    for (std::size_t i = 0; i < players.size(); ++i){
        if (players[i]->isRealPlayer()){
            continue;
        }
        auto position = TableLayout::OPPONENT_SEATS[seatIndex++];
        auto x = position.leftX * guiW + 65;
        auto y = position.topY * guiH - 40;
        auto moneyLabel = makeLabel(std::to_string(players[i]->getMoney()) + "$", 20, color.text, root);
        moneyLabel->setPosition(x,y);
        moneyLabels[i] = moneyLabel;
    }
    
}
tgui::Panel::Ptr Game_GUI::makeCirclePanel(std::string& name, tgui::Vector2f pos){
    const bool isPlayer = (name == "Player");

    const float radius = isPlayer ? getGuiWidth() * 0.036458f : getGuiWidth() * 0.026042f;
    const float gap = isPlayer ? getGuiWidth() * 0.046875f : getGuiWidth() * 0.036458f;

    auto circle = makePanel({radius, radius}, color.centerpanel, root);
    circle->getRenderer()->setRoundedBorderRadius(radius/2);
    circle->setPosition(pos.x - gap, pos.y + gap - getGuiHeight() * 0.027778f);
    return circle;
}

void Game_GUI::placePlayerName(tgui::Panel::Ptr parent, const std::string& name, const TableSeat& seat, float guiW, float guiH) {
    auto label = tgui::Label::create(name);
    label->setTextSize(scaledTextSize(guiH, 0.018f, 18, 28));
    label->getRenderer()->setTextColor(color.text);
    const float centerX = seatPairCenterX(seat, guiW);
    const float topY = seat.topY * guiH + seat.cardH * guiH + 10.f;
    placeCenteredLabel(label, centerX, topY, 20.f, guiW - 20.f);
    playerNameLabels.push_back(label);
    parent->add(label);
}

void Game_GUI::placeBlind(std::string& name, const std::string& title, tgui::Vector2f pos){
    auto blindPanel = makeCirclePanel(name, pos);
    auto blindLabel = makeLabel(title, 12, tgui::Color::White, blindPanel);
    blindLabel->setSize({"100%", "100%"});
    blindLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
    blindLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
}


void Game_GUI::refreshCommunity() {
    communityPanel->removeAllWidgets();
    const auto visible = engine->getVisibleCommunityCards();
    float cx = 0.f;
    for (size_t i = 0; i < visible.size() && i < 5; i++) {
        placeCard(communityPanel, visible[i], cx + 15, 0.f + 15, communityCardW, communityCardH);
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

    potLabel->setText("Pot: " + std::to_string(engine->getPot()) + "$");

    placeCenteredLabel(actionStatus, guiW/2.f, 80, 20.f, guiW - 20.f);

    winnerLabel->setText(engine->getWinnerMessage());
    centerLabel(winnerLabel, getGuiWidth() / 2.f, communityY + communityCardH + 22.f);
    refreshCommunity();

    for(std::size_t i = 0; i < players.size(); i++){
        if(moneyLabels[i]) moneyLabels[i]->setText(std::to_string(players[i]->getMoney()) + "$");
        if (i < currentBetLabels.size() && currentBetLabels[i]) {
            if (players[i]->isRealPlayer()) {
                currentBetLabels[i]->setText("Current bet: " + std::to_string(players[i]->getBetThisRound()) + "$");
            } else {
                currentBetLabels[i]->setText(std::to_string(players[i]->getBetThisRound()) + "$");
            }
        }
    }

    if (engine->getPhase() == GamePhase::SHOWDOWN) {
        disableActions();

        if(!restartPending){
            restartPending = true;
            restartClock.restart();
        }
    }
}

void Game_GUI::restartGame(){
    const int playerCount = engine->getNumberOfPlayers();
    const int smallBlind = engine->getSmallBlind();

    gui.removeAllWidgets();
    actionButtons.clear();
    communityPanel = nullptr;
    balanceLabel = nullptr;
    potLabel = nullptr;
    winnerLabel = nullptr;
    actionStatus = nullptr;
    raiseSlider = nullptr;
    raiseMoneyLabel = nullptr;
    confirmButton = nullptr;
    escapeMenuPanel = nullptr;

    // game() creates the replacement engine using the settings from this hand.
    game(playerCount, smallBlind);
}

void Game_GUI::hideEscapeMenu(){
    if(escapeMenuPanel && escapeMenuPanel->isVisible()){
        root->remove(escapeMenuPanel);
        escapeMenuPanel = nullptr;
    }
    else if(gameSettingsPanel && gameSettingsPanel->isVisible()){
        root->remove(gameSettingsPanel);
        gameSettingsPanel = nullptr;
    }
    else if (audioSettingsPanel && audioSettingsPanel->isVisible()){
        root->remove(audioSettingsPanel);
        audioSettingsPanel = nullptr;
    }
    hideOverlay();
}

void Game_GUI::escape(){
    handleEscape();
    if(escapeMenuPanel == nullptr && openedSettingsPanel == true)  showEscapeMenu();
}


void Game_GUI::showEscapeMenu(){

    showOverlay();

    const float panelW = Constants::Panel::signUpWidthRatio * getGuiWidth();
    const float panelH = Constants::Panel::loginHeightRatio * 1.5f * getGuiHeight();
    escapeMenuPanel = makePanel({panelW, panelH}, color.centerpanel, root);

    auto escapeMenuTitle = makeLabel("Paused", 36, color.text, escapeMenuPanel);
    escapeMenuTitle->setPosition({(panelW - escapeMenuTitle->getSize().x) / 2.f, panelH * 0.030303f});
    const float buttonW = panelW * 0.806452f;
    const float buttonH = panelH * 0.10f;
    const float startX = (panelW - buttonW) / 2.f;


    auto backToGameButton = makeButton("Back to Game", {startX, panelH * 0.181818f}, {buttonW, buttonH}, 28, escapeMenuPanel);
    auto restartGameButton = makeButton("Restart game", {startX, panelH * 0.333333f}, {buttonW, buttonH}, 24, escapeMenuPanel);
    auto settingsButton = makeButton("Settings", {startX, panelH * 0.484848f}, {buttonW, buttonH}, 24, escapeMenuPanel);
    auto backToMenuButton = makeButton("Back to Menu", {startX, panelH * 0.636364f}, {buttonW, buttonH}, 24, escapeMenuPanel);
    auto quitGameButton = makeButton("Exit Game", {startX, panelH * 0.787879f}, {buttonW, buttonH}, 24, escapeMenuPanel);
    
    bindSoundOnClick(backToGameButton, [this]{
        hideEscapeMenu();
    });


    bindSoundOnClick(restartGameButton, [this]{
        restartGame();
    });

    bindSoundOnClick(settingsButton, [this]{
        game_settings();
    });

    bindSoundOnClick(backToMenuButton, [this]{
        ambienceSound.stop();
        hideOverlay();
        root->remove(escapeMenuPanel);
        escapeMenuPanel = nullptr;
        onExit();
    });

    bindSoundOnClick(quitGameButton, [this]{

        escapeMenuPanel->setVisible(false);

        tgui::ChildWindow::Ptr quitPanel = makePopUpPanel("Do you want to exit?", {420.f, 190.f}, 22, color.down, sf::Color(253, 253, 253), false);
        tgui::Button::Ptr yesButton = makePopUpButton("Yes", {45.f, 50.f}, {120,50}, 20, quitPanel);
        tgui::Button::Ptr noButton = makePopUpButton("No", {235, 50.f}, {120,50}, 20, quitPanel);


        bindSoundOnClick(yesButton, [this]
        {
            root->remove(escapeMenuPanel);
            escapeMenuPanel = nullptr;
            window.close();
        });

        bindSoundOnClick(noButton, [this, quitPanel]
        {
            root->remove(quitPanel);
            escapeMenuPanel->setVisible(true);
        });
    });

}

void Game_GUI::updateBotWaiting(){
    if (!engine || engine->getPhase() == GamePhase::SHOWDOWN) {
        return;
    }

    auto& players = engine->getPlayers();
    if (players.empty()) {
        return;
    }

    const auto needsAction = [](const std::unique_ptr<Player>& player) {
        return player->isInGame() && !player->getIsEliminated() &&
               !player->getAllIn() && !player->getActed();
    };

    if (!std::any_of(players.begin(), players.end(), needsAction)) {
        engine->advancePhase();
        if (engine->getPhase() == GamePhase::SHOWDOWN) {
            resolveShowdown();
            return;
        }
        playerTurn = 0;
        updateBotWaiting();
        return;
    }

    // At most one full pass: a folded or all-in player can never stall this loop.
    for (std::size_t attempts = 0; attempts < players.size(); ++attempts) {
        if (playerTurn < 0 || playerTurn >= static_cast<int>(players.size())) {
            playerTurn = 0;
        }

        auto& player = players[playerTurn];
        if (!needsAction(player)) {
            playerTurn = (playerTurn + 1) % static_cast<int>(players.size());
            continue;
        }

        if (player->isRealPlayer()) {
            const int toCall = std::max(0, engine->getCurrentBet() - player->getBetThisRound());
            for (const auto& button : actionButtons) {
                button->setVisible(false);
            }

            foldButton->setVisible(true);
            if (toCall == 0) {
                checkButton->setVisible(true);
                raiseButton->setVisible(player->getMoney() > 0);
                allInButton->setVisible(player->getMoney() > 0);
            } else if (player->getMoney() <= toCall) {
                allInButton->setVisible(player->getMoney() > 0);
            } else {
                callButton->setVisible(true);
                raiseButton->setVisible(true);
                allInButton->setVisible(true);
            }
            return;
        }

        static std::mt19937 randomTimeForBot(std::random_device{}());
        std::uniform_real_distribution<float> dist(1.f, 4.f);
        timeForBot = (playerTurn == 1 || playerTurn == 2) ? 1.f : dist(randomTimeForBot);
        botWaiting = true;
        botChooseClock.restart();

        if (playerTurn < static_cast<int>(panels.size()) && panels[playerTurn] &&
            playerTurn > 0 && static_cast<std::size_t>(playerTurn - 1) < TableLayout::OPPONENT_SEATS.size() &&
            playerTurn < static_cast<int>(playerNameLabels.size()) && playerNameLabels[playerTurn]) {
            const float guiW = getGuiWidth();
            const float guiH = getGuiHeight();
            const auto& seat = TableLayout::OPPONENT_SEATS[playerTurn - 1];

            panels[playerTurn]->setVisible(true);
            playerNameLabels[playerTurn]->setText(players[playerTurn]->getName() + "'s turn");
            const float centerX = seatPairCenterX(seat, guiW);
            const float topY = seat.topY * guiH + seat.cardH * guiH + 10.f;
            placeCenteredLabel(playerNameLabels[playerTurn], centerX, topY, 20.f, guiW - 20.f);
        }
        return;
    }
}

void Game_GUI::game(int playerCount, int smallBlind) {
    gui.removeAllWidgets();
    actionButtons.clear();
    panels.clear();
    choiceLabels.clear();
    playerNameLabels.clear();
    moneyLabels.clear();
    currentBetLabels.clear();
    playerTurn = 1;
    smallBigCounter = 2;
    botWaiting = false;
    tgui::Font::setGlobalFont("assets/fonts/Shelten.ttf");

    
    if(!ambienceSound.openFromFile("assets/sound/ambienceSound.ogg")){
        std::cerr<< "Ambience sound is not loaded or other big problem. its cooked\n";
        return;     
    }
    ambienceSound.play();
    ambienceSound.setVolume(10);
    ambienceSound.setLooping(true);

    
    engine = std::make_unique<Game>();
    engine->setSmallBlind(smallBlind);
    engine->setNumberOfPlayers(playerCount);

    engine->game();
    buildTable();

}

void Game_GUI::nextHand(){
    gui.removeAllWidgets();
    actionButtons.clear();
    choiceLabels.clear();
    playerNameLabels.clear();
    panels.clear();
    botWaiting = false;
    timeForBot = 0.f;
    playerTurn = 1;
    smallBigCounter = 2;
    tgui::Font::setGlobalFont("assets/fonts/Shelten.ttf");
    engine->newHand();
    buildTable();
}

void Game_GUI::nextBot(){

    if (!engine || !botWaiting) {
        return;
    }

    botWaiting = false;

    auto& players = engine->getPlayers();
    if (playerTurn <= 0 || playerTurn >= static_cast<int>(players.size()) ||
        static_cast<std::size_t>(playerTurn - 1) >= TableLayout::OPPONENT_SEATS.size()) {
        updateBotWaiting();
        return;
    }

    const float guiW = getGuiWidth();
    const float guiH = getGuiHeight();
    int i = playerTurn - 1;
    const auto& seat = TableLayout::OPPONENT_SEATS[i];

    const float cardH = seat.cardH * guiH;
    const float centerX = seatPairCenterX(seat, guiW);
    const float topChoiceY = seat.topY * guiH + seat.cardH * guiH + 35.f;


    if (playerTurn < static_cast<int>(panels.size()) && panels[playerTurn]) {
        panels[playerTurn]->setVisible(false);
        playerNameLabels[playerTurn]->setText(players[playerTurn]->getName());

        const float topY = seat.topY * guiH + seat.cardH * guiH + 10.f;

        placeCenteredLabel(playerNameLabels[playerTurn], centerX, topY, 20.f, guiW - 20.f);


        if (smallBigCounter > 0) {
            if(players[playerTurn]->getIsSmallBlind()){
                const int paid = std::min(engine->getSmallBlind(), players[playerTurn]->getMoney());
                players[playerTurn]->getMoney() -= paid;
                players[playerTurn]->getBetThisRound() += paid;
                engine->setPot(paid);
                engine->setCurrentBet(std::max(engine->getCurrentBet(), players[playerTurn]->getBetThisRound()));
                currentBetLabels[playerTurn]->setText(std::to_string(players[playerTurn]->getBetThisRound()) + "$");
                smallBigCounter--;           
            }
            else if (players[playerTurn]->getIsBigBlind()){
                const int paid = std::min(engine->getBigBlind(), players[playerTurn]->getMoney());
                players[playerTurn]->getMoney() -= paid;
                players[playerTurn]->getBetThisRound() += paid;
                engine->setPot(paid);
                engine->setCurrentBet(std::max(engine->getCurrentBet(), players[playerTurn]->getBetThisRound()));
                currentBetLabels[playerTurn]->setText(std::to_string(players[playerTurn]->getBetThisRound()) + "$");
                smallBigCounter--;
            }
        }
        else {
            auto choice = engine->botChoice(playerTurn);
            std::string choiceLabelText = engine->toStringActionType(choice);
            
            if (i < static_cast<int>(choiceLabels.size()) && choiceLabels[i]) {
                if (choice == ActionType::RAISE) choiceLabels[i]->setText(choiceLabelText + " " + std::to_string(engine->raiseAmount) + "$");
                else choiceLabels[i]->setText(choiceLabelText);
                placeCenteredLabel(choiceLabels[i], centerX, topChoiceY, 20.f, guiW - 20.f);
            }
            if (playerTurn < static_cast<int>(currentBetLabels.size()) && currentBetLabels[playerTurn]) {
                currentBetLabels[playerTurn]->setText(std::to_string(players[playerTurn]->getBetThisRound()) + "$");
            }
        }
    }

    if(playerTurn == players.size() - 1){
        playerTurn = 0;
    }
    else {
        playerTurn++;
    }
    
    updateBotWaiting();
    refreshHud();
}


void Game_GUI::buildTable(){
    
    root = tgui::Panel::create({getGuiWidth(), getGuiHeight()});
    root->getRenderer()->setTextureBackground("assets/images/backgrounds/game_background.png");
    gui.add(root);
    
    auto topBar = makeBar({0,0});
    const float bottomBarH = Constants::Bar::bottomHeightRatio * getGuiHeight();
    auto bottomBar = makeBar({0, getGuiHeight() - bottomBarH});
    bottomBar->setSize({getGuiWidth(), bottomBarH});


    volume(topBar, root, mainMenuMusic);

    const float guiW = getGuiWidth();
    const float guiH = getGuiHeight();
    const float topBarH = Constants::Bar::topHeightRatio * guiW;

    // Community card panel
    const auto& communitySlot = TableLayout::COMMUNITY;
    const float communityStartX = communitySlot.leftX * guiW;

    communityY = communitySlot.topY * guiH;
    communityCardW = communitySlot.cardW * guiW;
    communityCardH = communitySlot.cardH * guiH;
    communityGap = communitySlot.gap * guiW;

    const float communityPanelW = 5.f * communityCardW + 7.f * communityGap;
    const float communityPanelH = communitySlot.cardH * guiH + 3 * communityGap;
    
    communityPanel = makePanel({communityPanelW, communityPanelH}, color.gamePanel, root);
    communityPanel->setPosition({(getGuiWidth() - communityPanel->getSize().x) / 2.f, (getGuiHeight() - communityPanel->getSize().y) / 2.f });

    auto& players = engine->getPlayers();
    size_t aiSeat = 0;
    humanPlayerIndex = players.size();

    

    float size_x = guiW * 0.102;
    float size_y = guiH * 0.2;    

    panels.clear();
    panels.resize(players.size());
    std::size_t turnSeat = 0;

    for (std::size_t i = 0; i < players.size(); ++i) {
        auto& player = players[i];
        if (!player->isRealPlayer() && player->isInGame() && !player->getIsEliminated() && turnSeat < TableLayout::OPPONENT_TURNS.size()) {
            const auto& seat = TableLayout::OPPONENT_TURNS[turnSeat++];
            auto x = seat.x * guiW;
            auto y = seat.y * guiH;

            auto panel = makePanel({size_x, size_y}, color.gamePanel, root);
            panel->setPosition({x,y});
            panel->setVisible(false);
            panels[i] = panel;
        }
    }

    createMoneyLabelsPlayers();
    createCurrentBetLabel();

    playerNameLabels.push_back(makeLabel("Player", 20, color.text, root));
    playerNameLabels[0]->setVisible(false);

    for (size_t i = 0; i < players.size(); ++i) {
        auto& player = players[i];
        std::string name = player->getName();
        if (!player->isRealPlayer() && aiSeat >= TableLayout::OPPONENT_SEATS.size()) continue;

        const auto& seat = player->isRealPlayer() ? TableLayout::HUMAN : TableLayout::OPPONENT_SEATS[aiSeat++];
        const float seatX = seat.leftX * guiW;
        const float seatY = seat.topY * guiH;
        const float cardW = seat.cardW * guiW;
        const float cardH = seat.cardH * guiH;
        const float gap = seat.gap * guiW;

        if (player->getIsSmallBlind()){
            placeBlind(name, "SMALL\nBLIND", {seatX,seatY});
        }
        if (player->getIsBigBlind()){
            placeBlind(name, "BIG\nBLIND", {seatX,seatY});
        }
        if (player->getIsDealer()){
            placeBlind(name, "DEALER", {seatX, seatY});
        }

        if (player->isRealPlayer()) {
            humanPlayerIndex = i;
            const auto& hole = player->getcard2();
            float holeX = seatX;
            for (size_t j = 0; j < hole.size() && j < 2; j++) {
                placeCard(root, hole[j], holeX, seatY, cardW, cardH);
                holeX += cardW + gap;
            }
            
            balanceLabel = makeLabel("Balance: " + std::to_string(player->getMoney()) + "$", 20, color.text, topBar);
            balanceLabel->setPosition({guiW * 0.01, topBarH * 0.20f});

        } else {
            placeFaceDownPair(root, seatX, seatY, cardW, cardH, gap);
            placePlayerName(root, player->getName(), seat, guiW, guiH);
            
            const float centerX = seatPairCenterX(seat, guiW);

            auto choiceLabel = makeLabel("", 18, color.text, root);
            choiceLabels.push_back(choiceLabel);
        }
    }

    for(size_t i = 0; i < players.size(); i++){
            if(players[i]->getName() == "Player"){
                if(players[i]->getMoney() <= 0 ){  

                tgui::ChildWindow::Ptr gameOverPanel = makePopUpPanel("Game Over", {Constants::Panel::loginWidthRatio * guiW, guiH * 0.277778f}, 32, sf::Color(128, 30, 30), sf::Color(253, 253, 253), true);
                tgui::Button::Ptr restartGameButton = makePopUpButton("Restart game", {(gameOverPanel->getSize().x - 500.f) / 2.f, 40.f}, {500,66}, 24, gameOverPanel);
                tgui::Button::Ptr exitGameButton = makePopUpButton("Exit game", {(gameOverPanel->getSize().x - 500.f) / 2.f, 140.f}, {500,66}, 24, gameOverPanel);

                bindSoundOnClick(restartGameButton,[this]{
                    restartGame();
                });

                bindSoundOnClick(exitGameButton, [gameOverPanel, this]{
                    root->remove(gameOverPanel);
                    onExit();
                });
                return;
            }
        }
    }


    potLabel = makeLabel("Pot: " + std::to_string(engine->getPot()) + "$", scaledTextSize(guiH, 0.026f, 24, 40), color.accent, root);
    placeCenteredLabel(potLabel, guiW / 2.f, communityY - potLabel->getSize().y - 18.f, 20.f, guiW - 20.f);

    winnerLabel = makeLabel("", scaledTextSize(guiH, 0.022f, 22, 34), color.text, root);
    placeCenteredLabel(winnerLabel, guiW / 2.f, communityY + communityCardH + 22.f, 20.f, guiW - 20.f);
    

    if (humanPlayerIndex < players.size()) {
        const float controlsY = guiH * 0.944444f;
        const float btnW = guiW * 0.064583f;
        const float btnH = guiH * 0.044444f;
        const float btnGap = guiW * 0.010417f;
        const float actionRowW = 5.f * btnW + 4.f * btnGap;
        const float startX = (guiW - actionRowW) / 2.f;

        actionStatus = makeLabel("Choose action", 24, color.text, root);
        placeCenteredLabel(actionStatus, guiW / 2.f, guiH * 0.074074f, guiW * 0.010417f, guiW * 0.989583f);

        root->remove(raiseSlider);
        root->remove(confirmButton);

        raiseSlider = nullptr;
        confirmButton = nullptr;

        confirmButton = makeButton("Confirm", {startX + (btnW + btnGap) * 7.f, controlsY}, {btnW, btnH}, 20, root);
        confirmButton->setVisible(false);

        raiseMoneyLabel = makeLabel("", 20, color.text, root);

        bindSoundOnClick(confirmButton, [this]{
            auto& players = engine->getPlayers();
            if (!raiseSlider) return;
            int amount = static_cast<int>(raiseSlider->getValue());
            engine->playerAction(ActionType::RAISE, amount);
            if (engine->getPhase() == GamePhase::SHOWDOWN) {
                resolveShowdown();
            }
            const int toCall = std::max(0, engine->getCurrentBet() - players[humanPlayerIndex]->getBetThisRound());
            raiseSlider->setMaximum(std::max(0, players[humanPlayerIndex]->getMoney() - toCall));
            raiseSlider->setVisible(false);
            raiseSlider->setValue(0);
            confirmButton->setVisible(false);
            raiseMoneyLabel->setVisible(false);
            actionStatus->setText("Action: Raise to " + std::to_string(engine->raiseAmount) + " $");

            if (engine->getPhase() != GamePhase::SHOWDOWN) {
                for (const auto& button : actionButtons) {
                    button->setVisible(false);
                }
                playerTurn = static_cast<int>(humanPlayerIndex) + 1;
                updateBotWaiting();
            }

            refreshHud();
        });

        auto placeActionButton = [this, controlsY, startX, btnW, btnH, btnGap](tgui::Button::Ptr button, ActionType action, std::string statusMsg) {
            bindSoundOnClick(button, [this, action, msg = std::move(statusMsg), controlsY, startX, btnW, btnH, btnGap] {
            auto& players = engine->getPlayers();
                if (action == ActionType::RAISE){
                    if (!raiseSlider){
                        raiseSlider = makeSlider({startX + (btnW + btnGap) * 3.f, controlsY - btnH * 0.35f}, {btnW * 2.f + btnGap, btnH * 0.20f}, 0, root);
                        float thumbX = raiseSlider->getPosition().x + 0.0 * raiseSlider->getSize().x;

                        const int toCall = std::max(0, engine->getCurrentBet() - players[humanPlayerIndex]->getBetThisRound());
                        raiseSlider->setMaximum(std::max(0, players[humanPlayerIndex]->getMoney() - toCall));
                        
                        raiseMoneyLabel->setPosition({thumbX - getGuiWidth() * 0.006250f, raiseSlider->getPosition().y - getGuiHeight() * 0.023148f});
                        raiseMoneyLabel->setVisible(true);
                        raiseMoneyLabel->setText("0$");
                        
                        raiseSlider->onValueChange([this](float value){

                            if(value > 1) {
                                confirmButton->setVisible(true);
                            }
                            if (value < 1){
                                confirmButton->setVisible(false);
                            }
                            play_button();
                            float ratio = (value - raiseSlider->getMinimum()) / (raiseSlider->getMaximum() - raiseSlider->getMinimum());
                            float thumbX = raiseSlider->getPosition().x + ratio * raiseSlider->getSize().x;

                            raiseMoneyLabel->setPosition({thumbX - getGuiWidth() * 0.006250f, raiseSlider->getPosition().y - getGuiHeight() * 0.023148f});
                            raiseMoneyLabel->setText(std::to_string((int)value) + "$");
                        });
                    } else {
                        confirmButton->setVisible(false);
                        bool show = !raiseSlider->isVisible();
                        raiseSlider->setVisible(show);
                        raiseMoneyLabel->setVisible(show);
                    }
                    return;
                }

                if (raiseSlider){
                    raiseSlider->setVisible(false);
                    confirmButton->setVisible(false);
                    raiseMoneyLabel->setVisible(false);
                }
                if(players[humanPlayerIndex]->isInGame()){
                    auto choice = engine->playerAction(action, 0);
                    currentBetLabels[humanPlayerIndex]->setText("Current bet: " + std::to_string(engine->getCurrentBet()) + "$");

                }
                if (engine->getPhase() == GamePhase::SHOWDOWN) {
                    resolveShowdown();
                }
                if(engine->getPhase() != GamePhase::SHOWDOWN){
                    actionStatus->setText(msg);
                    for (const auto& actionButton : actionButtons) {
                        actionButton->setVisible(false);
                    }
                    playerTurn = static_cast<int>(humanPlayerIndex) + 1;
                    updateBotWaiting();
                }
                refreshHud();
            });
        };

    foldButton = makeButton("Fold",{startX + (btnW + btnGap) * 3.f, controlsY}, {btnW, btnH}, 20, root);
    raiseButton = makeButton("Raise", {startX + (btnW + btnGap) * 4.f, controlsY}, {btnW, btnH}, 20, root);
    allInButton = makeButton("All in", {startX + (btnW + btnGap) * 2.f, controlsY}, {btnW, btnH}, 20, root);
    checkButton = makeButton("Check", {startX, controlsY}, {btnW, btnH}, 20, root);
    callButton = makeButton("Call", {startX + (btnW + btnGap) * 1.f, controlsY}, {btnW, btnH}, 20, root);


    actionButtons.push_back(foldButton);
    actionButtons.push_back(raiseButton);
    actionButtons.push_back(allInButton);
    actionButtons.push_back(checkButton);
    actionButtons.push_back(callButton);

    for(auto& button : actionButtons){
        button->setVisible(false);
    }


    placeActionButton(foldButton, ActionType::FOLD,  "Action: Fold");
    placeActionButton(raiseButton,  ActionType::RAISE, "Action: Raise");
    placeActionButton(allInButton,  ActionType::ALLIN, "Action: All In");
    placeActionButton(checkButton,  ActionType::CHECK, "Action: Check");
    placeActionButton(callButton,  ActionType::CALL,  "Action: Call");




    updateBotWaiting();



    
    }
}


void Game_GUI::resolveShowdown() {

    revealShowdownCards();

    auto& players = engine->getPlayers();

    HandEvaluator handEvaluator;
    std::vector<Card> cards;
    std::vector<HandScore> evoluation_cards;
    auto communityCards = engine->getCommunityCards();

    for (size_t i = 0; i < players.size(); i++) {
        auto& player = players[i];
        if (!player->isInGame()) continue;
       
        cards = utils::cards7collection(players, i, communityCards);
        evoluation_cards.push_back(handEvaluator.evaluate(cards, players, communityCards, i));
    }

    if (evoluation_cards.empty()) {
        actionStatus->setText("No winner");
        return;
    }

    utils::sort_function_hand_score(evoluation_cards);

    std::vector<HandScore> ties;
    for (int i = 0; i < static_cast<int>(evoluation_cards.size()) - 1; i++) {
        if (evoluation_cards[i].rank == evoluation_cards[i+1].rank &&
            evoluation_cards[i].sum_for_tie == evoluation_cards[i+1].sum_for_tie) {
            ties.push_back(evoluation_cards[i]);
            ties.push_back(evoluation_cards[i+1]);
        }
    }

    HandScore winner = utils::max(evoluation_cards);

    if (ties.empty()) {
        actionStatus->setText("Winner: " + winner.name);
        for (auto& player: players) {
            if (winner.name == player->getName())
                player->getMoney() += engine->getPot();
        }
    } else {
        HandScore tieswinner = utils::max(ties);
        if (winner.sum_for_tie > tieswinner.sum_for_tie && winner.rank > tieswinner.rank) {
            actionStatus->setText("Winner: " + winner.name);
            for (auto& player: players) {
                if (winner.name == player->getName())
                    player->getMoney() += engine->getPot();
            }
        } else {
            std::vector<std::string> tied_names;
            for (const auto& tie : ties) {
                if (std::find(tied_names.begin(), tied_names.end(), tie.name) == tied_names.end())
                    tied_names.push_back(tie.name);
            }
            std::string winnerMessage = "";
            for (size_t n = 0; n < tied_names.size(); n++) {
                if (n > 0) winnerMessage += ", ";
                winnerMessage += tied_names[n];
            }
            actionStatus->setText("Split pot: " + winnerMessage);

            int part = engine->getPot() / static_cast<int>(tied_names.size());
            for (auto& player: players) {
                if (std::find(tied_names.begin(), tied_names.end(), player->getName()) != tied_names.end())
                    player->getMoney() += part;
            }
        }
    }
    actionStatus->setTextSize(30);

}
