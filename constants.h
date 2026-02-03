#pragma once
#include <vector>
#include <string>

struct Card{
    int rank;
    std::string name;

    std::string write_name(){
        return name + " " + rank_to_string(rank);
    }

    std::string rank_to_string(int rank) {
        if (rank == 11) return "J";
        if (rank == 12) return "Q";
        if (rank == 13) return "K";
        if (rank == 14) return "A";
        return std::to_string(rank);
    }
};


namespace Constants {
    extern std::vector<std::string> NAMES;
    extern int MAX_PEOPLE;

    int START_PLAYER_MONEY = 200;

    extern const int AMOUNT_IN_PACK;
}
