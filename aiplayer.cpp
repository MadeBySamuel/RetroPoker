#include "aiplayer.h"


void AIPlayer::ai_moves(std::vector<Player>& players, int i){
    
    int j = 0;
    Plays plays;

    std::random_device rd;
    std::mt19937 engine(rd());

    float percent_of_money;

    std::uniform_int_distribution<int> dist(70,100);

    int chance_to_raise = dist(engine);

    int chance_to_check = dist(engine);


    int sum = players[i].getcard2()[j].rank + players[i].getcard2()[j+1].rank;

    if (sum > 4 && sum < 7)
    {
        int percent;
    }
    else if (sum >= 7 && sum < 12){
        int percent;
    }
    else if (sum >= 12 && sum < 18){
        int percent;
    }
    else if (sum >= 18 && sum < 22){
        int percent;
    }
    else if (sum >= 22 ){
        int percent;
        int raise_amount = players[i].getMoney() * percent;
        raise_amount = plays.Plays::raise_bet(players, i, raise_amount);
    }                                                       
    




    
    



    if (chance_to_raise > chance_to_check){
        if (sum > 15){

        }
       
    }



  

}
