#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <memory>

#include "Bankroll.h"
#include "../blackjack/BlackjackStats.h"

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

    bool enterSlots();
    bool isSlotsAvailable() const;

    void runTerminalSession();

private:
    Bankroll bankroll;
    BlackjackStats blackjackStats;
    std::unique_ptr<BlackjackGame> blackjackGame;

    bool sessionActive;
    ActiveModule activeModule;

    double promptStartingBankroll() const;
    int promptMainMenuChoice() const;
    void showTerminalMainMenu() const;

    void runTerminalBlackjack();
    void runTerminalRoulettePlaceholder();
    void runTerminalSlotsPlaceholder();

    void cashOutBlackjackToSession();
};

#endif