#include "RouletteStats.h"
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

RouletteStats::RouletteStats(double startBankroll)
    : totalRounds(0),
      totalWins(0),
      totalLosses(0),
      totalAmountBet(0.0),
      totalPayoutReceived(0.0),
      biggestWin(0.0),
      biggestLoss(0.0),
      straightUpHits(0)
{
}

// recordRound

void RouletteStats::recordRound(const RouletteRoundSummary& summary)
{
    totalRounds++;

    totalAmountBet      += summary.betAmount;
    totalPayoutReceived += summary.payoutAmount;

    if (summary.netChange > 0.0)
    {
        totalWins++;
        if (summary.netChange > biggestWin)
            biggestWin = summary.netChange;
        if (summary.wasStraightUp && summary.straightUpWon)
            straightUpHits++;
    }
    else
    {
        totalLosses++;
        double loss = std::abs(summary.netChange);
        if (loss > biggestLoss)
            biggestLoss = loss;
    }
}

// displayStats

void RouletteStats::displayStats() const
{
    const int W = 50;

    cout << endl;
    printDivider('=', W);
    cout << setw((W + 22) / 2) << right
         << "* GALACTIC CASINO  --  ROULETTE STATS *" << endl;
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
    printRow("Straight-Up Hits",    std::to_string(straightUpHits),            W);

    printDivider('=', W);
    cout << endl;
}

// Helpers

string RouletteStats::formatMoney(double amount) const
{
    ostringstream out;
    if (amount >= 0)
        out << "+$" << fixed << setprecision(2) << amount;
    else
        out << "-$" << fixed << setprecision(2) << std::abs(amount);
    return out.str();
}

void RouletteStats::printDivider(char c, int width) const
{
    cout << string(width, c) << endl;
}

void RouletteStats::printRow(const string& label, const string& value, int width) const
{
    int gap = width - 2 - (int)label.size() - (int)value.size();
    if (gap < 1) gap = 1;
    cout << "  " << label << string(gap, '.') << value << endl;
}

// Getters

int    RouletteStats::getTotalRounds()        const { return totalRounds; }
int    RouletteStats::getTotalWins()          const { return totalWins; }
int    RouletteStats::getTotalLosses()        const { return totalLosses; }
double RouletteStats::getTotalAmountBet()     const { return totalAmountBet; }
double RouletteStats::getTotalPayoutReceived() const { return totalPayoutReceived; }
double RouletteStats::getNetProfit()          const { return totalPayoutReceived - totalAmountBet; }
double RouletteStats::getBiggestWin()         const { return biggestWin; }
double RouletteStats::getBiggestLoss()        const { return biggestLoss; }
int    RouletteStats::getStraightUpHits()     const { return straightUpHits; }

double RouletteStats::getWinRate() const
{
    return totalRounds > 0 ? (double)totalWins / totalRounds : 0.0;
}
