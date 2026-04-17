#include "SlotsStats.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>

using std::cout;
using std::endl;
using std::setw;
using std::right;
using std::fixed;
using std::setprecision;
using std::ostringstream;
using std::string;

// Constructor

SlotsStats::SlotsStats(double startBankroll)
    : totalRounds(0),
      totalWins(0),
      totalLosses(0),
      totalAmountBet(0.0),
      totalPayoutReceived(0.0),
      biggestWin(0.0),
      biggestLoss(0.0),
      threeInARowHits(0),
      jackpotHits(0)
{
}

// recordRound

void SlotsStats::recordRound(const SlotsRoundSummary& summary)
{
    totalRounds++;

    totalAmountBet      += summary.betAmount;
    totalPayoutReceived += summary.payoutAmount;

    if (summary.netChange > 0.0)
    {
        totalWins++;
        if (summary.netChange > biggestWin)
            biggestWin = summary.netChange;
    }
    else
    {
        totalLosses++;
        double loss = std::abs(summary.netChange);
        if (loss > biggestLoss)
            biggestLoss = loss;
    }

    if (summary.wasThreeInARow) threeInARowHits++;
    if (summary.wasJackpot)     jackpotHits++;
}

// displayStats

void SlotsStats::displayStats() const
{
    const int W = 50;

    cout << endl;
    printDivider('=', W);
    cout << setw((W + 22) / 2) << right
         << "* GALACTIC CASINO  --  SLOTS STATS *" << endl;
    printDivider('=', W);

    // Session overview
    cout << " SESSION OVERVIEW" << endl;
    printDivider('-', W);
    printRow("Rounds Played",     std::to_string(totalRounds),              W);
    printRow("Wins",              std::to_string(totalWins),                W);
    printRow("Losses",            std::to_string(totalLosses),              W);

    ostringstream wr;
    wr << fixed << setprecision(1) << getWinRate() * 100.0 << "%";
    printRow("Win Rate", wr.str(), W);

    // Financial
    cout << endl;
    cout << " FINANCIAL" << endl;
    printDivider('-', W);
    printRow("Total Wagered",       formatMoney(totalAmountBet),               W);
    printRow("Total Payout",        formatMoney(totalPayoutReceived),           W);
    printRow("Net Profit / Loss",   formatMoney(getNetProfit()),               W);
    printRow("Biggest Single Win",  formatMoney(biggestWin),                   W);
    printRow("Biggest Single Loss", formatMoney(biggestLoss),                  W);

    // Special events
    cout << endl;
    cout << " SPECIAL EVENTS" << endl;
    printDivider('-', W);
    printRow("3-in-a-Row Matches",  std::to_string(threeInARowHits),          W);
    printRow("Jackpots (Bar/Seven)", std::to_string(jackpotHits),             W);

    printDivider('=', W);
    cout << endl;
}

// Helpers

string SlotsStats::formatMoney(double amount) const
{
    ostringstream out;
    if (amount >= 0)
        out << "+$" << fixed << setprecision(2) << amount;
    else
        out << "-$" << fixed << setprecision(2) << std::abs(amount);
    return out.str();
}

void SlotsStats::printDivider(char c, int width) const
{
    cout << string(width, c) << endl;
}

void SlotsStats::printRow(const string& label, const string& value, int width) const
{
    int gap = width - 2 - (int)label.size() - (int)value.size();
    if (gap < 1) gap = 1;
    cout << "  " << label << string(gap, '.') << value << endl;
}

// Getters

int    SlotsStats::getTotalRounds()        const { return totalRounds; }
int    SlotsStats::getTotalWins()          const { return totalWins; }
int    SlotsStats::getTotalLosses()        const { return totalLosses; }
double SlotsStats::getTotalAmountBet()     const { return totalAmountBet; }
double SlotsStats::getTotalPayoutReceived() const { return totalPayoutReceived; }
double SlotsStats::getNetProfit()          const { return totalPayoutReceived - totalAmountBet; }
double SlotsStats::getBiggestWin()         const { return biggestWin; }
double SlotsStats::getBiggestLoss()        const { return biggestLoss; }
int    SlotsStats::getThreeInARowHits()    const { return threeInARowHits; }
int    SlotsStats::getJackpotHits()        const { return jackpotHits; }

double SlotsStats::getWinRate() const
{
    return totalRounds > 0 ? (double)totalWins / totalRounds : 0.0;
}
