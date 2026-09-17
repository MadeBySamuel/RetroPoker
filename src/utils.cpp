#include "player.hpp"
#include "constants.hpp"

#include "utils.hpp"
#include "game.hpp"
#include <argon2.h>


#include <memory>


void utils::sort_function(std::vector<Card>& cards){
    std::sort(cards.begin(), cards.end(),[](const Card& a,const Card& b){
            return a.rank < b.rank;
    });
    
}

void utils::sort_function_hand_score(std::vector<HandScore>& hand_cards){
    std::sort(hand_cards.begin(), hand_cards.end(),[](const HandScore& a,const HandScore& b){
            return a.sum_for_tie < b.sum_for_tie;
    });
}

void utils::sort_function_rank(std::vector<HandScore>& hand_cards){
    std::sort(hand_cards.begin(), hand_cards.end(),[](const HandScore& a,const HandScore& b){
            return a.rank > b.rank;
    });
}


HandScore utils::max(std::vector<HandScore>& cards){
    auto max_it = std::max_element(cards.begin(), cards.end(),[](const HandScore &a, const HandScore &b) {
        return a.sum_for_tie < b.sum_for_tie;  // vracia true, ak a < b
    
    });

    return *max_it;
}


void utils::sort_name(std::vector<Card> cards){
    std::sort(cards.begin(), cards.end(),[](Card& a, Card& b){
            return a.name < b.name;
    });
    }


bool utils::is_valid(int money, int bet_amount){
    if (bet_amount > money) {
        std:: cout << "You cant bet bigger amount than your money " << std::endl;
        return false;
    }
    else if (bet_amount < 0){
        std::cout << "You cant bet negative amount of money " << std::endl;
        return false;
    }
    else if(bet_amount == 0){
        std::cout << "Your bet must me more than 0" << std::endl;
        return false;
    }
    else {
        std::cout << "Your bet: " << bet_amount << std::endl;
        return true;
    }
}


void utils::shuffle_cards(std::vector<Card>& cards){
    if (cards.empty ()) {
        return;
    }
    else{
        utils::shuffle_function(cards);
    } 
}

void utils::shuffle_players(std::vector<std::string>& names){
    utils::shuffle_function(names);
}

int utils::set_player_position(){
    
    std::vector<std::string> game_names = Constants::NAMES;

    if (game_names.empty()) {
        return -1;
    }

    int player_position = 0;
    game_names[player_position] = "Player";
    
    return player_position;
}


std::vector<std::unique_ptr<Player>>& utils::createPlayers(std::vector<std::unique_ptr<Player>>& players, int numberOfPlayers){
    players.clear();
    std::vector<std::string> game_names = Constants::NAMES;
    players.reserve(numberOfPlayers);


    for (size_t i = 0; i < numberOfPlayers; i++) {
        if (i == 0){
            players.push_back(std::make_unique<Player>("Player", 200, true));
        }
        else {
            players.push_back(std::make_unique<Player>(game_names[i], 200, false));
        }

    }

    return players;

}


void utils::show_my_cards(std::vector<std::unique_ptr<Player>>& players){
for(size_t i = 0; i < players.size(); i++){
        if(players[i]->getName() == "Player"){
            for (size_t j = 0; j < players[i]->getcard2().size(); j++){
                std::cout << players[i]->getcard2()[j].write_name() << std::endl;  
            }
        }
    } 
}

void utils::settingInGameStatus(std::vector<std::unique_ptr<Player>>& players){
    
    for(size_t i = 0; i < players.size(); i++){
        auto& player = players[i];
        if (player->getMoney() <= 0 && player->isBot()){
            player->setInGame(false);
            player->setEliminated();
        }
        else {
            players[i]->setInGame(true);
        }
    }
}


void utils::card_selection(std::vector<std::unique_ptr<Player>>& players, std::vector<int>& used_cards_index, std::vector<Card>& game_cards){
    

    for (auto& player: players){
        player->getcard2().clear();

        int card1;
        int card2;
        do{
        card1 = rand() % game_cards.size();
        card2 = rand() % game_cards.size();
        }while(card1 == card2 ||
        std::find(used_cards_index.begin(), used_cards_index.end(), card1) != used_cards_index.end() ||
        std::find(used_cards_index.begin(), used_cards_index.end(), card2) != used_cards_index.end());

        used_cards_index.push_back(card1);
        player->getcard2().push_back(game_cards[card1]);
        used_cards_index.push_back(card2);
        player->getcard2().push_back(game_cards[card2]);
    }
}


void utils::community_cards_selection(std::vector<Card>& game_cards, std::vector<int>& used_cards_index, std::vector<Card>& community_cards){
    community_cards.clear();

    while (community_cards.size() < 5) {
        int number = rand() % game_cards.size();
        if(std::find(used_cards_index.begin(), used_cards_index.end(), number) == used_cards_index.end())
        {
            used_cards_index.push_back(number);
            community_cards.push_back(game_cards[number]);
        }
    }
}

void utils::dealerSelection(std::vector<std::unique_ptr<Player>>& players, int& dealerPosition){
    if (players.empty()){
        return;
    }
    
    for (auto& player : players){
        if (player->getIsDealer()){
            player->ToggleDealer();
        }
    }
    players[dealerPosition]->ToggleDealer();
}

void utils::blindSelection(std::vector<std::unique_ptr<Player>>& players, int& bigBlindPosition, int& smallBlindPositon){
    if (players.empty()) {
        return;
    }

    for (auto& player : players) {
        if (player->getIsBigBlind()) {
            player->ToggleBigBlind();
        }
        if (player->getIsSmallBlind()) {
            player->ToggleSmallBlind();
        }
    }

    players[bigBlindPosition]->ToggleBigBlind();
    players[smallBlindPositon]->ToggleSmallBlind();
}

std::vector<Card> utils::cards7collection(std::vector<std::unique_ptr<Player>>& players, int i, std::vector<Card>& community_cards){
    
    std::vector<Card> map;

    
    for (size_t j = 0; j < players[i]->getcard2().size(); j++){
            map.push_back(players[i]->getcard2()[j]);
    }

    for (size_t i = 0; i < community_cards.size(); i ++){
            map.push_back(community_cards[i]);
    }
        return map;
    

    return map;
}


