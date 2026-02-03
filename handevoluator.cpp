#include "handevoluator.h"
#include <vector>




    int HandEvoluator::RoyalFlush(){
        return 7;
    }
    int HandEvoluator::StraightFlush(){
        return 6;
    }

    int HandEvoluator::FourOfKind(){
        return 5;
    }
    int HandEvoluator::FullHouse(){
        return 4;
    }
    int HandEvoluator::Straight(){
        return 3;
    }
    int HandEvoluator::ThreeOfKind(){
        return 2;
    }
    int HandEvoluator::TwoPairs(){
        return 2;
    }
    int HandEvoluator::OnePair(){
        
       return 1;
    }


    void HandEvoluator::evoluator(std::vector<int>& list){
        list.push_back(OnePair());
        list.push_back(TwoPairs());
        list.push_back(ThreeOfKind());
        list.push_back(Straight());
        list.push_back(FullHouse());
        list.push_back(FourOfKind());
        list.push_back(StraightFlush());
        list.push_back(RoyalFlush());

   


    }
    


