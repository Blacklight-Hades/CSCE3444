# How `core/` and `stats/` Work — Beginner's Guide

Think of your casino game like a real physical casino. This guide walks through every file using real-world comparisons so it's easy to understand even if you're new to programming.

---

## The Big Picture

Imagine you walk into a casino. Here's what maps to what:

| Real Casino | Code File |
|---|---|
| Your wallet with cash | `Bankroll` |
| The casino manager tracking everything | `SessionManager` |
| The casino's overall records at day-end | `SessionStats` |
| Blackjack dealer writing scores on a notepad | `BlackjackStats` |
| Slots host writing scores on a notepad | `SlotsStats` |
| Roulette host writing scores on a notepad | `RouletteStats` |

---

## `core/Bankroll` — Your Wallet

**What it is:** A simple class that holds one number — your money balance.

**What it does:**
- Stores how much money you currently have (`balance`)
- Remembers what you started with (`startingBalance`)
- Tracks the highest your balance ever got (`peakBalance`)
- Tracks the lowest your balance ever got (`lowestBalance`)

**How it's called:**
```
You start with $1000
→ Bankroll is created with balance = $1000
→ You bet $50 → Bankroll.withdraw(50) is called → balance = $950
→ You win $75 → Bankroll.deposit(75) is called → balance = $1025
→ Bankroll.getBalance() returns 1025 anytime someone wants to check
```

**Key methods:**
- `canAfford(amount)` — asks "do you have enough?" (returns true/false)
- `withdraw(amount)` — takes money out
- `deposit(amount)` — puts money back in
- `getNetGainLoss()` — tells you if you won or lost overall (current - starting)
- `isBroke()` — returns true if you're at $0

**Who calls it:** `SessionManager` is the only class that directly touches `Bankroll`. The games don't touch the real wallet — they just tell the manager how much they changed.

---

## `core/SessionManager` — The Casino Manager

**What it is:** The central "boss" of the whole program. It coordinates everything.

**Think of it like:** A casino floor manager who:
1. Takes your money at the door (sets up the bankroll)
2. Walks you to the Blackjack table and gives you chips
3. When you're done at Blackjack, takes your remaining chips back and updates your main wallet
4. Then walks you to Slots, gives you coins, takes them back when done
5. Keeps track of which game you're currently in
6. Can show you your stats at any time
7. Can end your session and tell you how much you walked out with

**How it connects to everything:**

```
main.cpp
    │
    └── SessionManager
            │
            ├── Bankroll         (your wallet)
            ├── BlackjackGame    (the actual blackjack game)
            ├── SlotsGame        (the actual slots game)
            ├── RouletteGame     (the actual roulette game)
            │
            ├── BlackjackStats   (blackjack scorecard)
            ├── SlotsStats       (slots scorecard)
            └── RouletteStats    (roulette scorecard)
```

**The flow when you play Blackjack:**
```
1. User picks "Blackjack" from the menu
2. SessionManager.enterBlackjack() is called
   → It creates a BlackjackGame object
   → Copies your current balance into the BlackjackGame
3. You play hands
   → After each hand, SessionManager calls slotsStats.recordRound()... wait, no:
   → After each blackjack hand: blackjackStats.recordRound(summary)
4. When you leave blackjack:
   → SessionManager.cashOutBlackjackToSession() is called
   → Gets final balance from BlackjackGame
   → Calculates the difference (gain or loss)
   → Applies that difference to the main Bankroll
   → Destroys the BlackjackGame object
```

**Terminal-only sections:** Large chunks of `SessionManager.cpp` are marked with:
```cpp
// --- Terminal-only: START ---
...
// --- Terminal-only: END ---
```
These are the parts that print menus, ask for input, and run game loops entirely in text. The GUI (SFML) version does these things visually instead.

---

## `core/SessionStats` — The Day-End Report

**What it is:** A higher-level summary class that was built first, before `SessionManager` existed. Think of it as the original "version 1" manager.

**Current role:** It stores an older, simpler version of overall session data — session duration timer, how many games were played, and a reference to `BlackjackStats`. It's now mostly used as a helper/legacy class.

**What it tracks:**
- A `Bankroll` (its own copy)
- Session start/end time
- Which games were played (flags: `playedBlackjack`, `playedSlots`, `playedRoulette`)
- Total rounds across all games
- A `BlackjackStats` object

**How it's called:**
```
SessionStats created with starting balance
→ startSession() — records the start time
→ playBlackjack() — runs a full blackjack session internally
→ endSession() — stops the timer
→ displaySessionSummary() — prints everything neatly
```

---

## `stats/BlackjackStats` — Blackjack Scorecard

**What it is:** A dedicated notepad for tracking everything that happened during your blackjack session.

**Think of it like:** A detailed scorecard that the blackjack dealer fills out after every hand with:
- Did you win? Lose? Push (tie)?
- How much did you bet?
- How much did you win or lose?
- Did you get a Blackjack (natural 21)?
- Did you bust?
- Are you on a winning streak?

**How it's called:**
```
1. BlackjackStats created at session start with your starting balance
2. You play a hand of blackjack
3. BlackjackGame produces a "RoundSummary" — a small package of data:
       { outcome: "Win", netChange: +50, bankrollAfter: 1050, wasBlackjack: false, ... }
4. SessionManager calls: blackjackStats.recordRound(summary)
5. BlackjackStats updates all its internal counters
6. When you ask for stats: blackjackStats.displayStats() prints everything
```

**What it can show you:**
- Rounds played, wins, losses, pushes
- Win rate percentage
- Number of blackjacks (natural 21s)
- Player busts and dealer busts
- Double-down count and win rate
- Biggest single win and biggest single loss
- Current win/loss streak and longest streaks ever
- Total wagered and return on investment (ROI)
- Peak and lowest bankroll
- Last 10 rounds as a history table

---

## `stats/SlotsStats` — Slots Scorecard

**What it is:** Same concept as `BlackjackStats` but built specifically for the Slots machine.

**How it's called:**
```
1. SlotsStats created at session start
2. You pull the lever (spin)
3. SessionManager checks the result and builds a "SlotsRoundSummary":
       { betAmount: 10, payoutAmount: 0, netChange: -10,
         wasThreeInARow: false, wasJackpot: false }
4. SessionManager calls: slotsStats.recordRound(summary)
5. SlotsStats updates all its counters
```

**What it can show you:**
- Rounds played, wins, losses
- Win rate percentage
- Total wagered and total payout
- Net profit or loss
- Biggest single win and biggest single loss
- Number of 3-in-a-row matches (any symbol)
- Number of jackpots (Bar or Seven triples — the premium hits)

---

## `stats/RouletteStats` — Roulette Scorecard

**What it is:** Same concept but for Roulette.

**How it's called:**
```
1. RouletteStats created at session start
2. You place a bet and the wheel spins
3. SessionManager checks the result and builds a "RouletteRoundSummary":
       { betAmount: 20, payoutAmount: 0, netChange: -20,
         wasStraightUp: true, straightUpWon: false }
4. SessionManager calls: rouletteStats.recordRound(summary)
5. RouletteStats updates all its counters
```

**What it can show you:**
- Rounds played, wins, losses
- Win rate percentage
- Total wagered and total payout
- Net profit or loss
- Biggest single win and biggest single loss
- Number of Straight-Up hits (exact number bets — these pay 35:1)

---

## How It All Calls Together — Full Flow Diagram

```
Program starts (main.cpp)
    │
    ├── User picks GUI → CasinoApp (SFML window opens)
    │       │
    │       └── CasinoApp holds a SessionManager
    │               └── (calls same SessionManager methods)
    │
    └── User picks Terminal → SessionManager.runTerminalSession()
            │
            ├── Prompts for starting balance
            ├── Creates Bankroll($1000)
            ├── Creates BlackjackStats($1000)
            ├── Creates SlotsStats($1000)
            ├── Creates RouletteStats($1000)
            │
            ├── User picks Blackjack
            │   ├── enterBlackjack() → creates BlackjackGame, copies balance in
            │   ├── Each hand → blackjackStats.recordRound(summary)
            │   └── Leave → cashOutBlackjackToSession() → updates Bankroll
            │
            ├── User picks Slots
            │   ├── enterSlots() → creates SlotsGame, copies balance in
            │   ├── Each spin → slotsStats.recordRound(summary)
            │   └── Leave → cashOutSlotsToSession() → updates Bankroll
            │
            ├── User picks Roulette
            │   ├── enterRoulette() → creates RouletteGame, copies balance in
            │   ├── Each spin → rouletteStats.recordRound(summary)
            │   └── Leave → cashOutRouletteToSession() → updates Bankroll
            │
            └── User picks Check Stats
                └── displayTerminalStatsMenu()
                        ├── Option 1 → blackjackStats.displayStats()
                        ├── Option 2 → slotsStats.displayStats()
                        ├── Option 3 → rouletteStats.displayStats()
                        └── Option 4 → displayTerminalSessionSummary()
                                       (pulls data from all 3 stats objects + Bankroll)
```

---

## The "Summary Struct" Pattern

Each game uses a small data package (called a **struct**) to pass results to the stats class. Think of it like filling out a slip of paper after each round:

**BlackjackRoundSummary** — the slip from Blackjack:
```
outcome: Win / Loss / Push / Blackjack
netChange: +50  (how much your balance changed)
betAmount: 50
wasBlackjack: false
playerBusted: false
dealerBusted: true
wasDoubleDown: false
```

**SlotsRoundSummary** — the slip from Slots:
```
betAmount: 10
payoutAmount: 0
netChange: -10
wasThreeInARow: false
wasJackpot: false
```

**RouletteRoundSummary** — the slip from Roulette:
```
betAmount: 20
payoutAmount: 0
netChange: -20
wasStraightUp: true
straightUpWon: false
```

The stats class takes that slip, reads it, and updates its internal scoreboard. It never talks directly to the game — it just reads the slip `SessionManager` hands it.

---

## Why Is It Organized This Way?

This design follows the **Single Responsibility Principle** (each class does one job):

- `Bankroll` only tracks money — it doesn't know about games
- `BlackjackGame` only runs the card game — it doesn't track long-term stats
- `BlackjackStats` only tracks stats — it doesn't run the game
- `SessionManager` is the only class that coordinates all of them
- `main.cpp` just starts the program — it doesn't do any logic itself

This means if you want to change how stats are displayed, you only touch `stats/`. If you want to change game rules, you only touch `games/`. Nothing breaks something else.
