/*
    Name:       Prayush Panta
    UID:        PP1008
    Team:       Group 11 - Team Galactic - Space Casino
    Course:     CSCE 3444 Software Engineering
    Instructor: Bahareh M. Dorri

    SessionStats — Main session manager.
    - Manages the Bankroll.
    - Tracks session duration.
    - Tracks game play.
    - Displays session summary.
*/

#ifndef SESSIONSTATS_H
#define SESSIONSTATS_H

#include <string>
#include <chrono>
#include "Bankroll.h"
#include "../stats/BlackjackStats.h"
#include "../stats/SlotsStats.h"
#include "../stats/RouletteStats.h"

using std::string;
using std::chrono::steady_clock;
using std::chrono::time_point;

class SessionStats
{
private:
    //  Centralized bankroll 
    Bankroll bankroll;

    //  Cross-game session totals 
    int  totalRoundsAllGames;
    int  gamesPlayed;
    bool playedBlackjack;
    bool playedSlots;
    bool playedRoulette;

    //  Per-game stats modules 
    BlackjackStats blackjackStats;
    SlotsStats slotsStats;
    RouletteStats rouletteStats;

    //  Session timer 
    time_point<steady_clock> sessionStart;
    bool sessionStarted;

    //  Helpers 
    string formatMoney(double amount)    const;
    string formatDuration(double secs)   const;
    void   printDivider(char c = '-', int width = 52) const;
    void   printRow(const string& label, const string& value, int width = 52) const;

public:
    explicit SessionStats(double startingBalance);

    //  Session lifecycle 
    void startSession();
    void endSession();

    //  Game runners (for terminal)
    void playBlackjack();
    void playSlots();
    void playRoulette();

    //  Display 
    void displaySessionSummary() const;

    //  Getters 
    double getCurrentBalance()  const { return bankroll.getBalance(); }
    double getNetGainLoss()     const { return bankroll.getNetGainLoss(); }
    double getPeakBalance()     const { return bankroll.getPeakBalance(); }
    double getLowestBalance()   const { return bankroll.getLowestBalance(); }
    int    getTotalRounds()     const { return totalRoundsAllGames; }
    int    getGamesPlayed()     const { return gamesPlayed; }
    double getSessionDuration() const;

    const BlackjackStats& getBlackjackStats() const { return blackjackStats; }
    const SlotsStats& getSlotsStats() const { return slotsStats; }
    const RouletteStats& getRouletteStats() const { return rouletteStats; }

    void recordBlackjackRound(const BlackjackRoundSummary& summary) {
        blackjackStats.recordRound(summary);
        totalRoundsAllGames++;
        if (!playedBlackjack) { playedBlackjack = true; gamesPlayed++; }
        bankroll.setBalance(summary.endingBankroll);
    }
    
    void recordSlotsRound(const SlotsRoundSummary& summary) {
        slotsStats.recordRound(summary);
        totalRoundsAllGames++;
        if (!playedSlots) { playedSlots = true; gamesPlayed++; }
        bankroll.deposit(summary.payoutAmount);
        bankroll.withdraw(summary.betAmount); // rough estimation for deposit/withdraw tracking if needed
    }
    
    void recordRouletteRound(const RouletteRoundSummary& summary) {
        rouletteStats.recordRound(summary);
        totalRoundsAllGames++;
        if (!playedRoulette) { playedRoulette = true; gamesPlayed++; }
        bankroll.deposit(summary.payoutAmount);
        bankroll.withdraw(summary.betAmount);
    }
    
    Bankroll& getBankroll() { return bankroll; }
};

#endif
