#pragma once

#include "player.h"
#include <iostream>
#include <windows.h>

#include "utils.h"
#include "constants.h"

#include <memory>




class Game {

    private:
        int pot = 0;
        int currentBet = 0;
        int smallBlind = 0;
        int bigBlind = 0;
         

    public:
        int& getPot();
        void setPot(int pot);

        void setBigBlind(int bigBlind);
        int getBigBlind();
        

        int getSmallBlind();
        void setSmallBlind(int smallBlind);

        int getCurrentBet();

        void setCurrentBet(int currentBet);
        


    void setup_once(std::vector<std::unique_ptr<Player>>& players, std::vector<std::string>& game_names);

    void setup_once_per_game(std::vector<std::unique_ptr<Player>>& players, std::vector<Card>& game_cards, 
        std::vector<int>& used_cards_index, std::vector<Card>& community_cards, int& bigBlindPosition, int& smallBlindPosition);
    
    // void show_game(std::vector<Player> players, std::vector<Card>& community_cards);

    void game();
    

    ~Game();
    
};


