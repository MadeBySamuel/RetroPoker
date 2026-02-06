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


    void HandEvoluator::evoluator(std::vector<Card> &cards, std::vector<std::unique_ptr<Player>>& players, std::vector<Card> community_cards){
        
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

    std::vector<int> game_evoluation; 
    std::vector<int> map_of_plays(players.size());
    
    int max_element = 0;


    for (int i = 0; i < players.size(); i ++)
    {
        if (players[i]->getInGame()){
        std::vector<Card> map;
        int max_element_of_person = 0;



        map = utils::cards7collection(players, i, community_cards);
        map_of_plays = handEvo.evoluator(map);

    
        max_element_of_person = *std::max_element(map_of_plays.begin(), map_of_plays.end());      // toto vracia iterator čiže * <- need 
        game_evoluation.push_back(max_element_of_person);
        }
    }
    auto iterator = std::max_element(game_evoluation.begin(), game_evoluation.end());

    int winner = *iterator;

    int winnerIndex = std::distance(game_evoluation.begin(), iterator);
    }
    


