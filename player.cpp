
#include "player.h"

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <random>
#include <numeric>
#include <map>
#include <string>


     
Player::Player(){
    money = (rand() % 11 + 1) * 10; 
}

std::string Player::getName(){
    return name;
}

std::vector<std::string>& Player::getcard2(){
    return cards2;
}

void Player::setName(std::string name){
    this->name = name;
}

int Player::getmoney(){
    return money;
}

void Player::setmoney(int money){
    this->money = money; 
}


bool Player::getIsBigBlind(){
    return isBigBlind;
}

void Player::ToggleBigBlind(){
    isBigBlind = !isBigBlind;
}

void Player::ToggleSmallBlind(){
    isSmallBlind = !isSmallBlind;
}

void Player::allin(){
char choice;
std::cout << "Are you sure you want  to go all in ? y/n" << std::endl;
std::cin >> choice;
}



void Player::bet(){
    std::string bet_amount;
while(true){ 
    std::cout << "Set your betting amount: " << std::endl; 
    std::cin >> bet_amount;

    if(std::all_of(bet_amount.begin(), bet_amount.end(), ::isdigit)){
        int input = std::stoi(bet_amount);

        if (input > this->money){
            std::cout << "You cannot bet more money than you have " << std::endl;  
        }
        else if (input == 0){
            std::cout << "You cannot bet zero " << std::endl;
        }
        else if (input < 0) {
            std::cout << "You cannot bet negative amount of money " << std::endl;
        }
        else {
            std::cout << "You bet " << input << " €" << std::endl;
        }
        this->money -= input;
        break;               
    }

    else{
        std::cout << "Sorry but you have to input a number not string " << std::endl;
    }
}
}

void Player::call(){
}

void Player::check(){

}
/*
void Player::raise(){
    std::string raise_amount;
    while (true)
{
    std::cout << "Enter amount of money you want to raise or press q to choose other option "; 
    std::cin >> raise_amount;


    if(std::all_of(raise_amount.begin(), raise_amount.end(), ::isdigit)){
        int input = std::stoi(raise_amount);
        
        if (input > this->money){
            std::cout << "You cannot raise more money than you have " << std::endl;  
            continue;
        }
        else if (input == 0){
            std::cout << "You cannot raise zero " << std::endl;
            continue;
        }
        else if (input < 0) {
            std::cout << "You cannot raise less money than zero " << std::endl;
            continue;
        }

        std::cout << "You added " << input << " € to pot" << std::endl;
        this->money -= input;
        pot += input;
        break;

    }

    else {
        std::cout << "Please enter number not string " << std::endl;
    }

    
}
    
}
*/
