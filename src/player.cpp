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

bool Player::isRealPlayer(){
    return isHuman;
}

bool Player::isBot(){
    return !isHuman;
}


int& Player::getMoney() {
    return money;
}

void Player::setMoney(int money){
    this->money = money; 
}
bool Player::getIsDealer(){
    return isDealer;
}

void Player::ToggleDealer(){
    this->isDealer = !isDealer;
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

bool Player::getIsEliminated() const{
    return isEliminated;
}

void Player::setEliminated(){
    isEliminated = true;
}

int& Player::getBetThisRound(){
    return betThisRound;
}

void Player::setBetThisRound(int betThisRound){
    this->betThisRound = betThisRound;
}

void Player::addCommittedThisHand(int amount){
    committedThisHand += amount;
}

int Player::getCommittedThisHand() const{
    return committedThisHand;
}

void Player::resetCommittedThisHand(){
    committedThisHand = 0;
}

void Player::setActed(bool hasActed){
    this->hasActed = hasActed;
}

bool Player::getActed(){
    return hasActed;
}

void Player::setAllIn(bool isAllIn){
    this->isAllIn = isAllIn;
}

bool Player::getAllIn(){
    return isAllIn;
}



void Player::setInGame(bool inGame){
    this->inGame = inGame;
}


Player::~Player(){
        std::cout << name << " was deleted. " << std::endl;
}


    
