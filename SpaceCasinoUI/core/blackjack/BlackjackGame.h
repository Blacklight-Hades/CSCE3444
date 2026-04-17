/*
    Author:     Ramon Lopez | rjl0157 | ramonlopez2@my.unt.edu
    Team:       Group 11 - Team Galactic - Space Casino
    Course:     CSCE 3444.400 Software Engineering
    Instructor: Bahareh M. Dorri
*/

#ifndef BLACKJACKGAME_H
#define BLACKJACKGAME_H

#include <string>
#include <vector>
#include "Shoe.h"
#include "Hand.h"
#include "BlackjackTypes.h"
#include "BlackjackStrategy.h"

using std::string;
using std::vector;

class BlackjackGame
{
private:
    // Tracks one player hand during a round.
    struct PlayerHandState
    {
        Hand hand;
        double bet = 0.0;
        double payout = 0.0;
        RoundOutcome outcome = RoundOutcome::None;
        vector<string> actionSequence;
        bool finished = false;
        bool stood = false;
        bool doubledDown = false;
        bool cameFromSplit = false;
        bool receivedSplitCard = false;
        bool isSplitAceHand = false;

        // Hint and accuracy tracking.
        StrategyAction recommendedAction = StrategyAction::Hit;
        bool recommendationReady = false;
        bool hintUsedForCurrentDecision = false;
        bool currentDecisionExcludedFromAccuracy = false;

        int optimalActions = 0;
        int suboptimalActions = 0;
        int hintExcludedActions = 0;
        int hintsUsed = 0;

        bool hadSuboptimalCountedAction = false;
        bool hadAnyCountedAction = false;
    };

    Shoe shoe;
    vector<PlayerHandState> playerHands;   // Supports split hands.
    Hand dealerHand;

    // Local funds currently available at the blackjack table.
    double tableBalance;
    double initialBet;
    double totalBetCommitted;
    double payoutAmount;
    double startingBankrollForRound;

    int roundNumber;
    int reshuffleCutoff;
    int activeHandIndex;

    bool dealerHoleCardRevealed;
    bool lastHandBeforeShuffle;
    bool reshufflePending;
    bool reshuffledBeforeCurrentRound;
    bool roundIncludedSplit;

    RoundState roundState;
    RoundOutcome roundOutcome;

    string roundResultText;
    vector<string> roundLog;

    BlackjackStrategy strategyEngine;

    // Session-wide tracking.
    BlackjackAccuracyStats sessionAccuracyStats;
    BlackjackLuckStats sessionLuckStats;

    // Per-round tracking.
    BlackjackAccuracyStats roundAccuracyStats;
    BlackjackLuckStats roundLuckStats;

    void addLogEntry(const string& entry);
    void dealInitialCards();
    void dealCardToHand(PlayerHandState& handState, const string& prefix);
    void revealDealerHoleCard();
    void playDealerTurn();
    void resolveRound();
    void resolveHandOutcome(PlayerHandState& handState, int dealerValue);
    void advanceToNextHandOrDealer();
    bool areAllPlayerHandsFinished() const;
    string buildRoundResultText() const;
    string formatMoney(double amount) const;
    string outcomeToString(RoundOutcome outcome) const;
    bool isValidHandIndex(int handIndex) const;
    bool canDoubleDownOnHand(int handIndex) const;
    bool canSplitHand(int handIndex) const;

    void updateRecommendedActionForActiveHand();
    void beginNewDecisionWindow();
    void recordPlayerDecision(StrategyAction actualAction);
    void applyLuckResult(PlayerHandState& handState);
    StrategyAction getRecommendedActionForHand(int handIndex) const;
    string strategyActionToString(StrategyAction action) const;

public:
    BlackjackGame(double startingBankroll, int numberOfDecks = 6);

    bool startNewRound(double betAmount);
    void resetForNextRound();

    bool canHit() const;
    bool canStand() const;
    bool canDoubleDown() const;
    bool canSplit() const;
    bool canRequestHint() const;

    void playerHit();
    void playerStand();
    void playerDoubleDown();
    void playerSplit();
    void requestHint();

    void setTableBalance(double amount);
    double getTableBalance() const;
    double cashOut() const;

    double getCurrentBet() const;
    double getInitialBet() const;
    double getTotalBetCommitted() const;
    double getPayoutAmount() const;

    int getRoundNumber() const;
    int getCardsRemaining() const;
    int getReshuffleCutoff() const;
    int getPlayerValue() const;
    int getDealerValue() const;
    int getActiveHandIndex() const;
    int getHandCount() const;

    Hand getPlayerHand() const;
    Hand getPlayerHand(int handIndex) const;
    Hand getDealerHand() const;

    double getPlayerHandBet(int handIndex) const;
    bool didPlayerDoubleDown() const;
    bool didPlayerDoubleDown(int handIndex) const;
    bool didRoundIncludeSplit() const;

    RoundState getRoundState() const;
    RoundOutcome getRoundOutcome() const;

    // Main stats handoff for session tracking.
    BlackjackRoundSummary getRoundSummary() const;

    string getRoundResultText() const;
    string getHintText() const;

    BlackjackAccuracyStats getSessionAccuracyStats() const;
    BlackjackLuckStats getSessionLuckStats() const;

    bool isRoundOver() const;
    bool isDealerHoleCardRevealed() const;
    bool isLastHandBeforeShuffle() const;
    bool isReshufflePending() const;
    bool wasShoeReshuffledBeforeCurrentRound() const;

    vector<string> getRoundLog() const;
    void clearRoundLog();
};

#endif