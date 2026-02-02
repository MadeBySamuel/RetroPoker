/*
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <random>
#include <numeric>
#include <map>
#include <string>

#include "constants.h"


int main(int argc, char const *argv[])
{
   srand(time(NULL));

    std::vector<Player> players(Constants::MAX_PEOPLE);


    std::cout << std::endl;


    game_names.push_back("Dealer");



    std::cout << std::endl;

    std::vector<int> used_cards_index;

    for(int i = players.size() - 1; i > players.size() - 2; i--){
        players[i].ToggleBigBlind();
        std::cout << players[i].getName() << " is BigBlind. " << std::endl;
    }

    for(int i = players.size() - 2; i > players.size() - 3; i--){
        players[i].ToggleSmallBlind();
        std::cout << players[i].getName() << " is SmallBlind. " << std::endl;
    }

    for (int i = 0; i < players.size(); i++){
        if (players[i].getIsBigBlind() == true){
            players[i].getstartMoney() -= 2;
            pot += 2;
            std::cout << players[i].getName() << " put 2€ - BigBlind. " << std::endl;
            
            players[i - 1].getstartMoney() -= 1;
            pot += 1;
            std::cout << players[i].getName() << " put 1€ - SmallBlind. " << std::endl;
        }
    }



    




    std::cout << std::endl;

    std::cout << "Your cards: " << std::endl;

    


    for (int i = 0; i < 3; i++){
        std::cout << community_cards[i];
    }



    std::map<int,std::string> rank;

    for(int i = 0; i < 13; i++){
    rank.insert({i + 1 ,Constants::CARDS[i].substr(Constants::CARDS[i].find(' ') + 1)});
    }


    for (int i = players.size() - 3; i > 0; i--){
        for(int j = 0; j < 2; j ++)
        {
            int number = 0;
            std::string a = players[i].getcard2()[j].substr(players[i].getcard2()[j].find(' ') + 1);
            if (a == "J") number = 11;
            else if (a == "Q") number = 12;
            else if (a == "K") number = 13;
            else if (a == "A") number = 14;
            else number  = std::stoi(a);
        }
    }

    

    






   





    return 0;
}


for(int i = 0; i < 5; i ++){
            int number = rand()% game_cards.size() + 1;
            if(std::find(used_cards_index.begin(), used_cards_index.end(), number) == used_cards_index.end())
                {
                community_cards.push_back(game_cards[number]);
                }
        }

*/