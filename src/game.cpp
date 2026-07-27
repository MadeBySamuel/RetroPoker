#include <iostream>
#include <memory>
#include <random>

#include "game.hpp"
#include "utils.hpp"
#include "gui_utils.hpp"
#include "hand_evaluator.hpp"
#include "constants.hpp"
#include "player.hpp"
#include "gui_game.hpp"


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
}

void Game::setup_once_per_game(std::vector<std::unique_ptr<Player>>& players, std::vector<Card>& game_cards,
    std::vector<int>& used_cards_index, std::vector<Card>& community_cards, int& bigBlindPosition, int& smallBlindPosition){

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
    int smallBlindPosition = playerCount - 1;
    int bigBlindPosition   = playerCount - 2;

    setup_once_per_game(players, game_cards, used_cards_index, communityCards,
                         smallBlindPosition, bigBlindPosition);
}

void Game::game() {
    pot = 0;
    currentBet = 0;
    currentPhase = GamePhase::PRE_FLOP;
    winnerMessage.clear();
    communityCards.clear();
    players.clear();

    std::vector<Card> game_cards = gui_utils::game_cards();

    setSmallBlind(1);
    setBigBlind(2);

    std::vector<std::string> game_names = Constants::NAMES;
    std::vector<int> used_cards_index;

    Game::setup_once(this->players, game_names);

    const int playerCount = static_cast<int>(players.size());
    int smallBlindPosition = playerCount - 1;
    int bigBlindPosition = playerCount - 2;

    Game::setup_once_per_game(this->players, game_cards, used_cards_index, communityCards,
                               smallBlindPosition, bigBlindPosition);
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

    if (currentPhase == GamePhase::SHOWDOWN) {
        resolveShowdown();
    }
}

void Game::resolveShowdown() {
    HandEvaluator handEvaluator;
    std::vector<Card> cards;
    std::vector<HandScore> evoluation_cards;

    for (size_t i = 0; i < players.size(); i++) {
        if (!players[i]->getInGame()) continue;
        cards = utils::cards7collection(players, i, communityCards);
        evoluation_cards.push_back(handEvaluator.evaluate(cards, players, communityCards, i));
    }

    if (evoluation_cards.empty()) {
        winnerMessage = "No winner";
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
        winnerMessage = "Winner: " + winner.name;
        for (size_t i = 0; i < players.size(); i++) {
            if (winner.name == players[i]->getName())
                players[i]->getMoney() += getPot();
        }
    } else {
        HandScore tieswinner = utils::max(ties);
        if (winner.sum_for_tie > tieswinner.sum_for_tie && winner.rank > tieswinner.rank) {
            winnerMessage = "Winner: " + winner.name;
            for (size_t i = 0; i < players.size(); i++) {
                if (winner.name == players[i]->getName())
                    players[i]->getMoney() += getPot();
            }
        } else {
            std::vector<std::string> tied_names;
            for (const auto& tie : ties) {
                if (std::find(tied_names.begin(), tied_names.end(), tie.name) == tied_names.end())
                    tied_names.push_back(tie.name);
            }
            winnerMessage = "Split pot: ";
            for (size_t n = 0; n < tied_names.size(); n++) {
                if (n > 0) winnerMessage += ", ";
                winnerMessage += tied_names[n];
            }
            int part = getPot() / static_cast<int>(tied_names.size());
            for (size_t i = 0; i < players.size(); i++) {
                if (std::find(tied_names.begin(), tied_names.end(), players[i]->getName()) != tied_names.end())
                    players[i]->getMoney() += part;
            }
        }
    }

}
