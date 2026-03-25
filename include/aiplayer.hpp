#pragma once

#include "plays.hpp"

class AIPlayer : public Plays {    
    void ai_moves(std::vector<std::unique_ptr<Player>>& players, int i, int raise_amount);
};
