#include "player.hpp"
#include "constants.hpp"

#include "utils.hpp"
#include "game.hpp"
#include <memory>


void utils::sort_function(std::vector<Card>& cards){
    std::sort(cards.begin(), cards.end(),[](const Card& a,const Card& b){
            return a.rank < b.rank;
    });
    
}

void utils::sort_function_hand_score(std::vector<HandScore>& hand_cards){
    std::sort(hand_cards.begin(), hand_cards.end(),[](const HandScore& a,const HandScore& b){
            return a.sum_for_tie < b.sum_for_tie;
    });
}

void utils::sort_function_rank(std::vector<HandScore>& hand_cards){
    std::sort(hand_cards.begin(), hand_cards.end(),[](const HandScore& a,const HandScore& b){
            return a.rank > b.rank;
    });
}


HandScore utils::max(std::vector<HandScore>& cards){
    auto max_it = std::max_element(cards.begin(), cards.end(),[](const HandScore &a, const HandScore &b) {
        return a.sum_for_tie < b.sum_for_tie;  // vracia true, ak a < b
    
    });

    return *max_it;
}


void utils::sort_name(std::vector<Card> cards){
    std::sort(cards.begin(), cards.end(),[](Card& a, Card& b){
            return a.name < b.name;
    });
    }


bool utils::is_valid(int money, int bet_amount){
    if (bet_amount > money) {
        std:: cout << "You cant bet bigger amount than your money " << std::endl;
        return false;
    }
    else if (bet_amount < 0){
        std::cout << "You cant bet negative amount of money " << std::endl;
        return false;
    }
    else if(bet_amount == 0){
        std::cout << "Your bet must me more than 0" << std::endl;
        return false;
    }
    else {
        std::cout << "Your bet: " << bet_amount << std::endl;
        return true;
    }
}


void utils::shuffle_cards(std::vector<Card>& cards){
    if (cards.empty ()) {
        return;
    }
    else{
        utils::shuffle_function(cards);
    } 
}

void utils::shuffle_players(std::vector<std::string>& names){
    utils::shuffle_function(names);
}

int utils::set_player_position(){
    
    std::vector<std::string> game_names = Constants::NAMES;

    if (game_names.empty()) {
        return -1;
    }

    int player_position = 0;
    game_names[player_position] = "Player";
    
    return player_position;
}


std::vector<std::unique_ptr<Player>>& utils::createPlayers(std::vector<std::unique_ptr<Player>>& players, int numberOfPlayers){
    players.clear();
    std::vector<std::string> game_names = Constants::NAMES;
    const int safePlayerCount = std::clamp(numberOfPlayers, 2, static_cast<int>(game_names.size()));
    players.reserve(safePlayerCount);


    for (int i = 0; i < safePlayerCount; i++) {
        if (i == 0){
            players.push_back(std::make_unique<Player>("Player", 200, true));
        }
        else {
            players.push_back(std::make_unique<Player>(game_names[static_cast<std::size_t>(i)], 200, false));
        }

    }

    return players;

}


void utils::show_my_cards(std::vector<std::unique_ptr<Player>>& players){
for(size_t i = 0; i < players.size(); i++){
        if(players[i]->getName() == "Player"){
            for (size_t j = 0; j < players[i]->getcard2().size(); j++){
                std::cout << players[i]->getcard2()[j].write_name() << std::endl;  
            }
        }
    } 
}

void utils::settingInGameStatus(std::vector<std::unique_ptr<Player>>& players){
    
    for(size_t i = 0; i < players.size(); i++){
        auto& player = players[i];
        player->setBetThisRound(0);
        player->resetCommittedThisHand();
        player->setActed(false);
        player->setAllIn(false);

        if (player->getMoney() <= 0 && player->isBot()){
            player->setInGame(false);
            player->setEliminated();
        }
        else {
            players[i]->setInGame(true);
        }
    }
}


void utils::card_selection(std::vector<std::unique_ptr<Player>>& players, std::vector<int>& used_cards_index, std::vector<Card>& game_cards){
    std::size_t nextCard = 0;

    for (auto& player: players){
        player->getcard2().clear();
        if (player->getIsEliminated() || nextCard + 2 > game_cards.size()) {
            continue;
        }

        used_cards_index.push_back(static_cast<int>(nextCard));
        player->getcard2().push_back(game_cards[nextCard++]);
        used_cards_index.push_back(static_cast<int>(nextCard));
        player->getcard2().push_back(game_cards[nextCard++]);
    }
}


void utils::community_cards_selection(std::vector<Card>& game_cards, std::vector<int>& used_cards_index, std::vector<Card>& community_cards){
    community_cards.clear();

    std::size_t nextCard = used_cards_index.size();
    while (community_cards.size() < 5 && nextCard < game_cards.size()) {
        used_cards_index.push_back(static_cast<int>(nextCard));
        community_cards.push_back(game_cards[nextCard++]);
    }
}

void utils::dealerSelection(std::vector<std::unique_ptr<Player>>& players, int& dealerPosition){
    if (players.empty()){
        return;
    }
    
    for (auto& player : players){
        if (player->getIsDealer()){
            player->ToggleDealer();
        }
    }
    for (std::size_t offset = 0; offset < players.size(); ++offset) {
        const int index = (dealerPosition + static_cast<int>(offset)) % static_cast<int>(players.size());
        if (!players[index]->getIsEliminated()) {
            dealerPosition = index;
            players[index]->ToggleDealer();
            return;
        }
    }
}

void utils::blindSelection(std::vector<std::unique_ptr<Player>>& players, int& bigBlindPosition, int& smallBlindPositon){
    if (players.empty()) {
        return;
    }

    for (auto& player : players) {
        if (player->getIsBigBlind()) {
            player->ToggleBigBlind();
        }
        if (player->getIsSmallBlind()) {
            player->ToggleSmallBlind();
        }
    }

    const auto findActive = [&players](int start, int excluded) {
        const int playerCount = static_cast<int>(players.size());
        for (int offset = 0; offset < playerCount; ++offset) {
            const int index = (start + offset) % playerCount;
            if (index != excluded && !players[index]->getIsEliminated()) {
                return index;
            }
        }
        return -1;
    };

    smallBlindPositon = findActive(smallBlindPositon, -1);
    bigBlindPosition = findActive(bigBlindPosition, smallBlindPositon);

    if (smallBlindPositon != -1) {
        players[smallBlindPositon]->ToggleSmallBlind();
    }
    if (bigBlindPosition != -1) {
        players[bigBlindPosition]->ToggleBigBlind();
    }
}

std::vector<Card> utils::cards7collection(std::vector<std::unique_ptr<Player>>& players, int i, const std::vector<Card>& community_cards){
    
    std::vector<Card> map;

    
    for (size_t j = 0; j < players[i]->getcard2().size(); j++){
            map.push_back(players[i]->getcard2()[j]);
    }

    for (size_t i = 0; i < community_cards.size(); i ++){
            map.push_back(community_cards[i]);
    }
    return map;
}
