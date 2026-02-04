#pragma once


#include "player.h"
#include "utils.h"

struct Play{
    std::string id;
    std::vector<int> numbers;
};

class HandEvoluator : public Player{

    public:
    int RoyalFlush(std::vector<Card>& cards);
    int StraightFlush(std::vector<Card>& cards);
    int FourOfKind(std::vector<Card>& cards);
    int FullHouse(std::vector<Card>& cards);
    int Flush(std::vector<Card>& cards);
    int Straight(std::vector<Card>& cards);
    int ThreeOfKind(std::vector<Card>& cards);
    int TwoPairs(std::vector<Card>& cards);
    int OnePair(std::vector<Card>& cards);
    std::vector<Play> evoluator(std::vector<Card>& cards, int i);

};