#include <iostream>
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
    this->smallBlindAmount = smallBlind;
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
    this->numberOfPlayers = numberOfPlayers;
}

void Game::setCurrentBet(int currentBet)
{
    this->currentBet = currentBet;
}

std::vector<std::unique_ptr<Player>> &Game::getPlayers()
{
    return players;
}

void Game::setup_once(std::vector<std::unique_ptr<Player>> &players)
{
    utils::createPlayers(players, numberOfPlayers);
    dealerPosition = 0;
    smallBlindPosition = 1;
    bigBlindPosition = 2;

    setSmallBlind(smallBlindAmount);
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
}

const std::vector<Card> &Game::getCommunityCards() const
{
    return communityCards;
}

std::vector<Card> Game::getVisibleCommunityCards() const
{
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
    pot = 0;
    currentBet = 0;
    raiseAmount = 0;
    currentPhase = GamePhase::PRE_FLOP;
    winnerMessage.clear();
    communityCards.clear();

    for (auto& player : players) {
        player->setBetThisRound(0);
        player->setActed(false);
        player->setAllIn(false);
    }

    std::vector<Card> game_cards = gui_utils::game_cards();
    std::vector<int> used_cards_index;

    const int playerCount = static_cast<int>(players.size());

    smallBlindPosition++;
    bigBlindPosition++;
    dealerPosition++;

    smallBlindPosition = smallBlindPosition == playerCount ? 0 : smallBlindPosition;
    bigBlindPosition = bigBlindPosition == playerCount ? 0 : bigBlindPosition;
    dealerPosition = dealerPosition == playerCount ? 0 : dealerPosition;


    setup_once_per_game(players, game_cards, used_cards_index, communityCards);
}

void Game::game()
{
    pot = 0;
    currentBet = 0;
    raiseAmount = 0;
    currentPhase = GamePhase::PRE_FLOP;
    winnerMessage.clear();
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
    if (toCall == 0) {
        possibleActions.push_back(ActionType::CHECK);
    } else {
        possibleActions.push_back(ActionType::CALL);
    }
    if (money > toCall) {
        possibleActions.push_back(ActionType::RAISE);
    }

    std::uniform_int_distribution<std::size_t> actionDist(0, possibleActions.size() - 1);
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
        return ActionType::CHECK;
    }

    if (action == ActionType::CALL) {
        const int paid = std::min(toCall, money);
        player->getMoney() -= paid;
        player->getBetThisRound() += paid;
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
