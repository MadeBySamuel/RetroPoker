#include <vector>
#include <string>

#include "constants.h"

namespace Constants {
std::vector <std::string> NAMES = {"Peter", "Paul", "Morgan", "Samuel", "Jacub", "Sabrina"};
int MAX_PEOPLE = std::size(NAMES);



const std::vector<std::string> CARDS = { "♥️ A","♥️ 2", "♥️ 3", "♥️ 4","♥️ 5 ", "♥️ 6", "♥️ 7", "♥️ 8", "♥️ 9", "♥️ 10","♥️ J", "♥️ Q", "♥️ K", 
    "♦️ A","♦️ 2", "♦️ 3", "♦️ 4","♦️ 5", "♦️ 6", "♦️ 7", "♦️ 8", "♦️ 9", "♦️ 10","♦️ J", "♦️ Q", "♦️ K",
    "♣️ A","♣️ 2", "♣️ 3", "♣️ 4","♣️ 5", "♣️ 6", "♣️ 7", "♣️ 8", "♣️ 9", "♣️ 10","♣️ J", "♣️ Q", "♣️ K",
    "♠️ A","♠️ 2", "♠️ 3", "♠️ 4","♠️ 5", "♠️ 6", "♠️ 7", "♠️ 8", "♠️ 9", "♠️ 10","♠️ J", "♠️ Q", "♠️ K"
};

const int AMOUNT_IN_PACK = std::size(CARDS); 




}



