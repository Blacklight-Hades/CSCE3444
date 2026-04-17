/*
    Author:     Ramon Lopez | rjl0157 | ramonlopez2@my.unt.edu
    Team:       Group 11 - Team Galactic - Space Casino
    Course:     CSCE 3444.400 Software Engineering
    Instructor: Bahareh M. Dorri
*/

#include "Hand.h"
#include <sstream>
#include <stdexcept>

using std::stringstream;
using std::out_of_range;

namespace
{
    // Gets blackjack value of a card.
    int getCardPointValue(const Card& card)
    {
        Rank rank = card.getRank();

        if (rank == Rank::Ace)
        {
            return 11;
        }

        if (rank == Rank::Ten ||
            rank == Rank::Jack ||
            rank == Rank::Queen ||
            rank == Rank::King)
        {
            return 10;
        }

        return static_cast<int>(rank) + 2;
    }

    // Gets value used for split comparisons.
    int getSplitComparableValue(const Card& card)
    {
        Rank rank = card.getRank();

        if (rank == Rank::Ten ||
            rank == Rank::Jack ||
            rank == Rank::Queen ||
            rank == Rank::King)
        {
            return 10;
        }

        if (rank == Rank::Ace)
        {
            return 11;
        }

        return static_cast<int>(rank) + 2;
    }
}

void Hand::addCard(const Card& card)
{
    cards.push_back(card);
}

Card Hand::removeCardAt(int index)
{
    if (index < 0 || index >= static_cast<int>(cards.size()))
    {
        throw out_of_range("Card index out of range.");
    }

    Card removed = cards[index];
    cards.erase(cards.begin() + index);
    return removed;
}

void Hand::clear()
{
    cards.clear();
}

// Calculates hand value. Handles ace adjustment.
int Hand::getValue() const
{
    int total = 0;
    int aceCount = 0;

    for (const Card& card : cards)
    {
        Rank rank = card.getRank();

        if (rank == Rank::Ace)
        {
            total += 11;
            aceCount++;
        }
        else if (rank == Rank::Ten ||
                 rank == Rank::Jack ||
                 rank == Rank::Queen ||
                 rank == Rank::King)
        {
            total += 10;
        }
        else
        {
            total += static_cast<int>(rank) + 2;
        }
    }

    while (total > 21 && aceCount > 0)
    {
        total -= 10;
        aceCount--;
    }

    return total;
}

int Hand::getCardCount() const
{
    return static_cast<int>(cards.size());
}

// Gets comparable value for splitting.
int Hand::getSplitValue() const
{
    if (cards.empty())
    {
        return 0;
    }

    return getSplitComparableValue(cards[0]);
}

bool Hand::isBust() const
{
    return getValue() > 21;
}

bool Hand::isBlackjack() const
{
    return cards.size() == 2 && getValue() == 21;
}

// Checks if hand is soft. Handles ace logic.
bool Hand::isSoft() const
{
    int total = 0;
    int aceCount = 0;

    for (const Card& card : cards)
    {
        total += getCardPointValue(card);

        if (card.getRank() == Rank::Ace)
        {
            aceCount++;
        }
    }

    while (total > 21 && aceCount > 0)
    {
        total -= 10;
        aceCount--;
    }

    return aceCount > 0;
}

// Checks if hand can be split.
bool Hand::canSplit() const
{
    return cards.size() == 2 &&
           getSplitComparableValue(cards[0]) == getSplitComparableValue(cards[1]);
}

vector<Card> Hand::getCards() const
{
    return cards;
}

// Formats hand as string.
string Hand::toString() const
{
    stringstream ss;

    for (int i = 0; i < static_cast<int>(cards.size()); i++)
    {
        ss << cards[i].toString();

        if (i < static_cast<int>(cards.size()) - 1)
        {
            ss << " ";
        }
    }

    return ss.str();
}