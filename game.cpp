#include <iostream>
#include <windows.h>
#include <memory>


#include "player.h"
#include "game.h"
#include "utils.h"
#include "handevoluator.h"
#include "constants.h"


int& Game::getPot(){
    return pot;
}

void Game::setPot(int pot){
    this->pot = pot;
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

    


    for (int i = 0; i < players.size(); i ++){
            if (players[i]->getIsSmallBlind()){
                players[i]->getMoney() -= getSmallBlind();
                pot += players[i]->getIsSmallBlind();
                std::cout << players[smallBlindPosition]->getName() << " set SmallBlind to: " << getSmallBlind(); 
                break;
        }
    }

    for(int i = 0; i < players.size(); i ++)
    {
        if (players[i]->getIsBigBlind()){
            players[i]->getMoney() -= getBigBlind();
            pot += players[i]->getIsBigBlind();
            std::cout << players[smallBlindPosition]->getName() << " set Big Blind to " << getBigBlind();
        }
    }

    HandEvoluator handEvo;
    
}


/*
void Game::show_game(std::vector<Player> players, std::vector<Card>& community_cards){
    for(int i = 0; i < players.size(); i ++){
        std::cout << players[i].getName() << std::endl;

    }
}
*/




void Game::game(){
    HandEvoluator handevoluator;


    std::vector<std::string> suits = {"♥️", "♦️", "♣️", "♠️"};
    std::vector<Card> game_cards;

    setSmallBlind(1);
    setBigBlind(2);
    
    for (auto s : suits) {
        for (int r = 2; r <= 14; r++) {
            game_cards.push_back({r, s});
        }
    }

    std::vector<std::unique_ptr<Player>> players;
    std::vector<std::string> game_names = Constants::NAMES;
    
    std::vector<Card> community_cards;
    
    std::vector<int> used_cards_index;
    std::vector<bool> card_evoluator;
    std::vector<std::string> cards7;
    
    Game::setup_once(players, game_names);

    int x = 1;
    int y = x+1;


    while(players.size()){
    
    int smallBlindPosition = players.size() - x;
    int bigBlindPosition = players.size() - y;

    std::vector<Card> cards;

    Game::setup_once_per_game(players, game_cards, used_cards_index, community_cards, smallBlindPosition, bigBlindPosition);
    for (size_t i = 0; i < players.size(); i++)
    {
        cards = utils::cards7collection(players, i, community_cards);    
        handevoluator.evoluator(cards, players, community_cards);
    }

    x++, y++;
    }

}




int main(int argc, char const *argv[]){
    
    std::unique_ptr<Game> game = std::make_unique<Game>();  //!!!!!
    game->Game::game();

    return 0;
}
