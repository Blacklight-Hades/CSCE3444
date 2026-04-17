#ifndef ROULETTESTATS_H
#define ROULETTESTATS_H

#include <string>
#include <vector>

using std::string;
using std::vector;

struct RouletteRoundSummary
{
    double betAmount;
    double payoutAmount;
    double netChange;
    bool wasStraightUp;
    bool straightUpWon;
};

class RouletteStats
{
private:
    int totalRounds;
    int totalWins;
    int totalLosses;

    double totalAmountBet;
    double totalPayoutReceived;
    double biggestWin;
    double biggestLoss;

    int straightUpHits;

    // Helpers
    string formatMoney(double amount) const;
    void printDivider(char c = '-', int width = 50) const;
    void printRow(const string& label, const string& value, int width = 50) const;

public:
    RouletteStats(double startingBankroll = 0.0);

    void recordRound(const RouletteRoundSummary& summary);
    void displayStats() const;

    // Getters
    int    getTotalRounds()        const;
    int    getTotalWins()          const;
    int    getTotalLosses()        const;
    double getTotalAmountBet()     const;
    double getTotalPayoutReceived() const;
    double getNetProfit()          const;
    double getBiggestWin()         const;
    double getBiggestLoss()        const;
    double getWinRate()            const;
    int    getStraightUpHits()     const;
};

#endif
