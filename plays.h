#pragma once

#include "utils.h"
#include "game.h"
#include "player.h"




class Plays : public Game{


    public:
        void allin(std::vector<std::unique_ptr<Player>>& players, int i);
        void fold(std::vector<std::unique_ptr<Player>>& players, int i);
        void check(std::vector<std::unique_ptr<Player>>& players, int i);
        void call(std::vector<std::unique_ptr<Player>>& players, int i, int raise_amount);
        int raise_bet(std::vector<std::unique_ptr<Player>>& players, int i, int raise_amount);
    
};