/*
    Author:     Ramon Lopez | rjl0157 | ramonlopez2@my.unt.edu
    Team:       Group 11 - Team Galactic - Space Casino
    Course:     CSCE 3444.400 Software Engineering
    Instructor: Bahareh M. Dorri
*/

#ifndef BLACKJACKTYPES_H
#define BLACKJACKTYPES_H

#include <string>
#include <vector>

using std::string;
using std::vector;

// Current round state.
enum class RoundState
{
    WaitingForBet,
    PlayerTurn,
    DealerTurn,
    RoundOver
};

// Final round outcome.
enum class RoundOutcome
{
    None,
    PlayerWin,
    DealerWin,
    Push,
    PlayerBlackjack,
    PlayerBust,
    DealerBust,
    DealerBlackjack
};

// Hint and decision stats.
struct BlackjackAccuracyStats
{
    int optimalActions = 0;
    int suboptimalActions = 0;
    int hintExcludedActions = 0;
    int hintsUsed = 0;

    // Calculates decision accuracy percent.
    double getAccuracyPercent() const
    {
        int countedActions = optimalActions + suboptimalActions;
        if (countedActions == 0)
        {
            return 0.0;
        }

        return (static_cast<double>(optimalActions) / countedActions) * 100.0;
    }
};

// Luck tracking stats.
struct BlackjackLuckStats
{
    int luckyHands = 0;
    int semiLuckyPushes = 0;
    int unluckyHands = 0;

    double luckyScore = 0.0;
    double unluckyScore = 0.0;

    // Calculates luck factor percent.
    double getLuckFactorPercent() const
    {
        double total = luckyScore + unluckyScore;
        if (total <= 0.0)
        {
            return 0.0;
        }

        return (luckyScore / total) * 100.0;
    }
};

// State of one player hand.
struct SplitHandState
{
    vector<string> cards;
    vector<string> actionSequence;

    double betAmount = 0.0;
    double payoutAmount = 0.0;

    int finalValue = 0;
    int handIndex = 0;

    RoundOutcome outcome = RoundOutcome::None;

    bool won = false;
    bool push = false;
    bool busted = false;
    bool doubledDown = false;
    bool cameFromSplit = false;
    bool finished = false;

    int optimalActions = 0;
    int suboptimalActions = 0;
    int hintExcludedActions = 0;
    int hintsUsed = 0;

    bool hadSuboptimalCountedAction = false;
    bool hadOnlyOptimalCountedActions = false;
    bool wasLucky = false;
    bool wasSemiLucky = false;
    bool wasUnlucky = false;
};

// Full round summary data.
struct BlackjackRoundSummary
{
    int roundNumber = 0;

    double startingBankroll = 0.0;
    double initialBetAmount = 0.0;
    double totalBetCommitted = 0.0;
    double bankrollAfterBetDeduction = 0.0;
    double payoutAmount = 0.0;
    double endingBankroll = 0.0;
    double netChange = 0.0;

    RoundOutcome roundOutcome = RoundOutcome::None;

    int dealerFinalValue = 0;
    int activeHandIndexAtRoundEnd = 0;

    bool playerWon = false;
    bool dealerWon = false;
    bool push = false;
    bool wasNaturalBlackjack = false;
    bool dealerBusted = false;
    bool roundIncludedSplit = false;

    vector<string> dealerCards;
    vector<SplitHandState> playerHands;

    bool lastHandBeforeShuffle = false;
    bool reshuffledBeforeRound = false;
    int cardsRemainingAfterRound = 0;

    BlackjackAccuracyStats accuracyStats;
    BlackjackLuckStats luckStats;
};

#endif