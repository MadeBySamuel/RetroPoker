#include "player.h"

class HandEvoluator : public Player{

    HandEvoluator();

    public:
    int RoyalFlush();
    int StraightFlush();
    int FourOfKind();
    int FullHouse();
    int Straight();
    int ThreeOfKind();
    int TwoPairs();
    int OnePair();
    void evoluator(std::vector<int>& list);

};