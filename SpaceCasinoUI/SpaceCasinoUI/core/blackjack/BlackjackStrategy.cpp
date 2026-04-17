/*
    Author:     Ramon Lopez | rjl0157 | ramonlopez2@my.unt.edu
    Team:       Group 11 - Team Galactic - Space Casino
    Course:     CSCE 3444.400 Software Engineering
    Instructor: Bahareh M. Dorri
*/

#include "BlackjackStrategy.h"

// Hard total strategy table.
const ChartAction BlackjackStrategy::hardTable[22][12] =
{
    // 0-4 unused
    {},
    {},
    {},
    {},
    {},

    // 5
    { ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit },

    // 6
    { ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit },

    // 7
    { ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit },

    // 8
    { ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit },

    // 9
    { ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::DoubleElseHit, ChartAction::DoubleElseHit, ChartAction::DoubleElseHit, ChartAction::DoubleElseHit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit },

    // 10
    { ChartAction::Hit, ChartAction::Hit, ChartAction::DoubleElseHit, ChartAction::DoubleElseHit, ChartAction::DoubleElseHit, ChartAction::DoubleElseHit, ChartAction::DoubleElseHit, ChartAction::DoubleElseHit, ChartAction::DoubleElseHit, ChartAction::DoubleElseHit, ChartAction::Hit, ChartAction::Hit },

    // 11
    { ChartAction::Hit, ChartAction::Hit, ChartAction::DoubleElseHit, ChartAction::DoubleElseHit, ChartAction::DoubleElseHit, ChartAction::DoubleElseHit, ChartAction::DoubleElseHit, ChartAction::DoubleElseHit, ChartAction::DoubleElseHit, ChartAction::DoubleElseHit, ChartAction::DoubleElseHit, ChartAction::Hit },

    // 12
    { ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit },

    // 13
    { ChartAction::Hit, ChartAction::Hit, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit },

    // 14
    { ChartAction::Hit, ChartAction::Hit, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit },

    // 15
    { ChartAction::Hit, ChartAction::Hit, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit },

    // 16
    { ChartAction::Hit, ChartAction::Hit, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit },

    // 17
    { ChartAction::Hit, ChartAction::Hit, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand },

    // 18
    { ChartAction::Hit, ChartAction::Hit, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand },

    // 19
    { ChartAction::Hit, ChartAction::Hit, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand },

    // 20
    { ChartAction::Hit, ChartAction::Hit, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand },

    // 21
    { ChartAction::Hit, ChartAction::Hit, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand }
};

// Soft total strategy table.
const ChartAction BlackjackStrategy::softTable[22][12] =
{
    {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},

    // 13 (A,2)
    { ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::DoubleElseHit, ChartAction::DoubleElseHit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit },

    // 14 (A,3)
    { ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::DoubleElseHit, ChartAction::DoubleElseHit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit },

    // 15 (A,4)
    { ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::DoubleElseHit, ChartAction::DoubleElseHit, ChartAction::DoubleElseHit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit },

    // 16 (A,5)
    { ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::DoubleElseHit, ChartAction::DoubleElseHit, ChartAction::DoubleElseHit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit },

    // 17 (A,6)
    { ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::DoubleElseHit, ChartAction::DoubleElseHit, ChartAction::DoubleElseHit, ChartAction::DoubleElseHit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit },

    // 18 (A,7)
    { ChartAction::Hit, ChartAction::Hit, ChartAction::Stand, ChartAction::DoubleElseStand, ChartAction::DoubleElseStand, ChartAction::DoubleElseStand, ChartAction::DoubleElseStand, ChartAction::Stand, ChartAction::Stand, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit },

    // 19 (A,8)
    { ChartAction::Hit, ChartAction::Hit, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand },

    // 20 (A,9)
    { ChartAction::Hit, ChartAction::Hit, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand },

    // 21
    { ChartAction::Hit, ChartAction::Hit, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand }
};

// Pair strategy table.
const ChartAction BlackjackStrategy::pairTable[12][12] =
{
    {}, {},

    // 2,2
    { ChartAction::Hit, ChartAction::Hit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit },

    // 3,3
    { ChartAction::Hit, ChartAction::Hit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit },

    // 4,4
    { ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit },

    // 5,5
    { ChartAction::Hit, ChartAction::Hit, ChartAction::DoubleElseHit, ChartAction::DoubleElseHit, ChartAction::DoubleElseHit, ChartAction::DoubleElseHit, ChartAction::DoubleElseHit, ChartAction::DoubleElseHit, ChartAction::DoubleElseHit, ChartAction::DoubleElseHit, ChartAction::Hit, ChartAction::Hit },

    // 6,6
    { ChartAction::Hit, ChartAction::Hit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit },

    // 7,7
    { ChartAction::Hit, ChartAction::Hit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit, ChartAction::Hit },

    // 8,8
    { ChartAction::Hit, ChartAction::Hit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::SplitElseHit },

    // 9,9
    { ChartAction::Hit, ChartAction::Hit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::Stand, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::Stand, ChartAction::Stand },

    // 10,10
    { ChartAction::Hit, ChartAction::Hit, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand, ChartAction::Stand },

    // A,A uses 11
    { ChartAction::Hit, ChartAction::Hit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::SplitElseHit, ChartAction::SplitElseHit }
};

// Gets final recommended action from the strategy tables.
StrategyAction BlackjackStrategy::getRecommendedAction(const Hand& playerHand,
                                                       const Card& dealerUpCard,
                                                       bool canDouble,
                                                       bool canSplit) const
{
    ChartAction chartAction = getChartAction(playerHand, dealerUpCard, canSplit);
    return resolveLegalAction(chartAction, canDouble, canSplit);
}

// Selects the proper chart action for the current hand.
ChartAction BlackjackStrategy::getChartAction(const Hand& playerHand,
                                              const Card& dealerUpCard,
                                              bool canSplit) const
{
    int dealerColumn = getDealerColumn(dealerUpCard);

    if (canSplit && isPairHand(playerHand))
    {
        int pairValue = getPairValue(playerHand);

        // Safety clamp (prevents crash)
        if (pairValue < 2) pairValue = 2;
        if (pairValue > 11) pairValue = 11;

        if (dealerColumn < 2) dealerColumn = 2;
        if (dealerColumn > 11) dealerColumn = 11;

        return pairTable[pairValue][dealerColumn];
    }

    if (isSoftHand(playerHand))
    {
        int softTotal = getSoftTotal(playerHand);
        return softTable[softTotal][dealerColumn];
    }

    int hardTotal = playerHand.getValue();
    if (hardTotal < 5)
    {
        hardTotal = 5;
    }
    else if (hardTotal > 21)
    {
        hardTotal = 21;
    }

    return hardTable[hardTotal][dealerColumn];
}

// Converts chart actions into legal in-game actions.
StrategyAction BlackjackStrategy::resolveLegalAction(ChartAction action,
                                                     bool canDouble,
                                                     bool canSplit) const
{
    switch (action)
    {
        case ChartAction::Hit:
            return StrategyAction::Hit;

        case ChartAction::Stand:
            return StrategyAction::Stand;

        case ChartAction::DoubleElseHit:
            return canDouble ? StrategyAction::Double : StrategyAction::Hit;

        case ChartAction::DoubleElseStand:
            return canDouble ? StrategyAction::Double : StrategyAction::Stand;

        case ChartAction::SplitElseHit:
            return canSplit ? StrategyAction::Split : StrategyAction::Hit;
    }

    return StrategyAction::Hit;
}

const char* BlackjackStrategy::actionToString(StrategyAction action)
{
    switch (action)
    {
        case StrategyAction::Hit:
            return "Hit";
        case StrategyAction::Stand:
            return "Stand";
        case StrategyAction::Double:
            return "Double Down";
        case StrategyAction::Split:
            return "Split";
    }

    return "Hit";
}

// Checks if the hand is soft. Handles ace logic.
bool BlackjackStrategy::isSoftHand(const Hand& hand) const
{
    int total = 0;
    int aceCount = 0;

    for (const Card& card : hand.getCards())
    {
        Rank rank = card.getRank();

        if (rank == Rank::Ace)
        {
            total += 11;
            aceCount++;
        }
        else if (rank == Rank::King || rank == Rank::Queen || rank == Rank::Jack || rank == Rank::Ten)
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

    return aceCount > 0;
}

// Checks if the hand is a pair.
bool BlackjackStrategy::isPairHand(const Hand& hand) const
{
    if (hand.getCardCount() != 2)
    {
        return false;
    }

    auto cards = hand.getCards();
    return cards[0].getRank() == cards[1].getRank();
}

// Maps dealer up card to chart column.
int BlackjackStrategy::getDealerColumn(const Card& dealerUpCard) const
{
    Rank rank = dealerUpCard.getRank();

    if (rank == Rank::Ace)
    {
        return 11;
    }

    if (rank == Rank::King || rank == Rank::Queen || rank == Rank::Jack || rank == Rank::Ten)
    {
        return 10;
    }

    return static_cast<int>(rank) + 2;
}

// Gets soft total used for chart lookup. Handles ace logic.
int BlackjackStrategy::getSoftTotal(const Hand& hand) const
{
    int total = 0;
    int aceCount = 0;

    for (const Card& card : hand.getCards())
    {
        Rank rank = card.getRank();

        if (rank == Rank::Ace)
        {
            total += 11;
            aceCount++;
        }
        else if (rank == Rank::King || rank == Rank::Queen || rank == Rank::Jack || rank == Rank::Ten)
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

    if (total < 13)
    {
        total = 13;
    }
    else if (total > 21)
    {
        total = 21;
    }

    return total;
}

// Gets pair value used for pair chart lookup.
int BlackjackStrategy::getPairValue(const Hand& hand) const
{
    const Card& card = hand.getCards()[0];
    Rank rank = card.getRank();

    if (rank == Rank::Ace)
    {
        return 11;
    }

    if (rank == Rank::King || rank == Rank::Queen || rank == Rank::Jack || rank == Rank::Ten)
    {
        return 10;
    }

    return static_cast<int>(rank) + 2;
}