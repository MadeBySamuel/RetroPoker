#pragma once

#include "player.h"
#include "constants.h"

namespace utils{
bool is_valid(int money, int bet_amount);

void shuffle_cards(std::vector<std::string>& game_cards);

void shuffle_players(std::vector<std::string>& names);

int set_player_position(std::vector<std::string>& game_names);


std::vector<Player> createPlayers(std::vector<std::string>& game_names, std::vector<Player>& players);


template<typename T>
void shuffle_function(std::vector<T>& map){
    
    std::random_device rd;
    std::mt19937 engine(rd());
    shuffle(map.begin(), map.end(), engine);
}

void shuffle_cards(std::vector<std::string>& cards);
void shuffle_players(std::vector<std::string>& players);


void show_my_cards(std::vector<Player> players);

void card_selection(std::vector<Player> players, std::vector<int> used_cards_index, std::vector<std::string> game_cards);

void community_cards_selection(std::vector<std::string>& game_cards, std::vector<int>& used_cards_index, std::vector<std::string>& community_cards);
}