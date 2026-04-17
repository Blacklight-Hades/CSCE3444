/*
    Author:     Ramon Lopez | rjl0157 | ramonlopez2@my.unt.edu
    Team:       Group 11 - Team Galactic - Space Casino
    Course:     CSCE 3444.400 Software Engineering
    Instructor: Bahareh M. Dorri
*/

#ifndef CARD_H
#define CARD_H

#include <string>

using std::string;

// Rank of a playing card.
enum class Rank
{
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Jack,
    Queen,
    King,
    Ace
};

// Suit of a playing card.
enum class Suit
{
    Clubs,
    Diamonds,
    Hearts,
    Spades
};

// Card class.
class Card
{
private:
    Rank rank;
    Suit suit;

public:
    Card(Rank newRank, Suit newSuit);

    Rank getRank() const;

    Suit getSuit() const;

    string toString() const;
};

#endif