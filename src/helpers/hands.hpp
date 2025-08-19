#include <vector>
#include <map>

#ifndef HANDS_HPP
#define HANDS_HPP

typedef struct Card {
    static const std::map<std::string,int> ordering;

    std::string label;
    std::string suite;

    bool operator>(const Card& cardB) const;

    bool operator<(const Card& cardB) const;

    bool operator==(const Card& cardB) const;

} Card;

typedef struct Condition {
    static const std::map<std::string, int> ordering;

    std::string label;
    std::vector<Card> important_cards;

    bool operator>(const Condition& other) const;

    bool operator<(const Condition& other) const;

    bool operator==(const Condition& other) const;

} Condition;

class Hands {
    public:
        std::vector<Card> cards;
        Condition condition;

        Hands(std::vector<Card> cardsIn);

        std::vector<Card> check_pair(std::vector<Card> cards);

        std::vector<Card> check_2pair(std::vector<Card> cards);

        std::vector<Card> check_trips(std::vector<Card> cards);

        std::vector<Card> check_quad(std::vector<Card> cards);

        std::vector<Card> check_straight_help(std::vector<Card> cards);
 

        std::vector<Card> check_straight(std::vector<Card> cards);

        std::vector<Card> check_flush(std::vector<Card> cards);
               
        std::vector<Card> check_straight_flush(std::vector<Card> cards);
        
        std::vector<Card> check_full_house(std::vector<Card> cards);

    bool operator>(const Hands& handB) const;

    bool operator<(const Hands& handB) const;

    bool operator==(const Hands& handB) const;

};

#endif