#pragma once


#include "handevoluator.hpp"
#include "constants.hpp"
#include "player.hpp"
#include "utils.hpp"


class HandEvoluator : public Player{

    public:
    int RoyalFlush(std::vector<Card>& cards, int& sum);
    int StraightFlush(std::vector<Card>& cards, int& sum);
    int FourOfKind(std::vector<Card>& cards, int& sum);
    int FullHouse(std::vector<Card>& cards, int& sum);
    int Flush(std::vector<Card>& cards, int& sum);
    int Straight(std::vector<Card>& cards, int& sum);
    int ThreeOfKind(std::vector<Card>& cards, int& sum);
    int TwoPairs(std::vector<Card>& cards, int& sum);
    int OnePair(std::vector<Card>& cards, int& sum);
    HandScore evoluator(std::vector<Card> &cards, std::vector<std::unique_ptr<Player>>& players, std::vector<Card> community_cards, int i);

};