#include "plays.h"



    void Plays::allin(std::vector<Player>& players, int i){   
        int amount = 0; 
        if (players[i].getName() == "Player"){
        char choice;  
        std::cout << "Are you sure you want to go all in? (y/n) ";
        std::cin >> choice;

        if (choice == 'y'){
            amount = players[i].getMoney();
            players[i].getMoney() = 0;
            getPot() += players[i].getMoney();           // tu treba zmenit metodu getPot() na vracanie referencie - inak error expression must be a modifiable lvalue
            }
        }
        else{
            players[i].getMoney() = 0;
            getPot() += players[i].getMoney(); 
        }

    std::cout << "Player " << players[i].getName() << " is going all in " << std::endl;
    std::cout << amount << " is added to pot. " << std::endl;

    }



    void Plays::fold(std::vector<Player>& players, int i){
        
        std::cout << "Player " << players[i].getName() << " is folding." << std::endl;
        players[i].setInGame(false);
    
    }

    void Plays::check(std::vector<Player>& players, int i){
        std::cout << "Player " << players[i].getName() << " check" << std::endl;
    }


    void Plays::call(std::vector<Player>& players, int i, int raise_amount){
    
        players[i].getMoney() -= raise_amount;
        getPot() += raise_amount;
    }

    int Plays::raise_bet(std::vector<Player>& players, int i, int raise_amount){

        if (players[i].getName() == "Player"){

            std::string raise_amount;
            std::cout << "Enter amount of money you want to raise " << std::endl;
            std::cin >> raise_amount; 
            
            if (std::all_of(raise_amount.begin(), raise_amount.end(), ::isdigit)){
                int bet = std::stoi(raise_amount);

                if (bet > players[i].getMoney())
                {
                    std::cout << "Raise is invalid! Your bet is higher than money you own." << std::endl;
                    return 0;
                }
                else if (bet == players[i].getMoney()){
                    allin(players, i);
                    return 0;
                }
                else if (bet == 0){
                    std::cout << "Raise is invalid! Your bet must be higher than 0" << std::endl;
                    return 0;
                }
                else if (bet < 0){
                    std::cout << "Raise is invalid! Your bet must be positive amount " << std::endl; 
                    return 0;          
                }
                else{
                    players[i].getMoney() -= bet;
                    getPot() += bet;
                    return bet;
                }
            }
            else{
                std::cout << "Invalid bet you have to enter a positive number " << std::endl;
                return 0;
            }
        }
        else {
            players[i].getMoney() -= raise_amount;
            getPot() += raise_amount;
            return raise_amount;
        }
    
    }
