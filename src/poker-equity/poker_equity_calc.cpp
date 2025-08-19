#include <iostream>
#include "../helpers/hands.hpp"

int main() {
    std::vector<Card> cards1 = {
        {"2", "spade"},
        {"3", "spade"},
        {"T", "spade"},
        {"9", "spade"},
        {"9", "heart"},
        {"2", "club"},
        {"J", "heart"}
    };
    std::vector<Card> cards2 = {
        {"A", "diamond"},
        {"K", "heart"},
        {"Q", "club"},
        {"J", "spade"},
        {"T", "spade"}
    };

    Hands hand1(cards1);
    Hands hand2(cards2);

   std::cout<<(hand1>hand2)<<"\n";
}