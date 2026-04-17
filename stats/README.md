# Stats — UI Integration Guide

This folder contains session statistics trackers for all three casino games.
They are currently fully integrated with **terminal mode** (SessionManager).
They are **ready for UI integration** but not yet connected to the GUI screens.

## Available Stats Classes

### BlackjackStats (`BlackjackStats.h / .cpp`)
- Tracks: rounds, wins, losses, pushes, blackjacks, player/dealer busts,
  double-down wins, bankroll history, win/loss streaks, biggest win/loss
- Input: `recordRound(BlackjackRoundSummary)` — pass `game.getRoundSummary()`
- Display: `displayStats()` prints formatted terminal output
- Getters: `getTotalRounds()`, `getTotalWins()`, `getNetProfit()`, `getBiggestWin()`, etc.

### SlotsStats (`SlotsStats.h / .cpp`)
- Tracks: rounds, wins, losses, total wagered, total payout, three-in-a-row hits, jackpots
- Input: `recordRound(SlotsRoundSummary)` — build summary after each spin
- Getters: `getTotalRounds()`, `getJackpotHits()`, `getWinRate()`, etc.

### RouletteStats (`RouletteStats.h / .cpp`)
- Tracks: rounds, wins, losses, straight-up hits, biggest win/loss
- Input: `recordRound(RouletteRoundSummary)` — build summary after each spin
- Getters: `getTotalRounds()`, `getStraightUpHits()`, `getWinRate()`, etc.

---

## How to Integrate with GUI Screens

### Step 1: Add stats members to the UI screen class

For example, in `BlackjackUI.h`, add:
```cpp
#include "../../../stats/BlackjackStats.h"

// In the class:
BlackjackStats bjStats;
```

Initialize in the constructor:
```cpp
BlackjackUI::BlackjackUI(sf::Font& sharedFont)
    : game(1000.0), bjStats(1000.0), ...
```

### Step 2: Call `recordRound()` after each round ends

In `BlackjackUI::handleGameClick()`, after a round completes:
```cpp
if (game.isRoundOver())
{
    bjStats.recordRound(game.getRoundSummary());
}
```

For SlotsUI, after each spin:
```cpp
SlotsRoundSummary summary;
summary.betAmount      = currentBet;
summary.payoutAmount   = lastPayout;
summary.netChange      = lastPayout - currentBet;
summary.wasThreeInARow = /* check middle row match */;
summary.wasJackpot     = /* check for bar/seven triple */;
slotsStats.recordRound(summary);
```

### Step 3: Display stats in the UI

Use the getter methods to render stats text on screen:
```cpp
std::string statsLine = "Rounds: " + std::to_string(bjStats.getTotalRounds())
                      + " | Wins: " + std::to_string(bjStats.getTotalWins())
                      + " | Win%: " + std::to_string((int)(bjStats.getWinRate() * 100)) + "%";
```

### Step 4 (Optional): Add a Stats screen to CasinoApp

1. Add `Stats` to `AppState` enum
2. Create a `StatsScreen` class that takes all three stats objects
3. Render formatted stats using the getter methods
4. Add navigation from `GameSelect` to the stats screen

---

## Summary Struct Definitions

### BlackjackRoundSummary (in `games/blackjack/BlackjackTypes.h`)
- Provided automatically by `BlackjackGame::getRoundSummary()`

### SlotsRoundSummary (in `stats/SlotsStats.h`)
```cpp
struct SlotsRoundSummary {
    double betAmount;
    double payoutAmount;
    double netChange;
    bool wasThreeInARow;
    bool wasJackpot;
};
```

### RouletteRoundSummary (in `stats/RouletteStats.h`)
```cpp
struct RouletteRoundSummary {
    double betAmount;
    double payoutAmount;
    double netChange;
    bool wasStraightUp;
    bool straightUpWon;
};
```
