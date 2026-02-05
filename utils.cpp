#include "player.h"
#include "constants.h"

#include "utils.h"
#include "game.h"

#include <memory>


void utils::sort_function(std::vector<Card>& cards){
    std::sort(cards.begin(), cards.end(),[](const Card& a,const Card& b){
            return a.rank < b.rank;
    });
    
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


std::vector<std::unique_ptr<Player>>& utils::createPlayers(std::vector<std::string>& game_names, std::vector<std::unique_ptr<Player>>& players){
    
    players.reserve(Constants::MAX_PEOPLE);

    for (size_t i = 0; i < Constants::MAX_PEOPLE; ++i) {
        players.push_back(std::make_unique<Player>());
    }

    for(int i = 0; i < game_names.size(); i++){
        players[i]->setName(game_names[i]);
        if (game_names[i] == "Player") players[i]->setMoney(Constants::START_PLAYER_MONEY);
        // tu sa hracom negenruju peniaze lebo uz boli pri tvorbe vygenerovane konstruktorom

    }

    return players;

}




void utils::show_my_cards(std::vector<std::unique_ptr<Player>>& players){
for(int i = 0; i < players.size(); i++){
        if(players[i]->getName() == "Player"){
            for (int j = 0; j < players[i]->getcard2().size(); j++){
                std::cout << players[i]->getcard2()[j].write_name() << std::endl;  
            }
        }
    } 
}

void utils::settingInGameStatus(std::vector<std::unique_ptr<Player>>& players){
    for(size_t i = 0; i < players.size(); i ++){
        players[i]->setInGame(true);
    }
}


void utils::card_selection(std::vector<std::unique_ptr<Player>>& players, std::vector<int>& used_cards_index, std::vector<Card>& game_cards){
    

    for (auto& player: players){
        int card1;
        int card2;
        do{
        card1 = rand()% game_cards.size() + 1;
        card2 = rand()% game_cards.size() + 1;
        }while(std::find(used_cards_index.begin(), used_cards_index.end(), card1) != used_cards_index.end()&&
        std::find(used_cards_index.begin(), used_cards_index.end(), card2) != used_cards_index.end());

        used_cards_index.push_back(card1);
        player->getcard2().push_back(game_cards[card1]);
        used_cards_index.push_back(card2);
        player->getcard2().push_back(game_cards[card2]);
    }
}


void utils::community_cards_selection(std::vector<Card>& game_cards, std::vector<int>& used_cards_index, std::vector<Card>& community_cards){
    for(std::size_t i = 0; i < 5; i ++){
        int number = rand()% game_cards.size() + 1;
        if(std::find(used_cards_index.begin(), used_cards_index.end(), number) == used_cards_index.end())
        {
            community_cards.push_back(game_cards[number]);
        }
    }
}

void utils::blindSelection(std::vector<std::unique_ptr<Player>>& players, int& bigBlindPosition, int& smallBlindPositon){

    if (players[bigBlindPosition]->getIsBigBlind() && players[smallBlindPositon]->getIsSmallBlind() )
    {
        players[bigBlindPosition]->ToggleBigBlind();
        players[smallBlindPositon]->ToggleSmallBlind();
        
        bigBlindPosition -= 1;
        smallBlindPositon -= 1;
    }
    else{
        players[bigBlindPosition]->ToggleBigBlind();
        players[smallBlindPositon]->ToggleSmallBlind();
    }
}

std::vector<Card> utils::cards7collection(std::vector<std::unique_ptr<Player>>& players, int i, std::vector<Card>& community_cards){
    
    std::vector<Card> map;

    
    for (int j = 0; j < players[i]->getcard2().size(); j++){
            map.push_back(players[i]->getcard2()[j]);
    }

    for (size_t i = 0; i < community_cards.size(); i ++){
            map.push_back(community_cards[i]);
    }
        return map;
    

    return map;
}


