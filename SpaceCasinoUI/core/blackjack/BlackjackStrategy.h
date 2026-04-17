/*
    Author:     Ramon Lopez | rjl0157 | ramonlopez2@my.unt.edu
    Team:       Group 11 - Team Galactic - Space Casino
    Course:     CSCE 3444.400 Software Engineering
    Instructor: Bahareh M. Dorri
*/

#ifndef BLACKJACKSTRATEGY_H
#define BLACKJACKSTRATEGY_H

#include "Hand.h"
#include "Card.h"

// Raw chart actions.
enum class ChartAction
{
    Hit,
    Stand,
    DoubleElseHit,
    DoubleElseStand,
    SplitElseHit
};

// Final playable actions.
enum class StrategyAction
{
    Hit,
    Stand,
    Double,
    Split
};

// Basic strategy engine.
class BlackjackStrategy
{
public:
    StrategyAction getRecommendedAction(const Hand& playerHand,
                                        const Card& dealerUpCard,
                                        bool canDouble,
                                        bool canSplit) const;

    static const char* actionToString(StrategyAction action);

private:
    ChartAction getChartAction(const Hand& playerHand,
                               const Card& dealerUpCard,
                               bool canSplit) const;

    StrategyAction resolveLegalAction(ChartAction action,
                                      bool canDouble,
                                      bool canSplit) const;

    bool isSoftHand(const Hand& hand) const;
    bool isPairHand(const Hand& hand) const;

    int getDealerColumn(const Card& dealerUpCard) const;
    int getSoftTotal(const Hand& hand) const;
    int getPairValue(const Hand& hand) const;

    static const ChartAction hardTable[22][12];
    static const ChartAction softTable[22][12];
    static const ChartAction pairTable[12][12];
};

#endif