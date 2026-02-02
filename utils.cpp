#include "player.h"
#include "constants.h"

#include "utils.h"


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


void utils::shuffle_cards(std::vector<std::string>& cards){
    utils::shuffle_function(cards);
}

void utils::shuffle_players(std::vector<std::string>& names){
    utils::shuffle_function(names);
}











int utils::set_player_position(std::vector<std::string>& game_names){
    int player_position = rand()% 6;
    game_names[player_position] = "Player";

    return player_position;
}


std::vector<Player> utils::createPlayers(std::vector<std::string>& game_names, std::vector<Player>& players){
    
    for(int i = 0; i < game_names.size(); i++){
        players[i].setName(game_names[i]);
        if (game_names[i] == "Player") players[i].setmoney(Constants::START_PLAYER_MONEY);
        // tu sa hracom negenruju peniaze lebo uz boli pri tvorbe vygenerovane konstruktorom

    }

    return players;

}




void utils::show_my_cards(std::vector<Player> players){
for(int i = 0; i < players.size(); i++){
        if(players[i].getName() == "Player"){
            for (int j = 0; j < players[i].getcard2().size(); j++){
                std::cout << players[i].getcard2()[j] << std::endl;
            }
        }
    } 
}

void utils::card_selection(std::vector<Player> players, std::vector<int> used_cards_index, std::vector<std::string> game_cards){
    

    for (int i = 0; i < players.size(); i++){
        
        int card1 = rand()% game_cards.size() + 1;
        int card2 = rand()% game_cards.size() + 1;
        if (std::find(used_cards_index.begin(), used_cards_index.end(), card1) == used_cards_index.end() &&
            std::find(used_cards_index.begin(), used_cards_index.end(), card2) == used_cards_index.end())
        {
            used_cards_index.push_back(card1);
            players[i].getcard2().push_back(game_cards[card1]);
            used_cards_index.push_back(card2);
            players[i].getcard2().push_back(game_cards[card2]);
            break;
        }
}
}

void utils::community_cards_selection(std::vector<std::string>& game_cards, std::vector<int>& used_cards_index, std::vector<std::string>& community_cards){
    for(int i = 0; i < 5; i ++){
        int number = rand()% game_cards.size() + 1;
        if(std::find(used_cards_index.begin(), used_cards_index.end(), number) == used_cards_index.end())
        {
            community_cards.push_back(game_cards[number]);
        }
    }
}