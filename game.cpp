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
    std::vector<HandScore> evoluation_cards;


    Game::setup_once_per_game(players, game_cards, used_cards_index, community_cards, smallBlindPosition, bigBlindPosition);
    for (size_t i = 0; i < players.size(); i++)
    {
        cards = utils::cards7collection(players, i, community_cards);    
        evoluation_cards.push_back(handevoluator.evoluator(cards, players, community_cards, i));
    }


    for (size_t i = 0; i < players.size(); i ++){
        
    }



        utils::sort_function_hand_score(evoluation_cards);

    std::vector<HandScore> ties; 
    std::vector<HandScore> winner_ties;


    for (int i = 0; i < evoluation_cards.size() - 1; i ++){
        if (evoluation_cards[i].rank == evoluation_cards[i+1].rank && evoluation_cards[i].sum_for_tie == evoluation_cards[i+1].sum_for_tie){
        ties.push_back(evoluation_cards[i]);
        ties.push_back(evoluation_cards[i+1]);
        }


    } 


    HandScore winner = utils::max(evoluation_cards);
    HandScore tieswinner = utils::max(ties);


    if (ties.empty() == 0  || (winner.sum_for_tie > tieswinner.sum_for_tie && winner.rank > tieswinner.rank)){
        std::cout << "Winner of round is " << winner.name;
        for (size_t i = 0; i < players.size(); i++){
            if (winner.name == players[i]->getName()){
                players[i]->getMoney() += this->getPot();
            }
        }
    }
    else{
        int counter = 0; 
        utils::sort_function_rank(ties);

        for (int i = 0; i < ties.size() -1; i++){
            if (ties[i].rank == ties[i+1].rank){
                counter += 2;
            }
        }
        int part = this->getPot() / counter;

        for (size_t i = 0; i < counter; i++){
            winner_ties.push_back(ties[i]);
        }

        for (size_t i = 0; i < players.size(); i ++){
            if (winner_ties[i].name == players[i]->getName()){
                players[i]->getMoney() += part;
            }
        }   
    }
    

    x++, y++;
    }

}




int main(int argc, char const *argv[]){
    
    std::unique_ptr<Game> game = std::make_unique<Game>();  //!!!!!
    game->Game::game();

    return 0;
}
