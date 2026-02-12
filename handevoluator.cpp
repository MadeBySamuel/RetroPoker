#include "handevoluator.h"
#include <vector>






    int HandEvoluator::RoyalFlush(std::vector<Card>& cards, int& sum){
        utils::sort_function(cards);
        
        for (int i = 0; i < cards.size() - 4; i++){
            if (cards[i].rank == 10 && cards[i+1].rank == 11 && cards[i+2].rank == 12 && cards[i+3].rank == 13 && cards[i+3].rank == 14){
            
            sum = 0;    
            return 9;
            }
        }
        return 0;
    }
    
    int HandEvoluator::StraightFlush(std::vector<Card>& cards, int& sum){
        utils::sort_function(cards);
        
        for (int i = 0; i < cards.size() - 4; i++){
            if (cards[i].rank == (cards[i+1].rank - 1)  && cards[i+1].rank == (cards[i+2].rank - 2) && cards[i+2].rank == (cards[i+3].rank -3) && cards[i+3].rank == (cards[i+4].rank - 4)){
                if (cards[i].name == cards[i+1].name && cards[i+1].name == cards[i+2].name && cards[i+2].name == cards[i+3].name && cards[i+3].name == cards[i+4].name){
                    sum = cards[i+4].rank;
                    return 8;
                }
            }
        }
        return 0;
    }

    int HandEvoluator::FourOfKind(std::vector<Card>& cards, int& sum){
        utils::sort_function(cards);        
        
        for (int i = 0; i < cards.size() - 3; i++){
            if (cards[i].rank == cards[i+1].rank && cards[i+1].rank == cards[i+2].rank && cards[i+2].rank == cards[i+3].rank ){
                sum = cards[i].rank;
                return 7;
            }
        }
        return 0;
    }
        
    int HandEvoluator::FullHouse(std::vector<Card>& cards, int& sum){
        sum = 0; 


        utils::sort_function(cards);

        for (int i = 0; i < cards.size() - 4; i++){
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

    int HandEvoluator::Flush(std::vector<Card>& cards, int& sum){
        sum = 0;
        utils::sort_name(cards);
        for (int i = 0; i < cards.size() - 4; i++){
            if (cards[i].name == cards[i+1].name && cards[i+1].name == cards[i+2].name && cards[i+2].name == cards[i+3].name && cards[i+3].name == cards[i+4].name){
                sum = cards[i+4].rank;
                return 5;
            }
        }
        return 0;
    }
    int HandEvoluator::Straight(std::vector<Card>& cards, int& sum){
        sum = 0;

        utils::sort_function(cards);
        
        for (int i = 0; i < cards.size() - 4; i++){
            if (cards[i].rank == (cards[i+1].rank - 1)  && cards[i+1].rank == (cards[i+2].rank - 2) && cards[i+2].rank == (cards[i+3].rank -3) && cards[i+3].rank == (cards[i+4].rank - 4))
                sum = (cards[i+4].rank + 4); 
                return 4;
        }
        return 0;
    }
    int HandEvoluator::ThreeOfKind(std::vector<Card>& cards, int& sum){

        sum = 0;
        utils::sort_function(cards);        
        
        for (int i = 0; i < cards.size() - 2; i++){
            if (cards[i].rank == cards[i+1].rank && cards[i+1].rank == cards[i+2].rank){
                sum = cards[i].rank + cards[i+1].rank + cards[i+2].rank;
                return 3;
            }
        }
        return 0;
    }
    int HandEvoluator::TwoPairs(std::vector<Card>& cards, int& sum){
        
        sum = 0;
        utils::sort_function(cards);
        int counter = 0;

        std::vector<Card> highpaircomp;

        for(int i =0; i <cards.size() - 1; i ++){
            if(cards[i].rank == cards[i+1].rank)
            
            highpaircomp.push_back(cards[i]);
            highpaircomp.push_back(cards[i+1]);
            counter++;
        }
        if (counter == 2) {
            for(int i = 0; i < highpaircomp.size(); i++){
                if(i == highpaircomp.size() - 1){
                    sum = highpaircomp[i].rank;
                }
            }
            return 2;
        }
        return 0;
    }
    int HandEvoluator::OnePair(std::vector<Card>& cards, int& sum){
        sum = 0;

       utils::sort_function(cards);

        for (int i = 0; i < cards.size() - 1; i++){
            if(cards[i].rank == cards[i+1].rank){
                sum = cards[i].rank;
                return 1;
            }
        }

       return 0;
    }


    void HandEvoluator::evoluator(std::vector<Card> &cards, std::vector<std::unique_ptr<Player>>& players, std::vector<Card> community_cards){
    
    std::vector<HandScore> game_evoluation;
    int sum = 0;

    std::vector<std::vector<Card>> all_cards;

    for (int i = 0; i < players.size(); i++)
    {
        cards = utils::cards7collection(players, i, community_cards);

        if(players[i]->getInGame()){
        
        std::vector<HandScore> list;

        list.emplace_back(HandScore{HandEvoluator::OnePair(cards, sum), sum});
        list.emplace_back(HandScore{HandEvoluator::TwoPairs(cards, sum), sum});
        list.emplace_back(HandScore{HandEvoluator::ThreeOfKind(cards, sum), sum});
        list.emplace_back(HandScore{HandEvoluator::Straight(cards, sum), sum});
        list.emplace_back(HandScore{HandEvoluator::Flush(cards,sum ), sum});
        list.emplace_back(HandScore{HandEvoluator::FullHouse(cards,sum), sum});
        list.emplace_back(HandScore{HandEvoluator::FourOfKind(cards, sum),sum});
        list.emplace_back(HandScore{HandEvoluator::StraightFlush(cards,sum),sum});
        list.emplace_back(HandScore{HandEvoluator::RoyalFlush(cards,sum),sum});



        int max_element = 0;
        int max_element_of_person = 0;

        
        HandScore best_hand = utils::max_if_tie(list);  
    
        game_evoluation.push_back(best_hand);

        }
    }

    utils::sort_function_hand_score(game_evoluation);


    
    for (size_t i = 0; i < players.size(); i ++){
        if (game_evoluation[i].rank == game_evoluation[i+1].rank){
            utils::sort_function_hand_score(game_evoluation);
            for (size_t j = 0; j < game_evoluation.size(); j++){
                HandScore hand = utils::max_if_tie(game_evoluation);
            }    
        }
    }
    }
    


