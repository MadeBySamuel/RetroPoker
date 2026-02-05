#include "player.h"
#include "plays.h"


class AIPlayer {
    int chance_to_raise;
    int chance_to_fold;
    int chance_to_check;

    
    void ai_moves(std::vector<Player>& players, int i);
};
