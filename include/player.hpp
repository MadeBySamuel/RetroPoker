#pragma once

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <random>
#include <numeric>
#include <map>
#include <string>
#include <memory>


#include "constants.hpp"

#include "gui_utils.hpp"


class Player{
    private:
        
        std::string name;
        int money = 0;
        int betThisRound = 0;
        int committedThisHand = 0;


        bool isDealer = false;
        bool isBigBlind = false;
        bool isSmallBlind = false;
        bool inGame = true;
        bool isHuman = true;
        bool isEliminated = false;
        bool hasActed = false;
        bool isAllIn = false;



        std::vector<Card> cards2;
    
        
        
    public:
    Player(const std::string& name, int money, const bool isBot);

        bool isRealPlayer();
        bool isBot();

        std::string getName();

        std::vector<Card>& getcard2();
        

        void setName(std::string name);

        int& getMoney();

        void setMoney(int money);

        void ToggleDealer();
        bool getIsDealer();

        bool getIsBigBlind();

        bool getIsSmallBlind();


        void ToggleBigBlind();

        void ToggleSmallBlind();

        bool isInGame();
        bool getIsEliminated() const;

        void setInGame(bool inGame);
        void setEliminated();

        void setBetThisRound(int betThisRound);
        int& getBetThisRound();
        void addCommittedThisHand(int amount);
        int getCommittedThisHand() const;
        void resetCommittedThisHand();

        void setActed(bool hasActed);
        bool getActed();


        void setAllIn(bool isAllIn);
        bool getAllIn();


        ~Player();
    };

