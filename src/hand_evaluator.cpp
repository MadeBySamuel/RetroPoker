#include <vector>

#include "hand_evaluator.hpp"
#include "player.hpp"
#include "utils.hpp"



    int HandEvaluator::RoyalFlush(std::vector<Card>& cards, int& sum){
        utils::sort_function(cards);
        
        for (int i = 0; i < static_cast<int>(cards.size()) - 4; i++){
            if (cards[i].rank == 10 && cards[i+1].rank == 11 && cards[i+2].rank == 12 && cards[i+3].rank == 13 && cards[i+4].rank == 14 &&
                cards[i].name == cards[i+1].name && cards[i+1].name == cards[i+2].name && cards[i+2].name == cards[i+3].name && cards[i+3].name == cards[i+4].name){
            
            sum = 0;    
            return 9;
            }
        }
        return 0;
    }

        
    int HandEvaluator::StraightFlush(std::vector<Card>& cards, int& sum){
        utils::sort_function(cards);
        
        for (int i = 0; i < static_cast<int>(cards.size()) - 4; i++){
            if (cards[i].rank == (cards[i+1].rank - 1)  && cards[i+1].rank == (cards[i+2].rank - 2) && cards[i+2].rank == (cards[i+3].rank -3) && cards[i+3].rank == (cards[i+4].rank - 4)){
                if (cards[i].name == cards[i+1].name && cards[i+1].name == cards[i+2].name && cards[i+2].name == cards[i+3].name && cards[i+3].name == cards[i+4].name){
                    sum = cards[i+4].rank;
                    return 8;
                }
            }
        }
        return 0;
    }

    int HandEvaluator::FourOfKind(std::vector<Card>& cards, int& sum){
        utils::sort_function(cards);        
        
        for (int i = 0; i < static_cast<int>(cards.size()) - 3; i++){
            if (cards[i].rank == cards[i+1].rank && cards[i+1].rank == cards[i+2].rank && cards[i+2].rank == cards[i+3].rank ){
                sum = cards[i].rank;
                return 7;
            }
        }
        return 0;
    }
        
    int HandEvaluator::FullHouse(std::vector<Card>& cards, int& sum){
        sum = 0; 


        utils::sort_function(cards);

        for (int i = 0; i < static_cast<int>(cards.size()) - 4; i++){
            if (cards[i].rank == cards[i+1].rank && cards[i+1].rank == cards[i+2].rank){
                sum = cards[i].rank;
                if(cards[i+3].rank == cards[i+4].rank){
                    return 6;
                }
            }
            else if(cards[i+2].rank == cards[i+3].rank && cards[i+3].rank == cards[i+4].rank){
                sum = cards[i].rank;
                return 6;
            }
    
        }
        return 0;
    }

    int HandEvaluator::Flush(std::vector<Card>& cards, int& sum){
        sum = 0;
        utils::sort_name(cards);
        for (int i = 0; i < static_cast<int>(cards.size()) - 4; i++){
            if (cards[i].name == cards[i+1].name && cards[i+1].name == cards[i+2].name && cards[i+2].name == cards[i+3].name && cards[i+3].name == cards[i+4].name){
                sum = cards[i+4].rank;
                return 5;
            }
        }
        return 0;
    }
    int HandEvaluator::Straight(std::vector<Card>& cards, int& sum){
        sum = 0;

        utils::sort_function(cards);
        
        for (int i = 0; i < static_cast<int>(cards.size()) - 4; i++){
            if (cards[i].rank + 1 == cards[i+1].rank && cards[i+1].rank + 1 == cards[i+2].rank && cards[i+2].rank + 1 == cards[i+3].rank &&cards[i+3].rank + 1 == cards[i+4].rank)
            {
                sum = (cards[i+4].rank + 4); 
                return 4;
        }
    }
        return 0;
}

    int HandEvaluator::ThreeOfKind(std::vector<Card>& cards, int& sum){

        sum = 0;
        utils::sort_function(cards);        
        
        for (int i = 0; i < static_cast<int>(cards.size()) - 2; i++){
            if (cards[i].rank == cards[i+1].rank && cards[i+1].rank == cards[i+2].rank){
                sum = cards[i].rank + cards[i+1].rank + cards[i+2].rank;
                return 3;
            }
        }
        return 0;
    }
    int HandEvaluator::TwoPairs(std::vector<Card>& cards, int& sum){
        
        sum = 0;
        utils::sort_function(cards);
        int counter = 0;

        std::vector<Card> highpaircomp;

        for(int i =0; i < static_cast<int>(cards.size()) - 1; i ++){
            if(cards[i].rank == cards[i+1].rank)
            
            highpaircomp.push_back(cards[i]);
            highpaircomp.push_back(cards[i+1]);
            counter++;
        }
        if (counter == 2) {
            for(int i = 0; i < static_cast<int>(highpaircomp.size()); i++){
                if(i == static_cast<int>(highpaircomp.size()) - 1){
                    sum = highpaircomp[i].rank;
                }
            }
            return 2;
        }
        return 0;
    }
    int HandEvaluator::OnePair(std::vector<Card>& cards, int& sum){
        sum = 0;

       utils::sort_function(cards);

        for (int i = 0; i < static_cast<int>(cards.size()) - 1; i++){
            if(cards[i].rank == cards[i+1].rank){
                sum = cards[i].rank;
                return 1;
            }
        }

       return 0;
    }


    HandScore HandEvaluator::evaluate(std::vector<Card> &cards, std::vector<std::unique_ptr<Player>>& players, std::vector<Card> community_cards, int i){
    
    int sum = 0;

    std::vector<HandScore> list;

    if(players[i]->getInGame()){
 

    list.emplace_back(HandScore{HandEvaluator::OnePair(cards, sum), sum, players[i]->getName()});
    list.emplace_back(HandScore{HandEvaluator::TwoPairs(cards, sum), sum,  players[i]->getName()});
    list.emplace_back(HandScore{HandEvaluator::ThreeOfKind(cards, sum), sum,  players[i]->getName()});
    list.emplace_back(HandScore{HandEvaluator::Straight(cards, sum), sum,  players[i]->getName()});
    list.emplace_back(HandScore{HandEvaluator::Flush(cards,sum ), sum,  players[i]->getName()});
    list.emplace_back(HandScore{HandEvaluator::FullHouse(cards,sum), sum,  players[i]->getName()});
    list.emplace_back(HandScore{HandEvaluator::FourOfKind(cards, sum),sum,  players[i]->getName()});
    list.emplace_back(HandScore{HandEvaluator::StraightFlush(cards,sum),sum,  players[i]->getName()});
    list.emplace_back(HandScore{HandEvaluator::RoyalFlush(cards,sum),sum,  players[i]->getName()});

    
    }
    HandScore best_hand = utils::max(list);  
    return best_hand;
    
}


