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


#include "constants.h"


class Player{
    private:
        
        std::string name;
        int money = 0;

        bool isBigBlind = false;
        bool isSmallBlind = false;
        bool inGame = true;

        std::vector<Card> cards2;
    
        
        
    public:
    Player();
        std::string getName();

        std::vector<Card>& getcard2();
        

        void setName(std::string name);

        int& getMoney();

        void setMoney(int money);


        bool getIsBigBlind();

        bool getIsSmallBlind();


        void ToggleBigBlind();

        void ToggleSmallBlind();

        int getInGame();
        void setInGame(bool inGame);

        ~Player();
    };



