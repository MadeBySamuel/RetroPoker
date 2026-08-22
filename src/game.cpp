#include <iostream>
#include <memory>
#include <random>

#include "game.hpp"
#include "utils.hpp"
#include "gui_utils.hpp"
#include "hand_evaluator.hpp"
#include "constants.hpp"
#include "player.hpp"


int& Game::getPot(){
    return pot;
}

void Game::setPot(int amount){
    pot += amount;
}

GamePhase Game::getPhase() const {
    return currentPhase;
}

int Game::getBigBlind(){
    return bigBlind;
}

void Game::setBigBlind(int bigBlind){
    this->bigBlind = bigBlind;
}

void Game::setSmallBlind(int smallBlind){
    this->smallBlind = smallBlind;
}

int Game::getSmallBlind(){
    return smallBlind;
}

int Game::getCurrentBet(){
    return currentBet;
}

void Game::setCurrentBet(int currentBet){
    this->currentBet = currentBet;
}

std::vector<std::unique_ptr<Player>>& Game::getPlayers(){
    return players;
}

void Game::setup_once(std::vector<std::unique_ptr<Player>>& players, std::vector<std::string>& game_names){
    utils::shuffle_players(game_names);
    utils::set_player_position(game_names);
    utils::createPlayers(game_names, players);
    smallBlindPosition = 1;
    bigBlindPosition = 2;
    setSmallBlind(10);
    setBigBlind(20);
}

void Game::setup_once_per_game(std::vector<std::unique_ptr<Player>>& players, std::vector<Card>& game_cards,
    std::vector<int>& used_cards_index, std::vector<Card>& community_cards){

    used_cards_index.clear();

    utils::shuffle_cards(game_cards);
    utils::card_selection(players, used_cards_index, game_cards);
    utils::community_cards_selection(game_cards, used_cards_index, community_cards);
    utils::blindSelection(players, bigBlindPosition, smallBlindPosition);
    utils::settingInGameStatus(players);

    for (size_t i = 0; i < players.size(); i++) {
        if (players[i]->getIsSmallBlind()) {
            players[i]->getMoney() -= getSmallBlind();
            setPot(getSmallBlind());
            break;
        }
    }

    for (size_t i = 0; i < players.size(); i++) {
        if (players[i]->getIsBigBlind()) {
            players[i]->getMoney() -= getBigBlind();
            setPot(getBigBlind());
            break;
        }
    }
}


const std::vector<Card>& Game::getCommunityCards() const {
    return communityCards;
}

std::vector<Card> Game::getVisibleCommunityCards() const {
    switch (currentPhase) {
        case GamePhase::PRE_FLOP: return {};
        case GamePhase::FLOP:     return {communityCards.begin(), communityCards.begin() + 3};
        case GamePhase::TURN:     return {communityCards.begin(), communityCards.begin() + 4};
        default:                  return communityCards;
    }
}

const std::string& Game::getWinnerMessage() const {
    return winnerMessage;
}

void Game::newHand() {
    pot = 0;
    currentBet = 0;
    currentPhase = GamePhase::PRE_FLOP;
    winnerMessage.clear();
    communityCards.clear();

    std::vector<Card> game_cards = gui_utils::game_cards();
    std::vector<int> used_cards_index;

    const int playerCount = static_cast<int>(players.size());
    
    smallBlindPosition++;
    bigBlindPosition++;

    smallBlindPosition = smallBlindPosition == playerCount ? 0 : smallBlindPosition;
    bigBlindPosition = bigBlindPosition == playerCount ? 0 : bigBlindPosition;


    setup_once_per_game(players, game_cards, used_cards_index, communityCards);
}

void Game::game() {
    pot = 0;
    currentBet = 0;
    currentPhase = GamePhase::PRE_FLOP;
    winnerMessage.clear();
    communityCards.clear();
    players.clear();

    std::vector<Card> game_cards = gui_utils::game_cards();
    std::vector<std::string> game_names = Constants::NAMES;
    std::vector<int> used_cards_index;

    Game::setup_once(this->players, game_names);
    
    Game::setup_once_per_game(this->players, game_cards, used_cards_index, communityCards);
}

void Game::playerAction(ActionType action, int raiseAmount) {
    int humanIdx = -1;
    for (size_t i = 0; i < players.size(); i++) {
        if (players[i]->getName() == "Player") { 
            humanIdx = static_cast<int>(i); 
            break; 
        }
    }

    if (humanIdx != -1 && players[humanIdx]->getInGame()) {
        switch (action) {
            case ActionType::FOLD:
                players[humanIdx]->setInGame(false);
                break;
            case ActionType::CHECK:
                break;
            case ActionType::CALL: {
                int callAmt = std::min(std::max(currentBet, bigBlind), players[humanIdx]->getMoney());
                players[humanIdx]->getMoney() -= callAmt;
                pot += callAmt;
                break;
            }
            case ActionType::RAISE: {
                currentBet = std::max(currentBet, bigBlind) + raiseAmount;
                int paid = std::min(raiseAmount, players[humanIdx]->getMoney());
                players[humanIdx]->getMoney() -= paid;
                pot += paid;
                break;
            }
            case ActionType::ALLIN: {
                int all = players[humanIdx]->getMoney();
                players[humanIdx]->getMoney() = 0;
                pot += all;
                break;
            }
        }
    }

    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, 4);

    for (size_t i = 0; i < players.size(); i++) {
        if (players[i]->getName() == "Player" || !players[i]->getInGame()){
            continue;
        }

        int r = dist(rng);
        
        if (r == 0) {
            players[i]->setInGame(false);
        } 
        else if (r <= 2) {
            // check
        } 
        else {
            int callAmt = std::min(std::max(currentBet, bigBlind), players[i]->getMoney());
            players[i]->getMoney() -= callAmt;
            pot += callAmt;
        }

    }

    switch (currentPhase) {
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
        default: break;
    }

}
