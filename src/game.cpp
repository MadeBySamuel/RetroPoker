#include <iostream>
#include <algorithm>
#include <memory>
#include <random>

#include "game.hpp"
#include "utils.hpp"
#include "gui_utils.hpp"
#include "hand_evaluator.hpp"
#include "constants.hpp"
#include "player.hpp"

int &Game::getPot()
{
    return pot;
}

void Game::setPot(int amount)
{
    pot += amount;
}

GamePhase Game::getPhase() const
{
    return currentPhase;
}

int Game::getBigBlind()
{
    return bigBlindAmount;
}

void Game::setBigBlind(int bigBlind)
{
    this->bigBlindAmount = bigBlind;
}

void Game::setSmallBlind(int smallBlind)
{
    blindSchedule = BlindSchedule(smallBlind);
    smallBlindAmount = blindSchedule.getSmallBlind();
    bigBlindAmount = blindSchedule.getBigBlind();
}

void Game::setBlindProgressionEnabled(bool enabled)
{
    blindProgressionEnabled = enabled;
}

bool Game::hasBlindProgression() const
{
    return blindProgressionEnabled;
}

const BlindSchedule& Game::getBlindSchedule() const
{
    return blindSchedule;
}

int Game::getSmallBlind()
{
    return smallBlindAmount;
}

int Game::getCurrentBet()
{
    return currentBet;
}

int Game::getNumberOfPlayers()
{
    return numberOfPlayers;
}

void Game::setNumberOfPlayers(int numberOfPlayers)
{
    this->numberOfPlayers = std::clamp(numberOfPlayers, 2, static_cast<int>(Constants::NAMES.size()));
}

void Game::setCurrentBet(int currentBet)
{
    this->currentBet = currentBet;
}

void Game::setStartingMoney(int humanMoney, int botMoney)
{
    startingMoney = std::max(1, humanMoney);
    opponentMoney = std::max(1, botMoney);
}

std::vector<std::unique_ptr<Player>> &Game::getPlayers()
{
    return players;
}

void Game::setup_once(std::vector<std::unique_ptr<Player>> &players)
{
    utils::createPlayers(players, numberOfPlayers);
    for (auto& player : players) {
        player->setMoney(player->isRealPlayer() ? startingMoney : opponentMoney);
    }
    dealerPosition = 0;
    smallBlindPosition = 1 % static_cast<int>(players.size());
    bigBlindPosition = (smallBlindPosition + 1) % static_cast<int>(players.size());

    setBigBlind(smallBlindAmount * 2);
}

void Game::setup_once_per_game(std::vector<std::unique_ptr<Player>> &players, std::vector<Card> &game_cards,
                               std::vector<int> &used_cards_index, std::vector<Card> &community_cards)
{
    used_cards_index.clear();
    utils::settingInGameStatus(players);
    utils::shuffle_cards(game_cards);
    utils::card_selection(players, used_cards_index, game_cards);
    utils::community_cards_selection(game_cards, used_cards_index, community_cards);
    utils::dealerSelection(players, dealerPosition);
    utils::blindSelection(players, bigBlindPosition, smallBlindPosition);
    if (smallBlindPosition >= 0) {
        postBlind(*players[smallBlindPosition], smallBlindAmount);
    }
    if (bigBlindPosition >= 0) {
        postBlind(*players[bigBlindPosition], bigBlindAmount);
    }
}

void Game::postBlind(Player& player, int amount)
{
    if (player.getIsEliminated() || amount <= 0) {
        return;
    }

    const int paid = std::min(amount, player.getMoney());
    player.getMoney() -= paid;
    player.getBetThisRound() += paid;
    player.addCommittedThisHand(paid);
    pot += paid;
    currentBet = std::max(currentBet, player.getBetThisRound());

    if (player.getMoney() == 0) {
        player.setAllIn(true);
        player.setActed(true);
    }
}

int Game::nextEligiblePlayerIndex(int startIndex) const
{
    if (players.empty()) {
        return 0;
    }

    const int playerCount = static_cast<int>(players.size());
    const int normalizedStart = ((startIndex % playerCount) + playerCount) % playerCount;
    for (int offset = 1; offset <= playerCount; ++offset) {
        const int index = (normalizedStart + offset) % playerCount;
        const auto& player = players[index];
        if (player->isInGame() && !player->getIsEliminated() && !player->getAllIn()) {
            return index;
        }
    }
    return 0;
}

int Game::getFirstTurnIndex() const
{
    const int anchor = currentPhase == GamePhase::PRE_FLOP ? bigBlindPosition : dealerPosition;
    return nextEligiblePlayerIndex(anchor);
}

const std::vector<Card> &Game::getCommunityCards() const
{
    return communityCards;
}


std::vector<Card> Game::getVisibleCommunityCards() const
{
    if (handWonByFold) {
        return {};
    }
    switch (currentPhase)
    {
    case GamePhase::PRE_FLOP:
        return {};
    case GamePhase::FLOP:
        return {communityCards.begin(), communityCards.begin() + 3};
    case GamePhase::TURN:
        return {communityCards.begin(), communityCards.begin() + 4};
    default:
        return communityCards;
    }
}

const std::string &Game::getWinnerMessage() const
{
    return winnerMessage;
}

const std::string Game::toStringActionType(ActionType type)
{

    switch (type)
    {
    case ActionType::CHECK:
        return "Check";
    case ActionType::FOLD:
        return "Fold";
    case ActionType::CALL:
        return "Call";
    case ActionType::ALLIN:
        return "All in";
    case ActionType::RAISE:
        return "Raise";
    }

    return "what";
}

void Game::newHand()
{
    if (players.empty() || currentPhase != GamePhase::SHOWDOWN) return;
    if (blindProgressionEnabled) {
        blindSchedule.advanceHand();
        smallBlindAmount = blindSchedule.getSmallBlind();
        bigBlindAmount = blindSchedule.getBigBlind();
    }

    pot = 0;
    currentBet = 0;
    raiseAmount = 0;
    currentPhase = GamePhase::PRE_FLOP;
    winnerMessage.clear();
    handWonByFold = false;
    communityCards.clear();

    for (auto& player : players) {
        player->setBetThisRound(0);
        player->resetCommittedThisHand();
        player->setActed(false);
        player->setAllIn(false);
    }

    std::vector<Card> game_cards = gui_utils::game_cards();
    std::vector<int> used_cards_index;

    const int playerCount = static_cast<int>(players.size());

    if (playerCount == 0) {
        return;
    }

    smallBlindPosition = (smallBlindPosition + 1) % playerCount;
    bigBlindPosition = (bigBlindPosition + 1) % playerCount;
    dealerPosition = (dealerPosition + 1) % playerCount;


    setup_once_per_game(players, game_cards, used_cards_index, communityCards);
}

void Game::game()
{
    blindSchedule.reset();
    smallBlindAmount = blindSchedule.getSmallBlind();
    bigBlindAmount = blindSchedule.getBigBlind();
    pot = 0;
    currentBet = 0;
    raiseAmount = 0;
    currentPhase = GamePhase::PRE_FLOP;
    winnerMessage.clear();
    handWonByFold = false;
    communityCards.clear();
    players.clear();

    std::vector<Card> game_cards = gui_utils::game_cards();
    std::vector<int> used_cards_index;

    Game::setup_once(players);

    Game::setup_once_per_game(players, game_cards, used_cards_index, communityCards);
}

ActionType Game::botChoice(std::size_t playerTurn)
{
    static std::mt19937 choice(std::random_device{}());

    if (playerTurn >= players.size()) {
        return ActionType::CHECK;
    }

    auto& player = players[playerTurn];
    if (player->isRealPlayer() || !player->isInGame() ||
        player->getIsEliminated() || player->getAllIn()) {
        return ActionType::CHECK;
    }

    const int toCall = std::max(0, currentBet - player->getBetThisRound());
    const int money = player->getMoney();

    std::vector<ActionType> possibleActions{ActionType::FOLD, ActionType::ALLIN};
    std::vector<int> weights;

    if(currentPhase == GamePhase::PRE_FLOP){
        weights.push_back(10);
    }
    else {
        weights.push_back(40);
    }
    
    weights.push_back(1);

    if (toCall == 0) {
        possibleActions.push_back(ActionType::CHECK);
        weights.push_back(40);
    } else {
        possibleActions.push_back(ActionType::CALL);
        weights.push_back(30);
    }
    if (money > toCall) {
        possibleActions.push_back(ActionType::RAISE);
        weights.push_back(2);
    }

    std::discrete_distribution<std::size_t> actionDist(weights.begin(), weights.end());

    const ActionType action = possibleActions[actionDist(choice)];
    player->setActed(true);

    if (action == ActionType::FOLD) {
        player->setInGame(false);
        return ActionType::FOLD;
    }

    if (action == ActionType::CHECK) {
        return ActionType::CHECK;
    }

    if (action == ActionType::CALL) {
        const int paid = std::min(toCall, money);
        player->getMoney() -= paid;
        player->getBetThisRound() += paid;
        player->addCommittedThisHand(paid);
        pot += paid;
        if (player->getMoney() == 0) {
            player->setAllIn(true);
        }
        return ActionType::CALL;
    }

    if (action == ActionType::ALLIN) {
        const int paid = money;
        const int totalBet = player->getBetThisRound() + paid;
        const bool raisesBet = totalBet > currentBet;

        player->setMoney(0);
        player->setBetThisRound(totalBet);
        player->addCommittedThisHand(paid);
        player->setAllIn(true);
        pot += paid;
        currentBet = std::max(currentBet, totalBet);

        if (raisesBet) {
            for (std::size_t i = 0; i < players.size(); ++i) {
                if (i != playerTurn && players[i]->isInGame() && !players[i]->getAllIn()) {
                    players[i]->setActed(false);
                }
            }
        }
        return ActionType::ALLIN;
    }

    // A raise pays the outstanding call first, then adds at least one more chip.
    std::uniform_int_distribution<int> raiseDist(1, money - toCall);
    const int raiseBy = raiseDist(choice);
    const int paid = toCall + raiseBy;
    const int totalBet = player->getBetThisRound() + paid;

    raiseAmount = totalBet;
    player->getMoney() -= paid;
    player->getBetThisRound() = totalBet;
    player->addCommittedThisHand(paid);
    pot += paid;
    currentBet = totalBet;

    for (std::size_t i = 0; i < players.size(); ++i) {
        if (i != playerTurn && players[i]->isInGame() && !players[i]->getAllIn()) {
            players[i]->setActed(false);
        }
    }
    return ActionType::RAISE;
}

ActionType Game::playerAction(ActionType action, int raiseAmount)
{
    int humanIdx = -1;
    for (size_t i = 0; i < players.size(); i++)
    {
        if (players[i]->isRealPlayer())
        {
            humanIdx = static_cast<int>(i);
            break;
        }
    }
    if (humanIdx == -1 || !players[humanIdx]->isInGame() || players[humanIdx]->getAllIn()) {
        return ActionType::CHECK;
    }

    auto& player = players[humanIdx];
    const int toCall = std::max(0, currentBet - player->getBetThisRound());
    const int money = player->getMoney();
    player->setActed(true);

    if (action == ActionType::FOLD) {
        player->setInGame(false);
        return ActionType::FOLD;
    }

    if (action == ActionType::CHECK) {
        if (toCall != 0) {
            player->setActed(false);
        }
        return ActionType::CHECK;
    }

    if (action == ActionType::CALL) {
        const int paid = std::min(toCall, money);
        player->getMoney() -= paid;
        player->getBetThisRound() += paid;
        player->addCommittedThisHand(paid);
        pot += paid;
        if (player->getMoney() == 0) {
            player->setAllIn(true);
        }
        return ActionType::CALL;
    }

    if (action == ActionType::ALLIN) {
        const int paid = money;
        const int totalBet = player->getBetThisRound() + paid;
        const bool raisesBet = totalBet > currentBet;

        player->setMoney(0);
        player->setBetThisRound(totalBet);
        player->addCommittedThisHand(paid);
        player->setAllIn(true);
        pot += paid;
        currentBet = std::max(currentBet, totalBet);

        if (raisesBet) {
            for (std::size_t i = 0; i < players.size(); ++i) {
                if (i != static_cast<std::size_t>(humanIdx) && players[i]->isInGame() && !players[i]->getAllIn()) {
                    players[i]->setActed(false);
                }
            }
        }
        return ActionType::ALLIN;
    }

    const int raiseBy = std::max(0, raiseAmount);
    if (raiseBy == 0 || money <= toCall) {
        const int paid = std::min(toCall, money);
        player->getMoney() -= paid;
        player->getBetThisRound() += paid;
        player->addCommittedThisHand(paid);
        pot += paid;
        if (player->getMoney() == 0) {
            player->setAllIn(true);
        }
        return ActionType::CALL;
    }

    const int paid = std::min(toCall + raiseBy, money);
    const int totalBet = player->getBetThisRound() + paid;
    if (totalBet <= currentBet) {
        return ActionType::CALL;
    }

    this->raiseAmount = totalBet;
    player->getMoney() -= paid;
    player->getBetThisRound() = totalBet;
    player->addCommittedThisHand(paid);
    pot += paid;
    currentBet = totalBet;
    if (player->getMoney() == 0) {
        player->setAllIn(true);
    }

    for (std::size_t i = 0; i < players.size(); ++i) {
        if (i != static_cast<std::size_t>(humanIdx) && players[i]->isInGame() && !players[i]->getAllIn()) {
            players[i]->setActed(false);
        }
    }
    return ActionType::RAISE;
}
void Game::advancePhase()
{
    for(auto& player : players){
        player->setBetThisRound(0);
        if (player->isInGame() && !player->getIsEliminated() && !player->getAllIn()) {
            player->setActed(false);
        }
    }
    currentBet = 0;
    switch (currentPhase)
    {
    case GamePhase::PRE_FLOP:
        currentPhase = GamePhase::FLOP;
        break;
    case GamePhase::FLOP:
        currentPhase = GamePhase::TURN;
        break;
    case GamePhase::TURN:
        currentPhase = GamePhase::RIVER;
        break;
    case GamePhase::RIVER:
        currentPhase = GamePhase::SHOWDOWN;
        break;
    default:
        break;
    }
}

void Game::finishHand()
{
    handWonByFold = true;
    currentPhase = GamePhase::SHOWDOWN;
}
