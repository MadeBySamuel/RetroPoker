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
    setSmallBlind(1);
    setBigBlind(2);
    

    
    used_cards_index.clear();                                                             
    
    utils::shuffle_cards(game_cards);
    utils::card_selection(players, used_cards_index, game_cards);
    utils::community_cards_selection(game_cards, used_cards_index, community_cards);
    utils::blindSelection(players, bigBlindPosition, smallBlindPosition);
    


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

    std::vector<Play> game_evoluation; 
    int max_element = 0;
    
    std::vector<Play> list;
    for (int i = 0; i < players.size(); i ++)
    {
        std::vector<Card> map;
        std::vector<Play> map_of_plays(3);
        
        int max_element_of_person = 0;

        map = utils::cards7collection(players, community_cards);

        map_of_plays[i].id = players[i]->getName();

        map_of_plays = handEvo.evoluator(map, i);

    
        max_element_of_person = *std::max_element(map_of_plays[i].numbers.begin(), map_of_plays[i].numbers.end());      // toto vracia iterator čiže * <- need 
        game_evoluation[i].numbers.push_back(max_element_of_person);
    }







    
}

/*
void Game::show_game(std::vector<Player> players, std::vector<Card>& community_cards){
    for(int i = 0; i < players.size(); i ++){
        std::cout << players[i].getName() << std::endl;

    }
}
*/




void Game::game(){

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

    int smallBlindPosition = players.size() - 1;
    int bigBlindPosition = players.size() - 2;

    // test 
    if (players.size() != 1){

        Game::setup_once_per_game(players, game_cards, used_cards_index, community_cards, smallBlindPosition, bigBlindPosition);
        

    }
    else{
        for (int i = 0; i < players.size(); i ++){
        std::cout << "Congratulations " << players[i]->getName();
        }
    }

    std::cout << players.size();

    


/*

    for (size_t i = 0; i < players.size(); i++){
        for(size_t j = 0; j < 2; j++){
            cards7.push_back(players[i]->getcard2()[j]);
        }
        
        for (size_t j = 0; j < 2; j ++){
            cards7.push_back(community_cards[j]);
        }
    
    }

*/
        

    /*
    players.shrink_to_fit();
    game_names.shrink_to_fit();
    game_cards.shrink_to_fit();

    setup_once(players,game_names);
    setup_once_per_game(players, game_cards, used_cards_index, community_cards);
    */
}

Game::~Game(){
    std::cout << "Game was killed" << std::endl;
}
    


int main(int argc, char const *argv[]){
    std::unique_ptr<Game> game = std::make_unique<Game>();  //!!!!!
    game->Game::game();

    return 0;
}
