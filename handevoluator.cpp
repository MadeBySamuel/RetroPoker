#include "handevoluator.h"
#include <vector>




    int HandEvoluator::RoyalFlush(std::vector<Card>& cards){
        utils::sort_function(cards);
        
        for (int i = 0; i < cards.size() - 4; i++){
            if (cards[i].rank == 10 && cards[i+1].rank == 11 && cards[i+2].rank == 12 && cards[i+3].rank == 13 && cards[i+3].rank == 14){
            return 9;
            }
        }
        return 0;
    }
    int HandEvoluator::StraightFlush(std::vector<Card>& cards){
        utils::sort_function(cards);
        
        for (int i = 0; i < cards.size() - 4; i++){
            if (cards[i].rank == cards[i+1].rank && cards[i+1].rank == cards[i+2].rank && cards[i+2].rank == cards[i+3].rank && cards[i+3].rank == cards[i+4].rank){
                if (cards[i].name == cards[i+1].name && cards[i+1].name == cards[i+2].name && cards[i+2].name == cards[i+3].name && cards[i+3].name == cards[i+4].name){
                    return 8;
                }
            }
        }
        return 0;
    }

    int HandEvoluator::FourOfKind(std::vector<Card>& cards){
        utils::sort_function(cards);        
        
        for (int i = 0; i < cards.size() - 3; i++){
            if (cards[i].rank == cards[i+1].rank && cards[i+1].rank == cards[i+2].rank && cards[i+2].rank == cards[i+3].rank ){
                return 7;
            }
        }
        return 0;
    }
        
    int HandEvoluator::FullHouse(std::vector<Card>& cards){
        utils::sort_function(cards);

        for (int i = 0; i < cards.size() - 4; i++){
            if (cards[i].rank == cards[i+1].rank && cards[i+1].rank == cards[i+2].rank){
                if (cards[i+3].rank == cards[i+4].rank){
                    return 6;
                }
            }
            else if(cards[i+2].rank == cards[i+3].rank && cards[i+3].rank == cards[i+4].rank){
                return 6;
            }
    
        }
        return 0;
    }

    int HandEvoluator::Flush(std::vector<Card>& cards){
        utils::sort_name(cards);
        for (int i = 0; i < cards.size() - 4; i++){
            if (cards[i].name == cards[i+1].name && cards[i+1].name == cards[i+2].name && cards[i+2].name == cards[i+3].name && cards[i+3].name == cards[i+4].name){
                return 5;
            }
        }
        return 0;
    }
    int HandEvoluator::Straight(std::vector<Card>& cards){
        utils::sort_function(cards);
        
        for (int i = 0; i < cards.size() - 4; i++){
            if (cards[i].rank == (--cards[i+1].rank)  && cards[i+1].rank == (--cards[i+2].rank) && cards[i+3].rank == (--cards[i+3].rank) && cards[i+3].rank == (--cards[i+4].rank))
            return 4;
        }
        return 0;
    }
    int HandEvoluator::ThreeOfKind(std::vector<Card>& cards){
        utils::sort_function(cards);        
        
        for (int i = 0; i < cards.size() - 2; i++){
            if (cards[i].rank == cards[i+1].rank && cards[i+1].rank == cards[i+2].rank){
                return 3;
            }
        }
        return 0;
    }
    int HandEvoluator::TwoPairs(std::vector<Card>& cards){
        utils::sort_function(cards);
        int counter = 0;

        for(int i =0; i <cards.size() - 1; i ++){
            if(cards[i].rank == cards[i+1].rank)
            counter++;
        }
        if (counter == 2) return 2;
        return 0;
    }
    int HandEvoluator::OnePair(std::vector<Card>& cards){

       utils::sort_function(cards);

        for (int i = 0; i < cards.size() - 1; i++){
            if(cards[i].rank == cards[i+1].rank){
                return 1;
            }
        }

       return 0;
    }


    std::vector<int> HandEvoluator::evoluator(std::vector<Card> &cards){
        
        std::vector<int> list; 

        list.emplace_back(HandEvoluator::OnePair(cards));
        list.emplace_back(HandEvoluator::TwoPairs(cards));
        list.emplace_back(HandEvoluator::ThreeOfKind(cards));
        list.emplace_back(HandEvoluator::Straight(cards));
        list.emplace_back(HandEvoluator::Flush(cards));
        list.emplace_back(HandEvoluator::FullHouse(cards));
        list.emplace_back(HandEvoluator::FourOfKind(cards));
        list.emplace_back(HandEvoluator::StraightFlush(cards));
        list.emplace_back(HandEvoluator::RoyalFlush(cards));

        return list;
    }
    


