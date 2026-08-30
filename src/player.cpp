#include "player.hpp"

 
Player::Player(const std::string& name, int money, const bool isHuman) : name(name), money(money), isHuman(isHuman) {}

std::string Player::getName(){
    return name;
}

std::vector<Card>& Player::getcard2(){
    return cards2;
}

void Player::setName(std::string name){
    this->name = name;
}

const bool Player::isRealPlayer(){
    return isHuman;
}

const bool Player::isBot(){
    return !isHuman;
}


int& Player::getMoney() {
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

bool Player::isInGame(){
    return inGame;
}

bool Player::isEliminated() const{
    return eliminated;
}

void Player::setEliminated(){
    eliminated = false;
}



void Player::setInGame(bool inGame){
    this->inGame = inGame;
}


Player::~Player(){
        std::cout << name << " was deleted. " << std::endl;
}


    

