#include <iostream> 
#include <memory>
#include <array>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

#include "gui_game.hpp"
#include "constants.hpp"

namespace {

struct SeatSlot {
    float leftX;
    float topY;
    float cardW;
    float cardH;
    float gap;
};

constexpr std::array<SeatSlot, 6> PLAYER_SEATS = {{
    {0.4599f, 0.8168f, 0.0438f, 0.1041f, 0.0039f}, // bottom center
    {0.2600f, 0.8164f, 0.0438f, 0.1041f, 0.0032f}, // bottom left
    {0.7076f, 0.8168f, 0.0438f, 0.1041f, 0.0043f}, // bottom right
    {0.6232f, 0.1007f, 0.0438f, 0.1041f, 0.0040f}, // top right
    {0.2895f, 0.1007f, 0.0438f, 0.1041f, 0.0040f}, // top left
    {0.0852f, 0.1644f, 0.0438f, 0.1041f, 0.0040f}, // left
}};

constexpr SeatSlot COMMUNITY_SLOT = {0.3550f, 0.4320f, 0.0532f, 0.1359f, 0.0059f};

constexpr const char* CARD_BACK_PATH = "assets/cards/turned_down_card.png";

} // namespace

Game_GUI::Game_GUI(tgui::Gui& gui, sf::RenderWindow& window, sf::Music& music)
    : Gui_base(gui), window(window), music(music) {}

void Game_GUI::placeCard(tgui::Panel::Ptr parent, const Card& card, float x, float y, float w, float h) {
    if (!card.image) {
        return;
    }
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

void Game_GUI::placePlayerName(tgui::Panel::Ptr parent, const std::string& name, float leftX, float topY, float cardH, float guiW, float guiH, const GuiTheme& color) {
    (void)guiW;
    auto label = tgui::Label::create(name);
    label->setPosition({leftX, topY + cardH * guiH + 6.f});
    label->setTextSize(18);
    label->getRenderer()->setTextColor(color.text);
    parent->add(label);
}

void Game_GUI::game(){
    GuiTheme color;

    gui.removeAllWidgets();

    engine = std::make_unique<Game>();
    engine->game();

    auto root = tgui::Panel::create({getGuiWidth(), getGuiHeight()});
    root->getRenderer()->setTextureBackground("assets/images/backgrounds/game_background.jpeg");
    gui.add(root);

    auto topBar = bar(root);
    volume(topBar, root, color, music);

    const float centerX = getGuiWidth() / 2.f;
    const float guiW = getGuiWidth();
    const float guiH = getGuiHeight();

    const auto& community = engine->getCommunityCards();
    float communityX = COMMUNITY_SLOT.leftX * guiW;
    const float communityY = COMMUNITY_SLOT.topY * guiH;
    const float communityCardW = COMMUNITY_SLOT.cardW * guiW;
    const float communityCardH = COMMUNITY_SLOT.cardH * guiH;
    const float communityGap = COMMUNITY_SLOT.gap * guiW;

    for (size_t i = 0; i < community.size() && i < 5; i++) {
        placeCard(root, community[i], communityX, communityY, communityCardW, communityCardH);
        communityX += communityCardW + communityGap;
    }

    const auto& players = engine->getPlayers();
    std::size_t opponentSeat = 1;

    for (const auto& player : players) {
        const bool isHuman = player->getName() == "Player";
        const auto& seat = PLAYER_SEATS[isHuman ? 0 : opponentSeat++];

        const float seatX = seat.leftX * guiW;
        const float seatY = seat.topY * guiH;
        const float cardW = seat.cardW * guiW;
        const float cardH = seat.cardH * guiH;
        const float gap = seat.gap * guiW;

        if (isHuman) {
            const auto& hole = player->getcard2();
            float holeX = seatX;
            for (size_t j = 0; j < hole.size() && j < 2; j++) {
                placeCard(root, hole[j], holeX, seatY, cardW, cardH);
                holeX += cardW + gap;
            }

            auto balanceLabel = tgui::Label::create("Balance: " + std::to_string(player->getMoney()) + "$");
            balanceLabel->setPosition({seatX, seatY - 28.f});
            balanceLabel->setTextSize(20);
            balanceLabel->getRenderer()->setTextColor(color.text);
            root->add(balanceLabel);

            placePlayerName(root, "You", seatX, seat.topY, seat.cardH, guiW, guiH, color);
        } else {
            placeFaceDownPair(root, seatX, seatY, cardW, cardH, gap);
            placePlayerName(root, player->getName(), seatX, seat.topY, seat.cardH, guiW, guiH, color);
        }
    }

    auto potLabel = tgui::Label::create("Pot: " + std::to_string(engine->getPot()) + "$");
    potLabel->setPosition({centerX - 80.f, communityY - 36.f});
    potLabel->setTextSize(28);
    potLabel->getRenderer()->setTextColor(color.accent);
    root->add(potLabel);

    auto winnerLabel = tgui::Label::create(engine->getWinnerMessage());
    winnerLabel->setPosition({centerX - 200.f, communityY + communityCardH + 20.f});
    winnerLabel->setTextSize(26);
    winnerLabel->getRenderer()->setTextColor(color.text);
    root->add(winnerLabel);

    auto backButton = tgui::Button::create("Back to Menu");
    backButton->setPosition({30.f, getGuiHeight() - 70.f});
    backButton->setSize({220.f, 48.f});
    backButton->setTextSize(22);
    backButton->getRenderer()->setTextColor(color.text);
    backButton->getRenderer()->setBackgroundColor(color.panel2);
    backButton->getRenderer()->setBackgroundColorHover(color.border);
    root->add(backButton);

    bindSoundOnClick(backButton, [this]{
        if (onExit) {
            onExit();
        }
    });
}
