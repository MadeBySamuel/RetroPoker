#pragma once

#include <algorithm>
#include <limits>

// One level lasts eight dealt hands. Changes apply before the next hand's blinds.
class BlindSchedule {
public:
    static constexpr int handsPerLevel = 8;
    static constexpr int maximumSmallBlind = std::numeric_limits<int>::max() / 2;

    explicit BlindSchedule(int smallBlind = 10)
        : startingSmallBlind(std::clamp(smallBlind, 1, maximumSmallBlind)) {
        reset();
    }

    void reset() {
        smallBlind = startingSmallBlind;
        level = 1;
        handInLevel = 1;
    }

    void advanceHand() {
        if (handInLevel < handsPerLevel) {
            ++handInLevel;
            return;
        }
        handInLevel = 1;
        if (level < std::numeric_limits<int>::max()) ++level;
        smallBlind = getNextSmallBlind();
    }

    int getSmallBlind() const { return smallBlind; }
    int getBigBlind() const { return smallBlind * 2; }
    int getLevel() const { return level; }
    int getHandsUntilIncrease() const { return handsPerLevel - handInLevel + 1; }
    int getNextSmallBlind() const {
        // Round a 50% increase up to a whole chip, keeping the big blind safe.
        return std::min(maximumSmallBlind, smallBlind + (smallBlind + 1) / 2);
    }
    int getNextBigBlind() const { return getNextSmallBlind() * 2; }

private:
    int startingSmallBlind;
    int smallBlind = 10;
    int level = 1;
    int handInLevel = 1;
};
