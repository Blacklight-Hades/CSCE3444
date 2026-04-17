#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <memory>

#include "Bankroll.h"
#include "../stats/BlackjackStats.h"
#include "../stats/SlotsStats.h"
#include "../stats/RouletteStats.h"
#include "../games/slots/SlotsGame.h"
#include "../games/roulette/RouletteGame.h"

class BlackjackGame;


class SessionManager
{
public:
    enum class ActiveModule
    {
        None,
        Blackjack,
        Roulette,
        Slots
    };

    explicit SessionManager(double startingBankroll = 1000.0);
    ~SessionManager();

    void startSession(double startingBankroll);
    void endSession();

    bool isSessionActive() const;
    double getCurrentBankroll() const;
    ActiveModule getActiveModule() const;

    void setActiveModule(ActiveModule module);
    void returnToMainMenu();

    bool enterBlackjack();
    bool startBlackjackRound(double betAmount);

    bool canBlackjackHit() const;
    bool canBlackjackStand() const;
    bool canBlackjackDoubleDown() const;
    bool canBlackjackSplit() const;
    bool canBlackjackRequestHint() const;

    bool blackjackHit();
    bool blackjackStand();
    bool blackjackDoubleDown();
    bool blackjackSplit();
    bool blackjackRequestHint();

    bool isBlackjackLoaded() const;
    bool isBlackjackRoundOver() const;

    BlackjackGame* getBlackjackGame();
    const BlackjackGame* getBlackjackGame() const;

    BlackjackStats& getBlackjackStats();
    const BlackjackStats& getBlackjackStats() const;

    void finalizeBlackjackRound();
    bool prepareNextBlackjackRound();

    bool enterRoulette();
    bool isRouletteAvailable() const;
    RouletteGame* getRouletteGame();
    const RouletteGame* getRouletteGame() const;

    bool enterSlots();
    bool isSlotsAvailable() const;
    Slots* getSlotsGame();
    const Slots* getSlotsGame() const;

    void runTerminalSession();

    // --- Terminal-only: Stats display methods ---
    void displayTerminalStatsMenu();
    void displayTerminalBlackjackStats() const;
    void displayTerminalSlotsStats() const;
    void displayTerminalRouletteStats() const;
    void displayTerminalSessionSummary() const;

private:
    Bankroll bankroll;
    BlackjackStats blackjackStats;
    std::unique_ptr<BlackjackGame> blackjackGame;
    std::unique_ptr<RouletteGame> rouletteGame;
    std::unique_ptr<Slots> slotsGame;

    bool sessionActive;
    ActiveModule activeModule;

    // Stats tracking for Slots and Roulette (encapsulated in stats classes)
    SlotsStats slotsStats;
    RouletteStats rouletteStats;

    double promptStartingBankroll() const;
    int promptMainMenuChoice() const;
    void showTerminalMainMenu() const;

    void runTerminalBlackjack();
    void runTerminalRoulettePlaceholder();
    void runTerminalSlotsPlaceholder();

    void cashOutBlackjackToSession();
    void cashOutRouletteToSession();
    void cashOutSlotsToSession();
};

#endif