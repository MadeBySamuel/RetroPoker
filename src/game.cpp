#include <iostream>
#include <memory>

#include "game.hpp"
#include "utils.hpp"
#include "gui_utils.hpp"
#include "handevoluator.hpp"
#include "constants.hpp"
#include "logger.hpp"
#include "player.hpp"


int& Game::getPot(){
    return pot;
}

void Game::setPot(int amount){
    pot += amount;
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

    


    for (size_t i = 0; i < players.size(); i ++){
            if (players[i]->getIsSmallBlind()){
                players[i]->getMoney() -= getSmallBlind();
                setPot(getSmallBlind());
                std::cout << players[smallBlindPosition]->getName() << " set SmallBlind to: " << getSmallBlind() << std::endl; 
                break;
        }
    }

    for(size_t i = 0; i < players.size(); i ++)
    {
        if (players[i]->getIsBigBlind()){
            players[i]->getMoney() -= getBigBlind();
            setPot(getBigBlind());

            std::cout << players[bigBlindPosition]->getName() << " set Big Blind to " << getBigBlind() << std::endl;
            break;
        }
    }

    HandEvoluator handEvo;
    
}



void Game::game(){

    std::vector<Card> game_cards = gui_utils::game_cards();

    HandEvoluator handevoluator;
    Logger logger;


    setSmallBlind(1);
    setBigBlind(2);
    


    std::vector<std::string> game_names = Constants::NAMES;
    
    std::vector<Card> community_cards;
    
    std::vector<int> used_cards_index;
    std::vector<bool> card_evoluator;
    std::vector<std::string> cards7;
    
    Game::setup_once(this->players, game_names);


    logger.log("number of players: " + std::to_string(players.size()) , "DEBUG");
    
    int blindOffset = 0;
    int round_count = 1;



    // while(!players.empty()){

    std::cout << "------------------" << std::endl;
    std::cout << "Round ---------- " << round_count << std::endl;
    std::cout << "------------------" << std::endl;

    const int playerCount = static_cast<int>(this->players.size());

    int smallBlindPosition = (playerCount - 1 - blindOffset + playerCount) % playerCount;
    int bigBlindPosition = (playerCount - 2 - blindOffset + playerCount) % playerCount;

    std::vector<Card> cards;
    std::vector<HandScore> evoluation_cards;

    std::cout << "$$ PRE-FLOP STAGE $$" << std::endl;

    Game::setup_once_per_game(this->players, game_cards, used_cards_index, community_cards, smallBlindPosition, bigBlindPosition);
    for (size_t i = 0; i < this->players.size(); i++)
    {
        cards = utils::cards7collection(this->players, i, community_cards);    
        evoluation_cards.push_back(handevoluator.evoluator(cards, this->players, community_cards, i));
    }
    std::cout << "Your cards: " << std::endl;

    utils::show_my_cards(this->players);
    
    
    for (size_t i = 0; i < 3; i ++){
        std::cout << community_cards[i].write_name() << std::endl;
    }

    std::cout << community_cards[3].write_name() << std::endl;


    for (size_t i = 0; i < this->players.size(); i ++){
        if (this->players[i]->getName() == "Player" && this->players[i]->getInGame() == true){
        }
        else if (this->players[i]->getName() != "Player" && this->players[i]->getInGame() == true) {

        }
    }

    std::cout << community_cards[4].write_name() << std::endl;





        utils::sort_function_hand_score(evoluation_cards);

    std::vector<HandScore> ties; 
    for (int i = 0; i < evoluation_cards.size() - 1; i ++){
        if (evoluation_cards[i].rank == evoluation_cards[i+1].rank && evoluation_cards[i].sum_for_tie == evoluation_cards[i+1].sum_for_tie){
        ties.push_back(evoluation_cards[i]);
        ties.push_back(evoluation_cards[i+1]);
        }


    } 


    HandScore winner = utils::max(evoluation_cards);

    if (ties.empty()){
        std::cout << "Winner of round is " << winner.name;
        for (size_t i = 0; i < this->players.size(); i++){
            if (winner.name == this->players[i]->getName()){
                this->players[i]->getMoney() += this->getPot();
            }
        }
    }
    else{
        HandScore tieswinner = utils::max(ties);

        if (winner.sum_for_tie > tieswinner.sum_for_tie && winner.rank > tieswinner.rank) {
            std::cout << "Winner of round is " << winner.name;
            for (size_t i = 0; i < this->players.size(); i++){
                if (winner.name == this->players[i]->getName()){
                    this->players[i]->getMoney() += this->getPot();
                }
            }
            blindOffset = (blindOffset + 1) % playerCount;
            //continue;
        }

        std::vector<std::string> tied_names;
        for (const auto& tie : ties) {
            if (std::find(tied_names.begin(), tied_names.end(), tie.name) == tied_names.end()) {
                tied_names.push_back(tie.name);
            }
        }

        if (tied_names.empty()) {
            blindOffset = (blindOffset + 1) % playerCount;
            //continue;
        }

        int part = this->getPot() / static_cast<int>(tied_names.size());

        for (size_t i = 0; i < this->players.size(); i ++){
            if (std::find(tied_names.begin(), tied_names.end(), this->players[i]->getName()) != tied_names.end()) {
                this->players[i]->getMoney() += part;
            }
        }
    }
    

    blindOffset = (blindOffset + 1) % playerCount;
    round_count++;
    }

// }


int game(){
    
    std::unique_ptr<Game> game = std::make_unique<Game>();  //!!!!!
    game->Game::game();

    return 0;
}
