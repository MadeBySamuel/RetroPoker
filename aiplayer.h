#pragma once

#include "player.h"
#include "plays.h"
#include <functional>
#include <random>


class AIPlayer : public Plays {    
    void ai_moves(std::vector<std::unique_ptr<Player>>& players, int i, int raise_amount);
};
