#pragma once

#include "player.h"
#include "constants.h"

#include <memory>




namespace utils{

void sort_function(std::vector<Card> cards);
void sort_name(std::vector<Card> cards);

bool is_valid(int money, int bet_amount);

void shuffle_cards(std::vector<Card>& game_cards);

void shuffle_players(std::vector<std::string>& names);

int set_player_position(std::vector<std::string>& game_names);


std::vector<std::unique_ptr<Player>>& createPlayers(std::vector<std::string>& game_names, std::vector<std::unique_ptr<Player>>& players);


template<typename T>
void shuffle_function(std::vector<T>& map){
    
    std::random_device rd;
    std::mt19937 engine(rd());
    shuffle(map.begin(), map.end(), engine);
}

void shuffle_cards(std::vector<Card>& cards);
void shuffle_players(std::vector<std::string>& players);


void show_my_cards(std::vector<std::unique_ptr<Player>>& players);

void card_selection(std::vector<std::unique_ptr<Player>>& players, std::vector<int>& used_cards_index, std::vector<Card>& game_cards);

void community_cards_selection(std::vector<Card>& game_cards, std::vector<int>& used_cards_index, std::vector<Card>& community_cards);

void blindSelection(std::vector<std::unique_ptr<Player>>& players, int& bigBlindPosition, int& smallBlindPositon);

std::vector<Card> cards7collection(std::vector<std::unique_ptr<Player>>& players, std::vector<Card>& community_cards);


}