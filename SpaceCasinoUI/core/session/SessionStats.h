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
#include "../blackjack/BlackjackStats.h"
// #include "SlotsStats.h"    // Sprint 2
// #include "RouletteStats.h" // Sprint 2

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
    // SlotsStats    slotsStats;      // Sprint 2
    // RouletteStats rouletteStats;   // Sprint 2

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

    //  Game runners 
    void playBlackjack();
    void playSlots();      // Sprint 2 placeholder
    void playRoulette();   // Sprint 2 placeholder

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
};

#endif
