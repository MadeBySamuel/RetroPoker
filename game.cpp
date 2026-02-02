#include "player.h"
#include <iostream>
#include <windows.h>
#include "utils.h"
#include "constants.h"



class Game {

    private:
        int pot = 0;
        int currentBet = 0;

    public:
        int getPot(){
            return pot;
        }
        void setPot(int pot){
            this->pot = pot;
        }


    void setup_once(std::vector<Player>& players, std::vector<std::string>& game_names){
        utils::shuffle_players(game_names);
        utils::set_player_position(game_names);
        utils::createPlayers(game_names, players);                         
    }

    void setup_once_per_game(std::vector<Player>& players, std::vector<std::string>& game_cards, std::vector<int>& used_cards_index, std::vector<std::string>& community_cards){
        
        used_cards_index.clear();                                                             
        utils::shuffle_cards(game_cards);
        utils::card_selection(players, used_cards_index, game_cards);
        utils::community_cards_selection(game_cards, used_cards_index, community_cards);

    }
    
    void game(){
        std::vector<Player> players(Constants::MAX_PEOPLE);
        std::vector<std::string> game_names = Constants::NAMES;
        std::vector<std::string> game_cards = Constants::CARDS;
        std::vector<std::string> community_cards;
        std::vector<int> used_cards_index;

        setup_once(players,game_names);
        setup_once_per_game(players, game_cards, used_cards_index, community_cards);
    }
    
};

int main(int argc, char const *argv[]){
    Game game;
    game.game();

    return 0;
}
