#include "aiplayer.h"


void AIPlayer::ai_moves(std::vector<std::unique_ptr<Player>>& players, int i, int raise_amount){

    std::vector<std::function<void()>> moves = {
        allin(players, i),
        fold(players, i),
        check(players, i),
        call(players, i, raise_amount),
        raise_bet(players, i, raise_amount)
    }
    }



  

