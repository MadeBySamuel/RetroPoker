#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <map>
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

const char* handCategoryName(int rank) {
    static constexpr std::array<const char*, 10> names = {
        "High card", "One pair", "Two pair", "Three of a kind", "Straight",
        "Flush", "Full house", "Four of a kind", "Straight flush", "Royal flush"
    };
    return rank >= 0 && rank < static_cast<int>(names.size()) ? names[rank] : "Unknown hand";
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
        if (!player->isRealPlayer() && player->getIsEliminated()) {
            continue;
        }
        auto x = seat.x * guiW; 
        auto y = seat.y * guiH; 

        tgui::Label::Ptr currentBet;
        if (player->isRealPlayer()){
                currentBet = makeLabel("Current bet: " + std::to_string(engine->getCurrentBet()) + "$", 24, color.text, root);
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
        if (players[i]->getIsEliminated()) {
            continue;
        }
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

 tgui::Label::Ptr Game_GUI::placePlayerName(tgui::Panel::Ptr parent, const std::string& name, const TableSeat& seat, float guiW, float guiH) {
    auto label = tgui::Label::create(name);
    label->setTextSize(scaledTextSize(guiH, 0.018f, 18, 28));
    label->getRenderer()->setTextColor(color.text);
    const float centerX = seatPairCenterX(seat, guiW);
    const float topY = seat.topY * guiH + seat.cardH * guiH + 10.f;
    placeCenteredLabel(label, centerX, topY, 20.f, guiW - 20.f);
    parent->add(label);
    return label;
}

void Game_GUI::placeBlind(std::string& name, const std::string& title, tgui::Vector2f pos){
    auto blindPanel = makeCirclePanel(name, pos);
    auto blindLabel = makeLabel(title, 12, tgui::Color::White, blindPanel);
    blindLabel->setSize({"100%", "100%"});
    blindLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
    blindLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
}


void Game_GUI::refreshCommunity() {
    const auto visible = engine->getVisibleCommunityCards();
    const std::size_t visibleCount = std::min<std::size_t>(visible.size(), 5);
    if (visibleCount < communityCardReveals.size()) {
        communityPanel->removeAllWidgets();
        communityCardReveals.clear();
    }

    const float now = communityRevealClock.getElapsedTime().asSeconds();
    float nextStart = communityCardReveals.empty()
        ? now
        : std::max(now, communityCardReveals.back().startTime + 0.16f);

    for (std::size_t i = communityCardReveals.size(); i < visibleCount; ++i) {
        if (visible[i].imagePath.empty()) {
            break;
        }

        const tgui::Vector2f targetPosition{
            15.f + i * (communityCardW + communityGap), 15.f
        };
        auto picture = tgui::Picture::create(tgui::Texture(visible[i].imagePath, {}, {}, true));
        picture->setSize({communityCardW, communityCardH});
        picture->setPosition({targetPosition.x, targetPosition.y + communityCardH * 0.12f});
        picture->getRenderer()->setOpacity(0.f);
        communityPanel->add(picture);
        communityCardReveals.push_back({picture, targetPosition, nextStart, false});
        nextStart += 0.16f;
    }
}

void Game_GUI::animateCommunityCards() {
    constexpr float duration = 0.28f;
    const float now = communityRevealClock.getElapsedTime().asSeconds();

    for (auto& card : communityCardReveals) {
        if (card.finished) {
            continue;
        }

        const float progress = std::clamp((now - card.startTime) / duration, 0.f, 1.f);
        const float eased = progress * (2.f - progress);
        card.picture->setPosition({
            card.targetPosition.x,
            card.targetPosition.y + communityCardH * 0.12f * (1.f - eased)
        });
        card.picture->getRenderer()->setOpacity(eased);
        card.finished = (progress == 1.f);
    }
}


void Game_GUI::disableActions() {
    for (const auto& button : actionButtons) {
        button->setEnabled(false);
    }
}

void Game_GUI::layoutVisibleActionButtons() {
    std::vector<tgui::Button::Ptr> visibleButtons;
    for (const auto& button : actionButtons) {
        if (button && button->isVisible()) {
            visibleButtons.push_back(button);
        }
    }

    if (visibleButtons.empty()) {
        return;
    }

    const float buttonWidth = visibleButtons.front()->getSize().x;
    const float buttonHeight = visibleButtons.front()->getSize().y;
    const float buttonGap = getGuiWidth() * 0.010417f;
    const float rowWidth = visibleButtons.size() * buttonWidth +
                           (visibleButtons.size() - 1) * buttonGap;
    const float startX = (getGuiWidth() - rowWidth) * 0.5f;
    const float controlsY = getGuiHeight() * 0.944444f;

    for (std::size_t i = 0; i < visibleButtons.size(); ++i) {
        visibleButtons[i]->setPosition({startX + i * (buttonWidth + buttonGap), controlsY});
        visibleButtons[i]->setSize({buttonWidth, buttonHeight});
    }
}

void Game_GUI::refreshBlindStatus() {
    if (!engine || !blindStatusLabel) return;
    std::string text = "Blinds: " + std::to_string(engine->getSmallBlind()) + "/" +
                       std::to_string(engine->getBigBlind()) + "$";
    if (engine->hasBlindProgression()) {
        const auto& schedule = engine->getBlindSchedule();
        text = "Level " + std::to_string(schedule.getLevel()) + " | " + text;
        const int remaining = schedule.getHandsUntilIncrease() -
                              (engine->getPhase() == GamePhase::SHOWDOWN ? 1 : 0);
        text += remaining == 0 ? "\nNext hand: " : "\nNext: ";
        text += std::to_string(schedule.getNextSmallBlind()) + "/" +
                std::to_string(schedule.getNextBigBlind()) + "$";
        if (remaining > 0) {
            text += " in " + std::to_string(remaining) + (remaining == 1 ? " hand" : " hands");
        }
    }
    blindStatusLabel->setText(text);
}

void Game_GUI::refreshHud() {
    auto& players = engine->getPlayers();
    if (humanPlayerIndex >= players.size()) return;

    const float guiW = getGuiWidth();
    balanceAmountLabel->setText(std::to_string(players[humanPlayerIndex]->getMoney()) + "$");
    publishCampaignBalance();
    refreshBlindStatus();

    potLabel->setText("Pot: " + std::to_string(engine->getPot()) + "$");

    placeCenteredLabel(actionStatus, guiW / 2.f, getGuiHeight() * 0.074074f, 20.f, guiW - 20.f);
    placeCenteredLabel(winnerReasonLabel, guiW / 2.f,
                       actionStatus->getPosition().y + actionStatus->getSize().y + getGuiHeight() * 0.006f,
                       20.f, guiW - 20.f);
    refreshCommunity();

    for(std::size_t i = 0; i < players.size(); i++){
        if(moneyLabels[i]) moneyLabels[i]->setText(std::to_string(players[i]->getMoney()) + "$");
        if (i < currentBetLabels.size() && currentBetLabels[i]) {
            if (players[i]->isRealPlayer()) {
                currentBetLabels[i]->setText("Current bet: " + std::to_string(engine->getCurrentBet()) + "$");
            } else {
                currentBetLabels[i]->setText(std::to_string(players[i]->getBetThisRound()) + "$");
            }
        }
    }

    if (engine->getPhase() == GamePhase::SHOWDOWN) {
        disableActions();

        if(!restartPending && !seriesFinished){
            restartPending = true;
            restartClock.restart();
        }
    }
}

void Game_GUI::restartGame(){
    if (campaignStage >= 0) return;
    const int playerCount = engine->getNumberOfPlayers();
    const int smallBlind = engine->getSmallBlind();

    gui.removeAllWidgets();
    actionButtons.clear();
    communityPanel = nullptr;
    balanceLabel = nullptr;
    balanceAmountLabel = nullptr;
    potLabel = nullptr;
    winnerReasonLabel = nullptr;
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
    if (seriesFinished) return;
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
    restartGameButton->setEnabled(campaignStage < 0);
    if (campaignStage >= 0) {
        restartGameButton->setText("Campaign: no free rebuy");
        auto notice = makeLabel("Leaving forfeits chips already bet in this hand.", 16, color.accent, escapeMenuPanel);
        notice->setMaximumTextWidth(panelW * 0.94f);
        notice->setPosition({panelW * 0.03f, panelH * 0.12f});
    }
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
        stopSession();
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
            stopSession();
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

    const int playersStillInHand = static_cast<int>(std::count_if(players.begin(), players.end(), [](const std::unique_ptr<Player>& player) {
        return player->isInGame() && !player->getIsEliminated();
    }));
    if (playersStillInHand <= 1) {
        engine->finishHand();
        resolveShowdown(false);
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
        playerTurn = engine->getFirstTurnIndex();
        updateBotWaiting();
        return;
    }

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
            if (animatedTurnPanel) {
                animatedTurnPanel->setVisible(false);
                animatedTurnPanel = nullptr;
                turnPanelMoving = false;
            }

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
            layoutVisibleActionButtons();
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

            const tgui::Vector2f targetPosition = panels[playerTurn]->getPosition();
            const tgui::Vector2f targetSize = panels[playerTurn]->getSize();

            if (!animatedTurnPanel || !animatedTurnPanel->isVisible()) {
                animatedTurnPanel = panels[playerTurn];
                animatedTurnPanelBasePosition = targetPosition;
                animatedTurnPanelBaseSize = targetSize;
                constexpr float startScale = 0.50f;
                const tgui::Vector2f startSize = animatedTurnPanelBaseSize * startScale;
                animatedTurnPanel->setSize(startSize);
                animatedTurnPanel->setPosition({
                    animatedTurnPanelBasePosition.x + (animatedTurnPanelBaseSize.x - startSize.x) * 0.5f,
                    animatedTurnPanelBasePosition.y + (animatedTurnPanelBaseSize.y - startSize.y) * 0.5f
                });
                animatedTurnPanel->setVisible(true);
                turnPanelMoving = false;
            } else {
                turnPanelStartPosition = animatedTurnPanel->getPosition();
                turnPanelStartSize = animatedTurnPanel->getSize();
                turnPanelTargetPosition = targetPosition;
                turnPanelTargetSize = targetSize;
                turnPanelMoving = true;
            }

            if (panels[playerTurn] != animatedTurnPanel) {
                panels[playerTurn]->setVisible(false);
            }
            turnPanelClock.restart();
            playerNameLabels[playerTurn]->setText(players[playerTurn]->getName() + "'s turn");
            const float centerX = seatPairCenterX(seat, guiW);
            const float topY = seat.topY * guiH + seat.cardH * guiH + 10.f;
            placeCenteredLabel(playerNameLabels[playerTurn], centerX, topY, 20.f, guiW - 20.f);
        }
        return;
    }
}

void Game_GUI::animateTurnPanel() {

    if (!animatedTurnPanel || !animatedTurnPanel->isVisible()) {
        return;
    }

    constexpr float duration = 0.4f;
    constexpr float startScale = 0.78f;
    const float progress = std::clamp(turnPanelClock.getElapsedTime().asSeconds() / duration, 0.f, 1.f);
    const float easedProgress = progress * (2.f - progress);

    if (turnPanelMoving) {
        const auto lerp = [easedProgress](float start, float end) {
            return start + (end - start) * easedProgress;
        };

        animatedTurnPanel->setSize({
            lerp(turnPanelStartSize.x, turnPanelTargetSize.x),
            lerp(turnPanelStartSize.y, turnPanelTargetSize.y)
        });
        
        animatedTurnPanel->setPosition({
            lerp(turnPanelStartPosition.x, turnPanelTargetPosition.x),
            lerp(turnPanelStartPosition.y, turnPanelTargetPosition.y)
        });

        if (progress == 1.f) {
            animatedTurnPanel->setSize(turnPanelTargetSize);
            animatedTurnPanel->setPosition(turnPanelTargetPosition);
            animatedTurnPanelBaseSize = turnPanelTargetSize;
            animatedTurnPanelBasePosition = turnPanelTargetPosition;
            turnPanelMoving = false;
        }
        return;
    }

    const float scale = startScale + (1.f - startScale) * easedProgress;
    const tgui::Vector2f size = animatedTurnPanelBaseSize * scale;

    animatedTurnPanel->setSize(size);
    animatedTurnPanel->setPosition({
        animatedTurnPanelBasePosition.x + (animatedTurnPanelBaseSize.x - size.x) * 0.5f,
        animatedTurnPanelBasePosition.y + (animatedTurnPanelBaseSize.y - size.y) * 0.5f
    });

    if (progress == 1.f) {
        animatedTurnPanel->setSize(animatedTurnPanelBaseSize);
        animatedTurnPanel->setPosition(animatedTurnPanelBasePosition);
    }
}

void Game_GUI::game(int playerCount, int smallBlind, int startingMoney, int opponentMoney, int stage) {
    campaignStage = stage;
    seriesFinished = false;
    restartPending = false;
    escapeMenuPanel = nullptr;
    gameSettingsPanel = nullptr;
    audioSettingsPanel = nullptr;
    modalOverlay = nullptr;
    openedSettingsPanel = false;
    gui.removeAllWidgets();
    winnerReasonLabel = nullptr;
    actionButtons.clear();
    panels.clear();
    choiceLabels.clear();
    playerNameLabels.clear();
    moneyLabels.clear();
    currentBetLabels.clear();
    outLogMessages.clear();
    outLogLabels.clear();
    loggedOutPlayers.clear();
    animatedTurnPanel = nullptr;
    botWaiting = false;
    showdownResolved = false;
    tgui::Font::setGlobalFont("assets/fonts/Shelten.ttf");
    if(!ambienceSound.openFromFile("assets/sound/ambienceSound.ogg")){
        std::cerr<< "Ambience sound is not loaded or other big problem. its cooked\n";
    }
    ambienceSound.play();
    ambienceSound.setVolume(10);
    ambienceSound.setLooping(true);

    
    engine = std::make_unique<Game>();
    engine->setSmallBlind(smallBlind);
    engine->setNumberOfPlayers(playerCount);
    engine->setStartingMoney(startingMoney, opponentMoney);
    engine->setBlindProgressionEnabled(campaignStage >= 0);

    engine->game();
    playerTurn = engine->getFirstTurnIndex();
    buildTable();
    publishCampaignBalance();

}

void Game_GUI::nextHand(){
    if (seriesFinished || !engine) return;
    restartPending = false;
    gui.removeAllWidgets();
    winnerReasonLabel = nullptr;
    actionButtons.clear();
    choiceLabels.clear();
    playerNameLabels.clear();
    panels.clear();
    outLogLabels.clear();
    animatedTurnPanel = nullptr;
    botWaiting = false;
    showdownResolved = false;
    timeForBot = 0.f;
    tgui::Font::setGlobalFont("assets/fonts/Shelten.ttf");
    engine->newHand();
    playerTurn = engine->getFirstTurnIndex();
    buildTable();
    publishCampaignBalance();
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

    const float centerX = seatPairCenterX(seat, guiW);
    const float topChoiceY = seat.topY * guiH + seat.cardH * guiH + 35.f;


    if (playerTurn < static_cast<int>(panels.size()) && panels[playerTurn]) {
        playerNameLabels[playerTurn]->setText(players[playerTurn]->getName());

        const float topY = seat.topY * guiH + seat.cardH * guiH + 10.f;

        placeCenteredLabel(playerNameLabels[playerTurn], centerX, topY, 20.f, guiW - 20.f);


        const auto choice = engine->botChoice(playerTurn);
        const std::string choiceLabelText = engine->toStringActionType(choice);

        if (playerTurn < static_cast<int>(choiceLabels.size()) && choiceLabels[playerTurn]) {
            if (choice == ActionType::RAISE) choiceLabels[playerTurn]->setText(choiceLabelText + " " + std::to_string(engine->raiseAmount) + "$");
            else choiceLabels[playerTurn]->setText(choiceLabelText);
            placeCenteredLabel(choiceLabels[playerTurn], centerX, topChoiceY, 20.f, guiW - 20.f);
        }
        if (playerTurn < static_cast<int>(currentBetLabels.size()) && currentBetLabels[playerTurn]) {
            currentBetLabels[playerTurn]->setText(std::to_string(players[playerTurn]->getBetThisRound()) + "$");
        }
    }

    if(playerTurn == static_cast<int>(players.size()) - 1){
        playerTurn = 0;
    }
    else {
        playerTurn++;
    }
    
    updateBotWaiting();
    refreshHud();
}

void Game_GUI::buildTable(){
    communityCardReveals.clear();
    communityRevealClock.restart();

    root = tgui::Panel::create({getGuiWidth(), getGuiHeight()});
    root->getRenderer()->setTextureBackground("assets/images/backgrounds/game_background.png");
    gui.add(root);
    
    auto topBar = makeBar({0,0});
    const float bottomBarH = Constants::Bar::bottomHeightRatio * getGuiHeight();
    auto bottomBar = makeBar({0, getGuiHeight() - bottomBarH});
    bottomBar->setSize({getGuiWidth(), bottomBarH});


    volume(topBar, root, mainMenuMusic);

    if (campaignStage >= 0) {
        auto stageLabel = makeLabel(std::string(campaign::stages[campaignStage].title) + " - " +
                                    campaign::stages[campaignStage].location, 18, color.accent, topBar);
        stageLabel->setPosition({getGuiWidth() * 0.25f, getGuiHeight() * 0.014f});
    }

    const float guiW = getGuiWidth();
    const float guiH = getGuiHeight();
    const float topBarH = Constants::Bar::topHeightRatio * guiW;

    blindStatusLabel = makeLabel("", scaledTextSize(guiH, 0.016f, 12, 18), color.accent, bottomBar);
    blindStatusLabel->setMaximumTextWidth(guiW * 0.28f);
    blindStatusLabel->setPosition({guiW * 0.01f, bottomBarH * 0.08f});
    refreshBlindStatus();

    // Community card panel
    const auto& communitySlot = TableLayout::COMMUNITY;
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
    for (std::size_t i = 0; i < players.size(); ++i) {
        auto& player = players[i];
        if (!player->isRealPlayer() && player->isInGame() && !player->getIsEliminated() &&
            i > 0 && i - 1 < TableLayout::OPPONENT_TURNS.size()) {
            const auto& seat = TableLayout::OPPONENT_TURNS[i - 1];
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

    playerNameLabels.resize(players.size());
    choiceLabels.resize(players.size());
    playerNameLabels[0] = makeLabel("Player", 20, color.text, root);
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

        const float badgeSize = player->isRealPlayer() ? guiW * 0.036458f : guiW * 0.026042f;
        const float badgeStep = badgeSize + guiH * 0.008f;
        int badgeIndex = 0;
        const auto placeBadge = [&](const std::string& title) {
            placeBlind(name, title, {seatX, seatY - badgeIndex * badgeStep});
            ++badgeIndex;
        };

        if (player->getIsSmallBlind()) placeBadge("SMALL\nBLIND");
        if (player->getIsBigBlind()) placeBadge("BIG\nBLIND");
        if (player->getIsDealer()) placeBadge("DEALER");

        if (player->isRealPlayer()) {
            humanPlayerIndex = i;
            const auto& hole = player->getcard2();
            float holeX = seatX;
            for (size_t j = 0; j < hole.size() && j < 2; j++) {
                placeCard(root, hole[j], holeX, seatY, cardW, cardH);
                holeX += cardW + gap;
            }
            
            balanceLabel = makeLabel("Balance:", 20, color.text, topBar);
            balanceLabel->setPosition({guiW * 0.01, topBarH * 0.20f});
            balanceAmountLabel = makeLabel(std::to_string(player->getMoney()) + "$", 20, color.down, topBar);
            balanceAmountLabel->setPosition({balanceLabel->getPosition().x + balanceLabel->getSize().x + 8.f, balanceLabel->getPosition().y});

        } else {
            if (!player->getIsEliminated()){
                placeFaceDownPair(root, seatX, seatY, cardW, cardH, gap);
                playerNameLabels[i] = placePlayerName(root, player->getName(), seat, guiW, guiH);
                choiceLabels[i]= makeLabel("", 18, color.text, root);
            }
        }
    }

    placeOutLabel();

    potLabel = makeLabel("Pot: " + std::to_string(engine->getPot()) + "$", scaledTextSize(guiH, 0.026f, 24, 40), color.accent, root);
    placeCenteredLabel(potLabel, guiW / 2.f, communityY - potLabel->getSize().y - 18.f, 20.f, guiW - 20.f);

    if (humanPlayerIndex < players.size()) {
        const float controlsY = guiH * 0.944444f;
        const float btnW = guiW * 0.064583f;
        const float btnH = guiH * 0.044444f;
        const float btnGap = guiW * 0.010417f;
        const float actionRowW = 5.f * btnW + 4.f * btnGap;
        const float startX = (guiW - actionRowW) / 2.f;

        actionStatus = makeLabel("Choose action", 24, color.text, root);
        actionStatus->setMaximumTextWidth(guiW - 40.f);
        actionStatus->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
        placeCenteredLabel(actionStatus, guiW / 2.f, guiH * 0.074074f, guiW * 0.010417f, guiW * 0.989583f);
        winnerReasonLabel = makeLabel("", scaledTextSize(guiH, 0.019f, 18, 26), color.text, root);
        winnerReasonLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
        winnerReasonLabel->setMaximumTextWidth(guiW * 0.48f);
        winnerReasonLabel->setVisible(false);

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

        auto placeActionButton = [this, btnW, btnH, btnGap](tgui::Button::Ptr button, ActionType action, std::string statusMsg) {
            bindSoundOnClick(button, [this, action, msg = std::move(statusMsg), btnW, btnH, btnGap] {
            auto& players = engine->getPlayers();
                if (action == ActionType::RAISE){
                    const float sliderW = btnW * 2.f + btnGap;
                    const float sliderH = btnH * 0.20f;
                    const float sliderX = raiseButton->getPosition().x + raiseButton->getSize().x + btnGap;
                    const float sliderY = raiseButton->getPosition().y + (btnH - sliderH) * 0.5f;
                    confirmButton->setPosition({sliderX + sliderW + btnGap, raiseButton->getPosition().y});

                    const auto updateRaiseValueLabel = [this](float value) {
                        const float range = raiseSlider->getMaximum() - raiseSlider->getMinimum();
                        const float ratio = range > 0.f
                            ? std::clamp((value - raiseSlider->getMinimum()) / range, 0.f, 1.f)
                            : 0.f;
                        const float thumbX = raiseSlider->getPosition().x + ratio * raiseSlider->getSize().x;
                        raiseMoneyLabel->setText(std::to_string(static_cast<int>(value)) + "$");
                        raiseMoneyLabel->setPosition({
                            thumbX - raiseMoneyLabel->getSize().x * 0.5f,
                            raiseSlider->getPosition().y - raiseMoneyLabel->getSize().y - getGuiHeight() * 0.005f
                        });
                    };

                    if (!raiseSlider){
                        raiseSlider = makeSlider({sliderX, sliderY}, {sliderW, sliderH}, 0, root);

                        const int toCall = std::max(0, engine->getCurrentBet() - players[humanPlayerIndex]->getBetThisRound());
                        raiseSlider->setMaximum(std::max(0, players[humanPlayerIndex]->getMoney() - toCall));
                        raiseMoneyLabel->setVisible(true);
                        updateRaiseValueLabel(raiseSlider->getValue());
                        
                        raiseSlider->onValueChange([this, updateRaiseValueLabel](float value){
                            confirmButton->setVisible(value >= 1.f);
                            play_button();
                            updateRaiseValueLabel(value);
                        });
                    } else {
                        raiseSlider->setPosition({sliderX, sliderY});
                        bool show = !raiseSlider->isVisible();
                        raiseSlider->setVisible(show);
                        raiseMoneyLabel->setVisible(show);
                        confirmButton->setVisible(show && raiseSlider->getValue() >= 1.f);
                        updateRaiseValueLabel(raiseSlider->getValue());
                    }
                    return;
                }

                if (raiseSlider){
                    raiseSlider->setVisible(false);
                    confirmButton->setVisible(false);
                    raiseMoneyLabel->setVisible(false);
                }
                if(players[humanPlayerIndex]->isInGame()){
                    engine->playerAction(action, 0);
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


    actionButtons.push_back(checkButton);
    actionButtons.push_back(callButton);
    actionButtons.push_back(allInButton);
    actionButtons.push_back(foldButton);
    actionButtons.push_back(raiseButton);

    for(auto& button : actionButtons){
        button->setVisible(false);
    }


    placeActionButton(foldButton, ActionType::FOLD,  "Action: Fold");
    placeActionButton(raiseButton,  ActionType::RAISE, "Action: Raise");
    placeActionButton(allInButton,  ActionType::ALLIN, "Action: All In");
    placeActionButton(checkButton,  ActionType::CHECK, "Action: Check");
    placeActionButton(callButton,  ActionType::CALL,  "Action: Call");




    updateBotWaiting();
    refreshCommunity();



    
    }
}


void Game_GUI::resolveShowdown(bool revealCards) {
    if (showdownResolved) {
        return;
    }
    showdownResolved = true;
    botWaiting = false;

    if (revealCards) {
        revealShowdownCards();
    }

    auto& players = engine->getPlayers();

    HandEvaluator handEvaluator;
    std::vector<Card> cards;
    std::vector<HandScore> evaluationCards;
    const auto& communityCards = engine->getCommunityCards();

    for (size_t i = 0; i < players.size(); i++) {
        auto& player = players[i];
        if (!player->isInGame()) continue;
       
        cards = utils::cards7collection(players, i, communityCards);
        evaluationCards.push_back(handEvaluator.evaluate(cards, player->getName()));
    }

    if (evaluationCards.empty()) {
        actionStatus->setText("No winner");
        winnerReasonLabel->setVisible(false);
        return;
    }

    const auto isBetter = [](const HandScore& left, const HandScore& right) {
        return left.rank != right.rank ? left.rank < right.rank
                                       : left.sum_for_tie < right.sum_for_tie;
    };
    std::map<std::string, HandScore> scores;
    for (const HandScore& score : evaluationCards) {
        scores.emplace(score.name, score);
    }

    std::vector<int> contributionLevels;
    for (const auto& player : players) {
        if (player->getCommittedThisHand() > 0) {
            contributionLevels.push_back(player->getCommittedThisHand());
        }
    }
    std::sort(contributionLevels.begin(), contributionLevels.end());
    contributionLevels.erase(std::unique(contributionLevels.begin(), contributionLevels.end()), contributionLevels.end());

    std::map<std::string, int> payouts;
    int previousLevel = 0;
    for (const int level : contributionLevels) {
        std::vector<std::string> eligibleNames;
        int contributorCount = 0;

        for (const auto& player : players) {
            if (player->getCommittedThisHand() < level) {
                continue;
            }
            ++contributorCount;
            if (player->isInGame() && scores.find(player->getName()) != scores.end()) {
                eligibleNames.push_back(player->getName());
            }
        }

        const int sidePot = (level - previousLevel) * contributorCount;
        previousLevel = level;
        if (sidePot == 0 || eligibleNames.empty()) {
            continue;
        }

        HandScore best = scores.at(eligibleNames.front());
        std::vector<std::string> winners{eligibleNames.front()};
        for (std::size_t i = 1; i < eligibleNames.size(); ++i) {
            const HandScore& candidate = scores.at(eligibleNames[i]);
            if (isBetter(best, candidate)) {
                best = candidate;
                winners = {eligibleNames[i]};
            } else if (!isBetter(candidate, best) && !isBetter(best, candidate)) {
                winners.push_back(eligibleNames[i]);
            }
        }

        const int share = sidePot / static_cast<int>(winners.size());
        int remainder = sidePot % static_cast<int>(winners.size());
        for (const std::string& winnerName : winners) {
            payouts[winnerName] += share;
            if (remainder > 0) {
                ++payouts[winnerName];
                --remainder;
            }
        }
    }

    std::vector<std::string> winnerNames;
    for (auto& player : players) {
        const auto payout = payouts.find(player->getName());
        if (payout == payouts.end() || payout->second == 0) {
            continue;
        }
        player->getMoney() += payout->second;
        winnerNames.push_back(player->getName());
    }

    if (winnerNames.empty()) {
        actionStatus->setText("No winner");
        winnerReasonLabel->setVisible(false);
    } else if (winnerNames.size() == 1) {
        actionStatus->setText("Winner: " + winnerNames.front());
        winnerReasonLabel->setText(revealCards ? handCategoryName(scores.at(winnerNames.front()).rank)
                                                : "All other players folded");
        winnerReasonLabel->setVisible(true);
    } else {
        std::string names;
        std::string reasons;
        for (std::size_t i = 0; i < winnerNames.size(); ++i) {
            if (i > 0) {
                names += ", ";
                reasons += "\n";
            }
            names += winnerNames[i];
            reasons += winnerNames[i] + ": " + handCategoryName(scores.at(winnerNames[i]).rank);
        }
        actionStatus->setText("Winners: " + names);
        winnerReasonLabel->setText(reasons);
        winnerReasonLabel->setVisible(true);
    }
    actionStatus->setTextSize(30);
    placeCenteredLabel(actionStatus, getGuiWidth() / 2.f, getGuiHeight() * 0.074074f,20.f, getGuiWidth() - 20.f);
    placeCenteredLabel(winnerReasonLabel, getGuiWidth() / 2.f, actionStatus->getPosition().y + actionStatus->getSize().y + getGuiHeight() * 0.006f, 20.f, getGuiWidth() - 20.f);
    placeOutLabel();
    publishCampaignBalance();
    refreshBlindStatus();
    std::vector<int> stacks;
    for (const auto& player : players) stacks.push_back(player->getMoney());
    const auto result = campaign::seriesResult(stacks, humanPlayerIndex);
    if (result != campaign::SeriesResult::InProgress) finishSeries(result);
}

void Game_GUI::publishCampaignBalance() {
    if (campaignStage < 0 || !engine || !onCampaignUpdate) return;
    for (const auto& player : engine->getPlayers()) {
        if (player->isRealPlayer()) {
            onCampaignUpdate(player->getMoney(), false);
            return;
        }
    }
}

void Game_GUI::stopSession() {
    botWaiting = false;
    restartPending = false;
    ambienceSound.stop();
    publishCampaignBalance();
}

void Game_GUI::finishSeries(campaign::SeriesResult result) {
    if (seriesFinished) return;
    seriesFinished = true;
    botWaiting = false;
    restartPending = false;
    disableActions();
    if (animatedTurnPanel) animatedTurnPanel->setVisible(false);
    const bool won = result == campaign::SeriesResult::Won;
    if (campaignStage >= 0 && onCampaignUpdate) {
        onCampaignUpdate(engine->getPlayers()[humanPlayerIndex]->getMoney(), won);
    }

    const float width = getGuiWidth() * 0.48f;
    const float height = getGuiHeight() * 0.33f;
    showOverlay();
    // Put the result above any pause/settings panels that were open.
    if (escapeMenuPanel) escapeMenuPanel->setVisible(false);
    if (gameSettingsPanel) gameSettingsPanel->setVisible(false);
    if (audioSettingsPanel) audioSettingsPanel->setVisible(false);
    auto panel = makePanel({width, height}, color.centerpanel, root);
    auto title = makeLabel(won ? "Series won!" : "Series lost", 32, color.accent, panel);
    title->setPosition({width * 0.06f, height * 0.08f});
    std::string message = won ? "You eliminated every opponent." : "Your bankroll is empty.";
    if (campaignStage >= 0 && won) {
        message += campaignStage + 1 < static_cast<int>(campaign::stages.size())
            ? "\nNext stop: " + std::string(campaign::stages[campaignStage + 1].title)
            : "\nYou are the WSOP champion in Las Vegas!";
    }
    message += "\nBalance: " + std::to_string(engine->getPlayers()[humanPlayerIndex]->getMoney()) + "$";
    auto detail = makeLabel(message, 22, color.text, panel);
    detail->setMaximumTextWidth(width * 0.88f);
    detail->setPosition({width * 0.06f, height * 0.30f});
    auto back = makeButton(campaignStage >= 0 ? "Back to Campaign" : "Back to Menu",
                           {width * 0.06f, height * 0.76f}, {width * 0.88f, height * 0.17f}, 24, panel);
    bindSoundOnClick(back, [this] {
        stopSession();
        if (onExit) onExit();
    });
}

void Game_GUI::placeOutLabel() {
    const auto& players = engine->getPlayers();
    loggedOutPlayers.resize(players.size(), false);

    for (std::size_t i = 0; i < players.size(); ++i) {
        const auto& player = players[i];
        const bool out = player->getIsEliminated() || (engine->getPhase() == GamePhase::SHOWDOWN && player->getMoney() <= 0);
        if (player->isBot() && out && !loggedOutPlayers[i]) {
            outLogMessages.push_back(player->getName() + " is out");
            loggedOutPlayers[i] = true;
        }
    }

    const float guiW = getGuiWidth();
    const float guiH = getGuiHeight();
    const int fontSize = scaledTextSize(guiH, 0.013f, 12, 18);
    const float firstY = Constants::Bar::topHeightRatio * guiH + guiH * 0.005f;
    const float lineHeight = fontSize * 1.4f;

    for (std::size_t i = outLogLabels.size(); i < outLogMessages.size(); ++i) {
        auto label = makeLabel(outLogMessages[i], fontSize, color.text, root);
        label->setPosition({guiW - guiW * 0.01f - label->getSize().x, firstY + i * lineHeight});
        outLogLabels.push_back(label);
    }
}
