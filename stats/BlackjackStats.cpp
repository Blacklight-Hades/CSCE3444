/*
    Name:       Prayush Panta
    UID:        PP1008
    Team:       Group 11 - Team Galactic - Space Casino
    Course:     CSCE 3444 Software Engineering
    Instructor: Bahareh M. Dorri
*/

#include "BlackjackStats.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cmath>

using std::cout;
using std::endl;
using std::setw;
using std::left;
using std::right;
using std::fixed;
using std::setprecision;
using std::ostringstream;
using std::string;


//  Constructor

BlackjackStats::BlackjackStats(double startBankroll)
    : totalRounds(0),
      totalWins(0),
      totalLosses(0),
      totalPushes(0),
      totalBlackjacks(0),
      totalPlayerBusts(0),
      totalDealerBusts(0),
      totalDoubleDowns(0),
      totalDoubleDownWins(0),
      startingBankroll(startBankroll),
      currentBankroll(startBankroll),
      totalAmountBet(0.0),
      totalPayoutReceived(0.0),
      biggestWin(0.0),
      biggestLoss(0.0),
      peakBankroll(startBankroll),
      lowestBankroll(startBankroll),
      currentStreak(0),
      longestWinStreak(0),
      longestLossStreak(0)
{
}


//  recordRound — call after every game.getRoundSummary()

void BlackjackStats::recordRound(const BlackjackRoundSummary& summary)
{
    totalRounds++;

    //  Outcome counters 
    if (summary.playerWon)
    {
        totalWins++;
        currentStreak = (currentStreak > 0) ? currentStreak + 1 : 1;
        longestWinStreak = std::max(longestWinStreak, currentStreak);
    }
    else if (summary.dealerWon)
    {
        totalLosses++;
        currentStreak = (currentStreak < 0) ? currentStreak - 1 : -1;
        longestLossStreak = std::max(longestLossStreak, std::abs(currentStreak));
    }
    else if (summary.push)
    {
        totalPushes++;
        currentStreak = 0;
    }

    if (summary.wasNaturalBlackjack) totalBlackjacks++;
    if (summary.dealerBusted)        totalDealerBusts++;

    //  Player bust: derive from roundOutcome 
    //    BlackjackRoundSummary has no playerBusted field;
    //    PlayerBust outcome means the player busted.
    if (summary.roundOutcome == RoundOutcome::PlayerBust)
        totalPlayerBusts++;

    //  Double down: derive from hand-level data 
    //    BlackjackRoundSummary has no wasDoubleDown field;
    //    check each player hand's doubledDown flag instead.
    for (const SplitHandState& hand : summary.playerHands)
    {
        if (hand.doubledDown)
        {
            totalDoubleDowns++;
            if (hand.won) totalDoubleDownWins++;
        }
    }

    //  Financial tracking 
    //    Use initialBetAmount (not betAmount — that field doesn't exist).
    totalAmountBet      += summary.initialBetAmount;
    totalPayoutReceived += summary.payoutAmount;
    currentBankroll      = summary.endingBankroll;

    if (summary.netChange > 0.0)
        biggestWin  = std::max(biggestWin,  summary.netChange);
    else if (summary.netChange < 0.0)
        biggestLoss = std::max(biggestLoss, std::abs(summary.netChange));

    peakBankroll   = std::max(peakBankroll,   currentBankroll);
    lowestBankroll = std::min(lowestBankroll, currentBankroll);

    //  History (last 10 rounds) 
    RoundResult result;
    result.roundNumber   = summary.roundNumber;
    result.netChange     = summary.netChange;
    result.bankrollAfter = summary.endingBankroll;

    switch (summary.roundOutcome)
    {
        case RoundOutcome::PlayerBlackjack: result.outcome = "BLACKJACK";    break;
        case RoundOutcome::PlayerWin:       result.outcome = "WIN";          break;
        case RoundOutcome::DealerBust:      result.outcome = "WIN (D.Bust)"; break;
        case RoundOutcome::Push:            result.outcome = "PUSH";         break;
        case RoundOutcome::PlayerBust:      result.outcome = "LOSS (Bust)";  break;
        case RoundOutcome::DealerWin:       result.outcome = "LOSS";         break;
        case RoundOutcome::DealerBlackjack: result.outcome = "LOSS (D.BJ)";  break;
        default:                            result.outcome = "---";          break;
    }

    history.push_back(result);
    if (history.size() > 10)
        history.erase(history.begin());
}


//  displayStats

void BlackjackStats::displayStats() const
{
    const int W = 50;

    cout << endl;
    printDivider('=', W);
    cout << setw((W + 22) / 2) << right
         << "* GALACTIC CASINO  --  BLACKJACK STATS *" << endl;
    printDivider('=', W);

    //  Session overview 
    cout << " SESSION OVERVIEW" << endl;
    printDivider('-', W);
    printRow("Rounds Played",     std::to_string(totalRounds),              W);
    printRow("Starting Bankroll", formatMoney(startingBankroll),            W);
    printRow("Current Bankroll",  formatMoney(currentBankroll),             W);
    printRow("Net Profit / Loss", formatMoney(currentBankroll - startingBankroll), W);
    printRow("Peak Bankroll",     formatMoney(peakBankroll),                W);
    printRow("Lowest Bankroll",   formatMoney(lowestBankroll),              W);
    printRow("Total Wagered",     formatMoney(totalAmountBet),              W);
    printRow("ROI",               formatPercent(currentBankroll - startingBankroll,
                                                totalAmountBet),            W);

    //  Win / loss breakdown 
    cout << endl;
    cout << " WIN / LOSS BREAKDOWN" << endl;
    printDivider('-', W);
    printRow("Wins",     std::to_string(totalWins)   + "  " + formatPercent(totalWins,   totalRounds), W);
    printRow("Losses",   std::to_string(totalLosses) + "  " + formatPercent(totalLosses, totalRounds), W);
    printRow("Pushes",   std::to_string(totalPushes) + "  " + formatPercent(totalPushes, totalRounds), W);
    printRow("Win Rate", formatPercent(totalWins, totalWins + totalLosses),                            W);

    if (totalWins + totalLosses > 0)
    {
        double ratio = (double)totalWins / (totalWins + totalLosses);
        cout << "  [" << buildBar(ratio, W - 6) << "]" << endl;
    }

    //  Special events 
    cout << endl;
    cout << " SPECIAL EVENTS" << endl;
    printDivider('-', W);
    printRow("Natural Blackjacks",
             std::to_string(totalBlackjacks) + "  " + formatPercent(totalBlackjacks, totalRounds), W);
    printRow("Player Busts",
             std::to_string(totalPlayerBusts) + "  " + formatPercent(totalPlayerBusts, totalRounds), W);
    printRow("Dealer Busts",
             std::to_string(totalDealerBusts) + "  " + formatPercent(totalDealerBusts, totalRounds), W);
    printRow("Double Downs", std::to_string(totalDoubleDowns), W);

    if (totalDoubleDowns > 0)
        printRow("  DD Win Rate", formatPercent(totalDoubleDownWins, totalDoubleDowns), W);

    //  Streaks & records 
    cout << endl;
    cout << " STREAKS & RECORDS" << endl;
    printDivider('-', W);

    string streakStr;
    if      (currentStreak > 0) streakStr = "+" + std::to_string(currentStreak) + " (winning)";
    else if (currentStreak < 0) streakStr =       std::to_string(currentStreak) + " (losing)";
    else                        streakStr = "0 (neutral)";

    printRow("Current Streak",      streakStr,                          W);
    printRow("Longest Win Streak",  std::to_string(longestWinStreak),   W);
    printRow("Longest Loss Streak", std::to_string(longestLossStreak),  W);
    printRow("Biggest Single Win",  formatMoney(biggestWin),            W);
    printRow("Biggest Single Loss", formatMoney(biggestLoss),           W);

    //  Recent rounds 
    cout << endl;
    cout << " LAST " << history.size() << " ROUNDS" << endl;
    printDivider('-', W);
    cout << left
         << setw(8)  << "  Round"
         << setw(18) << "Outcome"
         << setw(12) << "Net"
         << setw(14) << "Bankroll"
         << endl;
    printDivider('-', W);

    for (const RoundResult& r : history)
    {
        cout << left
             << setw(8)  << ("  #" + std::to_string(r.roundNumber))
             << setw(18) << r.outcome
             << setw(12) << formatMoney(r.netChange)
             << setw(14) << formatMoney(r.bankrollAfter)
             << endl;
    }

    printDivider('=', W);
    cout << endl;
}


//  Helpers

string BlackjackStats::formatMoney(double amount) const
{
    ostringstream out;
    if (amount >= 0)
        out << "+$" << fixed << setprecision(2) << amount;
    else
        out << "-$" << fixed << setprecision(2) << std::abs(amount);
    return out.str();
}

string BlackjackStats::formatPercent(double numerator, double denominator) const
{
    if (denominator <= 0) return "(0.0%)";
    ostringstream out;
    out << "(" << fixed << setprecision(1)
        << (numerator / denominator * 100.0) << "%)";
    return out.str();
}

string BlackjackStats::buildBar(double ratio, int width) const
{
    int filled = static_cast<int>(ratio * width);
    filled = std::max(0, std::min(filled, width));
    return string(filled, '#') + string(width - filled, '.');
}

void BlackjackStats::printDivider(char c, int width) const
{
    cout << string(width, c) << endl;
}

void BlackjackStats::printRow(const string& label, const string& value, int width) const
{
    int gap = width - 2 - (int)label.size() - (int)value.size();
    if (gap < 1) gap = 1;
    cout << "  " << label << string(gap, '.') << value << endl;
}


//  Getters

int    BlackjackStats::getTotalRounds()       const { return totalRounds; }
int    BlackjackStats::getTotalWins()         const { return totalWins; }
int    BlackjackStats::getTotalLosses()       const { return totalLosses; }
int    BlackjackStats::getTotalPushes()       const { return totalPushes; }
int    BlackjackStats::getTotalBlackjacks()   const { return totalBlackjacks; }
int    BlackjackStats::getTotalPlayerBusts()  const { return totalPlayerBusts; }
int    BlackjackStats::getTotalDealerBusts()  const { return totalDealerBusts; }
int    BlackjackStats::getTotalDoubleDowns()  const { return totalDoubleDowns; }
int    BlackjackStats::getLongestWinStreak()  const { return longestWinStreak; }
int    BlackjackStats::getLongestLossStreak() const { return longestLossStreak; }
int    BlackjackStats::getCurrentStreak()     const { return currentStreak; }

double BlackjackStats::getStartingBankroll()    const { return startingBankroll; }
double BlackjackStats::getCurrentBankroll()     const { return currentBankroll; }
double BlackjackStats::getNetProfit()           const { return currentBankroll - startingBankroll; }
double BlackjackStats::getTotalAmountBet()      const { return totalAmountBet; }
double BlackjackStats::getTotalPayoutReceived() const { return totalPayoutReceived; }
double BlackjackStats::getBiggestWin()          const { return biggestWin; }
double BlackjackStats::getBiggestLoss()         const { return biggestLoss; }
double BlackjackStats::getPeakBankroll()        const { return peakBankroll; }
double BlackjackStats::getLowestBankroll()      const { return lowestBankroll; }

double BlackjackStats::getWinRate() const
{
    int decided = totalWins + totalLosses;
    return decided > 0 ? (double)totalWins / decided : 0.0;
}

double BlackjackStats::getROI() const
{
    return totalAmountBet > 0
        ? (currentBankroll - startingBankroll) / totalAmountBet
        : 0.0;
}
