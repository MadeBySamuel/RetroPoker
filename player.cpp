#include "player.h"

 
Player::Player(){
    money = (rand() % 11 + 1) * 10; 
}

std::string Player::getName(){
    return name;
}

std::vector<Card>& Player::getcard2(){
    return cards2;
}

void Player::setName(std::string name){
    this->name = name;
}

int& Player::getMoney(){
    return money;
}

void Player::setMoney(int money){
    this->money = money; 
}


bool Player::getIsBigBlind(){
    return isBigBlind;
}

bool Player::getIsSmallBlind(){
    return isSmallBlind;
}

void Player::ToggleBigBlind(){
    isBigBlind = !isBigBlind;
}

void Player::ToggleSmallBlind(){
    isSmallBlind = !isSmallBlind;
}

int Player::getInGame(){
    return inGame;
}

void Player::setInGame(bool inGame){
    this->inGame = inGame;
}


Player::~Player(){
        std::cout << "Player was deleted. " << std::endl;
}


    


