#pragma once

#include "player.hpp"
#include "utils.hpp"

class HandEvaluator {

    public:
    HandScore evaluate(const std::vector<Card>& cards, const std::string& playerName);

};
