#pragma once

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <random>
#include <numeric>
#include <map>
#include <string>


class Player{
    private:
        
        std::string name;
        int money = 0;

        bool isBigBlind = false;
        bool isSmallBlind = false;
        bool isInGame = true;

        std::vector<std::string> cards2;
    
        
        
    public:
    Player();
        std::string getName();

        std::vector<std::string>& getcard2();
        

        void setName(std::string name);

        int getmoney();

        void setmoney(int money);


        bool getIsBigBlind();

        void ToggleBigBlind();

        void ToggleSmallBlind();

        void allin();
        
        

        void bet();
        void call();

        void check();
        void raise();

};

