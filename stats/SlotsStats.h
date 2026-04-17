#ifndef SLOTSSTATS_H
#define SLOTSSTATS_H

#include <string>
#include <vector>

using std::string;
using std::vector;

struct SlotsRoundSummary
{
    double betAmount;
    double payoutAmount;
    double netChange;
    bool wasThreeInARow;
    bool wasJackpot;    // bar or seven triple
};

class SlotsStats
{
private:
    int totalRounds;
    int totalWins;
    int totalLosses;

    double totalAmountBet;
    double totalPayoutReceived;
    double biggestWin;
    double biggestLoss;

    int threeInARowHits;
    int jackpotHits;

    // Helpers
    string formatMoney(double amount) const;
    void printDivider(char c = '-', int width = 50) const;
    void printRow(const string& label, const string& value, int width = 50) const;

public:
    SlotsStats(double startingBankroll = 0.0);

    void recordRound(const SlotsRoundSummary& summary);
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
    int    getThreeInARowHits()    const;
    int    getJackpotHits()        const;
};

#endif
