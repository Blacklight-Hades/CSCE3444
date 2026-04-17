/*
    Author:     Ramon Lopez | rjl0157 | ramonlopez2@my.unt.edu
    Team:       Group 11 - Team Galactic - Space Casino
    Course:     CSCE 3444.400 Software Engineering
    Instructor: Bahareh M. Dorri
*/

#include "Card.h"

Card::Card(Rank newRank, Suit newSuit)
    : rank(newRank), suit(newSuit)
{
}

Rank Card::getRank() const
{
    return rank;
}

Suit Card::getSuit() const
{
    return suit;
}

// Formatting cards into strings.
string Card::toString() const
{
    string rankText;
    string suitText;

    switch (rank)
    {
        case Rank::Two:
            rankText = "2";
            break;
        case Rank::Three:
            rankText = "3";
            break;
        case Rank::Four:
            rankText = "4";
            break;
        case Rank::Five:
            rankText = "5";
            break;
        case Rank::Six:
            rankText = "6";
            break;
        case Rank::Seven:
            rankText = "7";
            break;
        case Rank::Eight:
            rankText = "8";
            break;
        case Rank::Nine:
            rankText = "9";
            break;
        case Rank::Ten:
            rankText = "10";
            break;
        case Rank::Jack:
            rankText = "J";
            break;
        case Rank::Queen:
            rankText = "Q";
            break;
        case Rank::King:
            rankText = "K";
            break;
        case Rank::Ace:
            rankText = "A";
            break;
    }

    switch (suit)
    {
        case Suit::Clubs:
            suitText = "C";
            break;
        case Suit::Diamonds:
            suitText = "D";
            break;
        case Suit::Hearts:
            suitText = "H";
            break;
        case Suit::Spades:
            suitText = "S";
            break;
    }

    return rankText + suitText;
}