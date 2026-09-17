#pragma once
#include "player.hpp"
#include "utils.hpp"


enum class GamePhase { PRE_FLOP, FLOP, TURN, RIVER, SHOWDOWN };
enum class ActionType { FOLD, CHECK, CALL, RAISE, ALLIN };

class Game {

    private:
        int pot = 0;
        int currentBet = 0;
        int smallBlindAmount = 10;
        int bigBlindAmount = 0;
        int numberOfPlayers = 2;

        std::vector<std::unique_ptr<Player>> players;
        std::vector<Card> communityCards;
        std::string winnerMessage;
        GamePhase currentPhase = GamePhase::PRE_FLOP;
        int smallBlindPosition;
        int bigBlindPosition;
        int dealerPosition;
        



 

    public:

        GamePhase getPhase() const;
        int& getPot();
        void setPot(int pot);

        void setBigBlind(int bigBlind);
        int getBigBlind();
        ActionType botChoice(std::size_t playerTurn);

        int getNumberOfPlayers();
        void setNumberOfPlayers(int numberOfPlayers); 
        int getSmallBlind();
        void setSmallBlind(int smallBlind);

        int getCurrentBet();

        void setCurrentBet(int currentBet);
        std::vector<std::unique_ptr<Player>>& getPlayers();
        const std::string toStringActionType(ActionType type);
        

        void setup_once(std::vector<std::unique_ptr<Player>>& players);

        void setup_once_per_game(std::vector<std::unique_ptr<Player>>& players, std::vector<Card>& game_cards, 
        std::vector<int>& used_cards_index, std::vector<Card>& community_cards);
        
        void newHand();
        void game();
        ActionType playerAction(ActionType action, int raiseAmount = 0);
        void advancePhase();

        const std::vector<Card>& getCommunityCards() const;
        std::vector<Card> getVisibleCommunityCards() const;
        const std::string& getWinnerMessage() const;
        int raiseAmount = 0;

    
};
