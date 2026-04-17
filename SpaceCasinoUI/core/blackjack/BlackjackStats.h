#ifndef BLACKJACKSTATS_H
#define BLACKJACKSTATS_H

#include <string>
#include <vector>
#include "BlackjackTypes.h"

using std::string;
using std::vector;

// Tracks and displays cumulative blackjack session statistics.
// Usage: after every round ends, call recordRound(game.getRoundSummary()).
// Call displayStats() anytime to print a formatted stats screen.

class BlackjackStats
{
private:
    //  Round / hands count
    int totalRounds;
    int totalWins;
    int totalLosses;
    int totalPushes;
    int totalBlackjacks;
    int totalPlayerBusts;
    int totalDealerBusts;
    int totalDoubleDowns;
    int totalDoubleDownWins;

    //  Bankroll tracker
    double startingBankroll;
    double currentBankroll;
    double totalAmountBet;
    double totalPayoutReceived;
    double biggestWin;
    double biggestLoss;
    double peakBankroll;
    double lowestBankroll;

    //  WIN/Loss Streak tracking 
    int currentStreak;         // positive = win streak, negative = loss streak
    int longestWinStreak;
    int longestLossStreak;

    //  Round history
    struct RoundResult
    {
        int roundNumber;
        string outcome;
        double netChange;
        double bankrollAfter;
    };
    vector<RoundResult> history;

    //  Helpers 
    string formatMoney(double amount) const;
    string formatPercent(double numerator, double denominator) const;
    string buildBar(double ratio, int width) const;
    void printDivider(char c = '-', int width = 50) const;
    void printRow(const string& label, const string& value, int width = 50) const;

public:
    // Pass in the starting bankroll once when the session begins
    BlackjackStats(double startingBankroll);

    // Call this after every round using game.getRoundSummary()
    void recordRound(const BlackjackRoundSummary& summary);

    // Print the full stats screen to the terminal
    void displayStats() const;

    //  Individual getters (for future app integration) 
    int   getTotalRounds()       const;
    int   getTotalWins()         const;
    int   getTotalLosses()       const;
    int   getTotalPushes()       const;
    int   getTotalBlackjacks()   const;
    int   getTotalPlayerBusts()  const;
    int   getTotalDealerBusts()  const;
    int   getTotalDoubleDowns()  const;
    int   getLongestWinStreak()  const;
    int   getLongestLossStreak() const;
    int   getCurrentStreak()     const;

    double getStartingBankroll()    const;
    double getCurrentBankroll()     const;
    double getNetProfit()           const;
    double getTotalAmountBet()      const;
    double getTotalPayoutReceived() const;
    double getBiggestWin()          const;
    double getBiggestLoss()         const;
    double getPeakBankroll()        const;
    double getLowestBankroll()      const;
    double getWinRate()             const;  // wins / (wins + losses)
    double getROI()                 const;  // net profit / total amount bet
};

#endif
