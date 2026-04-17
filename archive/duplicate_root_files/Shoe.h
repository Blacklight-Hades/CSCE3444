/*
    Author:     Ramon Lopez | rjl0157 | ramonlopez2@my.unt.edu
    Team:       Group 11 - Team Galactic - Space Casino
    Course:     CSCE 3444.400 Software Engineering
    Instructor: Bahareh M. Dorri
*/

#ifndef SHOE_H
#define SHOE_H

#include <vector>
#include <random>
#include "Card.h"

using std::vector;
using std::mt19937;


// Shoe with multiple decks.
class Shoe
{
private:
    vector<Card> cards;
    int deckCount;
    int nextCardIndex;
    mt19937 rng;

    void buildShoe();   // Builds the shoe.

public:
    Shoe(int numberOfDecks = 6);

    void reset();
    void shuffle();

    Card dealCard();

    int getCardsRemaining() const;
    int getTotalCards() const;
    bool isEmpty() const;
};

#endif