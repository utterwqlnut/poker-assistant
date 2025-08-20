#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include "hands.hpp"

bool Card::operator>(const Card& cardB) const {
    return ordering.at(label)>ordering.at(cardB.label);
}

bool Card::operator<(const Card& cardB) const {
    return ordering.at(label)<ordering.at(cardB.label);
}

bool Card::operator==(const Card& cardB) const {
    return ordering.at(label)==ordering.at(cardB.label);
}


const std::map<std::string,int> Card::ordering = {
    {"2", 2},
    {"3", 3},
    {"4", 4},
    {"5", 5},
    {"6", 6},
    {"7", 7},
    {"8", 8},
    {"9", 9},
    {"T", 10},
    {"J", 11},
    {"Q", 12},
    {"K", 13},
    {"A", 14},
    {"A0", 1},
};



bool Condition::operator>(const Condition& other) const {
    if (ordering.at(label) != ordering.at(other.label))
        return ordering.at(label) > ordering.at(other.label);
    // Compare highest cards if hand type is equal
    for (int i = important_cards.size() - 1, j = other.important_cards.size() - 1; i >= 0 && j >= 0; --i, --j) {
        if (!(important_cards[i] == other.important_cards[j]))
            return important_cards[i] > other.important_cards[j];
    }
    return false;
}

bool Condition::operator<(const Condition& other) const {
    return other > *this;
}

bool Condition::operator==(const Condition& other) const {
    if (label != other.label) return false;
    if (important_cards.size() != other.important_cards.size()) return false;
    for (size_t i = 0; i < important_cards.size(); i++) {
        if (!(important_cards[i] == other.important_cards[i])) return false;
    }
    return true;
}

// Initialize ordering map
const std::map<std::string,int> Condition::ordering = {
    {"high-card", 1},
    {"pair", 2},
    {"two-pair", 3},
    {"trips", 4},
    {"straight", 5},
    {"flush", 6},
    {"full-house", 7},
    {"quad", 8},
    {"straight-flush", 9}
};

Hands::Hands(std::vector<Card> cardsIn) {
    this->cards = cardsIn;
    std::sort(cards.begin(),cards.end());

    std::vector<Card> important_cards;

    // First check Straight-Flush
    important_cards = check_straight_flush(cards);
    if(important_cards.size()!=0) {
        this->condition.label = "straight-flush";
        this->condition.important_cards = important_cards;
        return;
    }

    // Second check 4 of a kind
    important_cards = check_quad(cards);
    if(important_cards.size()!=0) {
        this->condition.label = "quad";
        this->condition.important_cards = important_cards;

        return;
    }
    
    // Third check full house
    important_cards = check_full_house(cards);
    if(important_cards.size()!=0) {
        this->condition.label = "full-house";
        this->condition.important_cards = important_cards;

        return;
    }

    // Fourth check flush
    important_cards = check_flush(cards);
    if(important_cards.size()!=0) {
        this->condition.label = "flush";
        this->condition.important_cards = important_cards;
        
        return;
    }

    // Fifth check straight
    important_cards = check_straight(cards);
    if(important_cards.size()!=0) {
        this->condition.label = "straight";
        this->condition.important_cards = important_cards;
        
        return;
    }

    // Sixth check trips
    important_cards = check_trips(cards);
    if(important_cards.size()!=0) {
        this->condition.label = "trips";
        this->condition.important_cards = important_cards;
        
        return;
    }

    // Seventh check Two Pair
    important_cards = check_2pair(cards);
    if(important_cards.size()!=0) {
        this->condition.label = "two-pair";
        this->condition.important_cards = important_cards;
        
        return;
    }

    // Eigth check Pair
    important_cards = check_pair(cards);
    if(important_cards.size()!=0) {
        this->condition.label = "pair";
        this->condition.important_cards = important_cards;
        
        return;
    }

    // High Card
    important_cards = cards;
    if(important_cards.size()!=0) {
        this->condition.label = "high-card";
        this->condition.important_cards = important_cards;
        
        return;
    }
}

std::vector<Card> Hands::check_pair(std::vector<Card> cards) {
    // Signifies a "null" card
    std::vector<Card> highest_pair;
    for(int i=1;i<cards.size();i++) {
        if(cards[i]==cards[i-1]) {
            highest_pair = {cards[i]};
        }
    }

    return highest_pair;
}

std::vector<Card> Hands::check_2pair(std::vector<Card> cards) {
    std::vector<Card> two_pairs;
    int pair_count = 0;
    for(int i=1;i<cards.size();i++) {
        if(cards[i]==cards[i-1]) {
            two_pairs.push_back(cards[i]);
            pair_count+=1;
        }
    }
    std::sort(two_pairs.begin(),two_pairs.end());
    

    if(pair_count>=2) {
        std::vector<Card> slice(two_pairs.end()-2,two_pairs.end());
        return slice;
    } else {
        return std::vector<Card>();
    }
}

std::vector<Card> Hands::check_trips(std::vector<Card> cards) {
    // Signifies a "null" card
    std::vector<Card> highest_trips;
    for(int i=2;i<cards.size();i++) {
        if(cards[i]==cards[i-1]&&cards[i]==cards[i-2]) {
            highest_trips = {cards[i]};
        }
    }

    return highest_trips;

}

std::vector<Card> Hands::check_quad(std::vector<Card> cards) {
    // Signifies a "null" card
    std::vector<Card> highest_quad;
    for(int i=3;i<cards.size();i++) {
        if(cards[i]==cards[i-1]&&cards[i]==cards[i-2]&&cards[i]==cards[i-3]) {
            highest_quad = {cards[i]};
        }
    }

    return highest_quad;

}

std::vector<Card> Hands::check_straight_help(std::vector<Card> cards) {
    std::vector<Card> highest_straight;
    std::vector<Card> curr_straight;

    int count = 0;
    int max_count = 0;
    for(int i=1;i<cards.size();i++) {
        if(Card::ordering.at(cards[i].label)==Card::ordering.at(cards[i-1].label)+1) {
            if(count==0) {
                curr_straight.push_back(cards[i-1]);
            }
            curr_straight.push_back(cards[i]);
            count+=1;
        } else if(Card::ordering.at(cards[i].label)==Card::ordering.at(cards[i-1].label)) {
            continue;
        } 
        else {
            count = 0;
            if (curr_straight.size()>=5) {
                highest_straight = curr_straight;
            }
            curr_straight.clear();
        }
    }
    if(curr_straight.size()>=5) {
        std::vector<Card> slice(curr_straight.end()-5,curr_straight.end());
        return slice;
    } else if(highest_straight.size()>=5){
        std::vector<Card> slice(highest_straight.end()-5,highest_straight.end());
        return slice;
    } else {
        return std::vector<Card>();
    }
}

std::vector<Card> Hands::check_straight(std::vector<Card> cards) {
    std::vector<Card> result;
    result = check_straight_help(cards);
    if(result.size()!=0) {
        return result;
    } else {
        for(int i=0;i<cards.size();i++) {
            if(cards[i].label=="A") {
                cards[i].label = "A0";
            }
        }
        std::sort(cards.begin(),cards.end());
        result = check_straight_help(cards);
        return result;
    }
}

std::vector<Card> Hands::check_flush(std::vector<Card> cards) {
    std::vector<Card> spade;
    std::vector<Card> diamond;
    std::vector<Card> heart;
    std::vector<Card> club;


    for(int i=0;i<cards.size();i++) {
        if(cards[i].suite=="spade") {
            spade.push_back(cards[i]);
        } else if (cards[i].suite=="diamond") {
            diamond.push_back(cards[i]);
        } else if (cards[i].suite=="heart") {
            heart.push_back(cards[i]);
        } else {
            club.push_back(cards[i]);
        }
    }

    if(spade.size()>=5) {
        std::vector<Card> slice(spade.end()-5,spade.end());
        return slice;
    } else if(heart.size()>=5) {
        std::vector<Card> slice(heart.end()-5,heart.end());
        return slice;
    } else if(diamond.size()>=5) {
        std::vector<Card> slice(diamond.end()-5,diamond.end());
        return slice;
    } else if(club.size()>=5) {
        std::vector<Card> slice(club.end()-5,club.end());
        return slice;
    } else {
        return std::vector<Card>();
    }
}

std::vector<Card> Hands::check_straight_flush(std::vector<Card> cards) {
    std::vector<Card> spade;
    std::vector<Card> diamond;
    std::vector<Card> heart;
    std::vector<Card> club;


    for(int i=0;i<cards.size();i++) {
        if(cards[i].suite=="spade") {
            spade.push_back(cards[i]);
        } else if (cards[i].suite=="diamond") {
            diamond.push_back(cards[i]);
        } else if (cards[i].suite=="heart") {
            heart.push_back(cards[i]);
        } else {
            club.push_back(cards[i]);
        }
    }

    if(spade.size()>=5) {
        return check_straight(spade);
    } else if(heart.size()>=5) {
        return check_straight(heart);
    } else if(diamond.size()>=5) {
        return check_straight(diamond);
    } else if(club.size()>=5) {
        return check_straight(club);
    } else {
        return std::vector<Card>();
    }

}

std::vector<Card> Hands::check_full_house(std::vector<Card> cards) {
    std::vector<Card> trips;
    trips = check_trips(cards);
    if(trips.size()==0) {
        return trips;
    } else {
        std::vector<Card> highest_pair;
        for(int i=1;i<cards.size();i++) {
            if(cards[i]==cards[i-1] && !(cards[i]==trips[0])) {
                highest_pair = {cards[i]};
            }
        }
        std::vector<Card> result;
        result.push_back(highest_pair[0]);
        result.push_back(trips[0]);
        std::sort(result.begin(),result.end());

        return result;
    }
}
bool Hands::operator>(const Hands& handB) const {
    if(this->condition > handB.condition) {
        return true;
    } else if(this->condition == handB.condition){
        if (this->cards[this->cards.size()-1] > handB.cards[handB.cards.size()-1]) {
            return true;
        }
    }
    return false;
}

bool Hands::operator<(const Hands& handB) const {
    if(this->condition < handB.condition) {
        return true;
    } else if(this->condition == handB.condition){
        if (this->cards[this->cards.size()-1] < handB.cards[handB.cards.size()-1]) {
            return true;
        }
    }
    return false;
}

bool Hands::operator==(const Hands& handB) const {
    if(this->condition == handB.condition) {
        if (this->cards[this->cards.size()-1] == handB.cards[handB.cards.size()-1]) {
            return true;
        }
    }
    return false;
}