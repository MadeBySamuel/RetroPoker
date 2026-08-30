#pragma once
#include "player.hpp"
#include "utils.hpp"


enum class GamePhase { PRE_FLOP, FLOP, TURN, RIVER, SHOWDOWN };
enum class ActionType { FOLD, CHECK, CALL, RAISE, ALLIN };

class Game {

    private:
        int pot = 0;
        int currentBet = 0;
        int smallBlind = 0;
        int bigBlind = 0;
        std::vector<std::unique_ptr<Player>> players;
        std::vector<Card> communityCards;
        std::string winnerMessage;
        GamePhase currentPhase = GamePhase::PRE_FLOP;
        int smallBlindPosition;
        int bigBlindPosition;

 

    public:

        GamePhase getPhase() const;
        int& getPot();
        void setPot(int pot);

        void setBigBlind(int bigBlind);
        int getBigBlind();
        

        int getSmallBlind();
        void setSmallBlind(int smallBlind);

        int getCurrentBet();

        void setCurrentBet(int currentBet);
        std::vector<std::unique_ptr<Player>>& getPlayers();
        

    void setup_once(std::vector<std::unique_ptr<Player>>& players);

    void setup_once_per_game(std::vector<std::unique_ptr<Player>>& players, std::vector<Card>& game_cards, 
        std::vector<int>& used_cards_index, std::vector<Card>& community_cards);
    
    void newHand();
    void game();
    void playerAction(ActionType action, int raiseAmount = 0);

    const std::vector<Card>& getCommunityCards() const;
    std::vector<Card> getVisibleCommunityCards() const;
    const std::string& getWinnerMessage() const;
    
};


