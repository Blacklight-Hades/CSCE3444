/*
    Author:     Ramon Lopez | rjl0157 | ramonlopez2@my.unt.edu
    Team:       Group 11 - Team Galactic - Space Casino
    Course:     CSCE 3444.400 Software Engineering
    Instructor: Bahareh M. Dorri
*/

#include "Shoe.h"
#include <algorithm>
#include <chrono>
#include <stdexcept>

using std::runtime_error;

// Initializes shoe and seeds RNG.
Shoe::Shoe(int numberOfDecks)
    : deckCount(numberOfDecks), nextCardIndex(0)
{
    unsigned seed = static_cast<unsigned>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()
    );

    rng.seed(seed);
    reset();
}

// Builds full shoe of cards.
void Shoe::buildShoe()
{
    cards.clear();

    for (int deck = 0; deck < deckCount; deck++)
    {
        for (int suitValue = static_cast<int>(Suit::Clubs);
             suitValue <= static_cast<int>(Suit::Spades);
             suitValue++)
        {
            Suit suit = static_cast<Suit>(suitValue);

            for (int rankValue = static_cast<int>(Rank::Two);
                 rankValue <= static_cast<int>(Rank::Ace);
                 rankValue++)
            {
                Rank rank = static_cast<Rank>(rankValue);
                cards.push_back(Card(rank, suit));
            }
        }
    }
}

// Shuffles cards using RNG.
void Shoe::shuffle()
{
    std::shuffle(cards.begin(), cards.end(), rng);
    nextCardIndex = 0;
}

// Resets and reshuffles shoe.
void Shoe::reset()
{
    buildShoe();
    shuffle();
}

// Deals next card from shoe.
Card Shoe::dealCard()
{
    if (isEmpty())
    {
        throw runtime_error("Cannot deal from an empty shoe.");
    }

    Card nextCard = cards[nextCardIndex];
    nextCardIndex++;

    return nextCard;
}

int Shoe::getCardsRemaining() const
{
    return static_cast<int>(cards.size()) - nextCardIndex;
}

int Shoe::getTotalCards() const
{
    return static_cast<int>(cards.size());
}

bool Shoe::isEmpty() const
{
    return nextCardIndex >= static_cast<int>(cards.size());
}