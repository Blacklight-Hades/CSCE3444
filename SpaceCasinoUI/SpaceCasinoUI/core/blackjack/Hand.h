/*
    Author:     Ramon Lopez | rjl0157 | ramonlopez2@my.unt.edu
    Team:       Group 11 - Team Galactic - Space Casino
    Course:     CSCE 3444.400 Software Engineering
    Instructor: Bahareh M. Dorri
*/

#ifndef HAND_H
#define HAND_H

#include <vector>
#include <string>
#include "Card.h"

using std::string;
using std::vector;


// Player or dealer hand.
class Hand
{
private:
    vector<Card> cards;

public:
    void addCard(const Card& card);
    Card removeCardAt(int index);
    void clear();

    int getValue() const;
    int getCardCount() const;
    int getSplitValue() const;
    bool isBust() const;
    bool isBlackjack() const;
    bool isSoft() const;
    bool canSplit() const;

    vector<Card> getCards() const;
    string toString() const;
};

#endif
