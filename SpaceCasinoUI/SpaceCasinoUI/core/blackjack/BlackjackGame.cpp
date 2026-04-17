/*
    Author:     Ramon Lopez | rjl0157 | ramonlopez2@my.unt.edu
    Team:       Group 11 - Team Galactic - Space Casino
    Course:     CSCE 3444.400 Software Engineering
    Instructor: Bahareh M. Dorri
*/

#include "BlackjackGame.h"
#include <sstream>
#include <iomanip>
#include <cmath>

using std::fixed;
using std::ostringstream;
using std::setprecision;

BlackjackGame::BlackjackGame(double startingBankroll, int numberOfDecks)
    : shoe(numberOfDecks),
      tableBalance(startingBankroll),
      initialBet(0.0),
      totalBetCommitted(0.0),
      payoutAmount(0.0),
      startingBankrollForRound(0.0),
      roundNumber(0),
      reshuffleCutoff(52),
      activeHandIndex(0),
      dealerHoleCardRevealed(false),
      lastHandBeforeShuffle(false),
      reshufflePending(false),
      reshuffledBeforeCurrentRound(false),
      roundIncludedSplit(false),
      roundState(RoundState::WaitingForBet),
      roundOutcome(RoundOutcome::None),
      roundResultText("")
{
}

// Starts a new round and resets round-specific tracking.
bool BlackjackGame::startNewRound(double betAmount)
{
    if (roundState != RoundState::WaitingForBet && roundState != RoundState::RoundOver)
    {
        return false;
    }

    if (betAmount <= 0.0 || betAmount > tableBalance)
    {
        return false;
    }

    reshuffledBeforeCurrentRound = false;

    if (reshufflePending)
    {
        shoe.reset();
        reshufflePending = false;
        reshuffledBeforeCurrentRound = true;
    }

    playerHands.clear();
    dealerHand.clear();
    roundLog.clear();

    startingBankrollForRound = tableBalance;
    initialBet = betAmount;
    totalBetCommitted = betAmount;
    payoutAmount = 0.0;
    activeHandIndex = 0;
    dealerHoleCardRevealed = false;
    roundOutcome = RoundOutcome::None;
    roundResultText = "";
    roundIncludedSplit = false;
    roundAccuracyStats = BlackjackAccuracyStats{};
    roundLuckStats = BlackjackLuckStats{};

    roundNumber++;

    lastHandBeforeShuffle = shoe.getCardsRemaining() <= reshuffleCutoff;

    tableBalance -= betAmount;
    roundState = RoundState::PlayerTurn;

    addLogEntry("========================================");
    addLogEntry("Round " + std::to_string(roundNumber) + " started.");
    if (reshuffledBeforeCurrentRound)
    {
        addLogEntry("Shoe reshuffled before this round.");
    }

    addLogEntry("Starting table balance before bet: " + formatMoney(startingBankrollForRound));
    addLogEntry("Initial bet placed: " + formatMoney(initialBet));
    addLogEntry("Table balance after initial bet deduction: " + formatMoney(tableBalance));

    if (lastHandBeforeShuffle)
    {
        addLogEntry("Last hand before shuffle.");
    }

    dealInitialCards();

    if (playerHands[0].hand.isBlackjack() || dealerHand.isBlackjack())
    {
        revealDealerHoleCard();
        resolveRound();
    }
    else if (roundState == RoundState::PlayerTurn)
    {
        beginNewDecisionWindow();
    }

    return true;
}

void BlackjackGame::resetForNextRound()
{
    playerHands.clear();
    dealerHand.clear();
    roundLog.clear();

    initialBet = 0.0;
    totalBetCommitted = 0.0;
    payoutAmount = 0.0;
    startingBankrollForRound = 0.0;

    activeHandIndex = 0;

    dealerHoleCardRevealed = false;
    lastHandBeforeShuffle = false;
    reshuffledBeforeCurrentRound = false;
    roundIncludedSplit = false;

    roundOutcome = RoundOutcome::None;
    roundResultText.clear();

    roundAccuracyStats = BlackjackAccuracyStats{};
    roundLuckStats = BlackjackLuckStats{};

    roundState = RoundState::WaitingForBet;
}

void BlackjackGame::addLogEntry(const string& entry)
{
    roundLog.push_back(entry);
}

// Deals opening cards to player and dealer.
void BlackjackGame::dealInitialCards()
{
    playerHands.push_back(PlayerHandState{});
    playerHands[0].bet = initialBet;

    Card playerCard1 = shoe.dealCard();
    playerHands[0].hand.addCard(playerCard1);
    addLogEntry("Player hand 1 dealt: " + playerCard1.toString());

    Card dealerCard1 = shoe.dealCard();
    dealerHand.addCard(dealerCard1);
    addLogEntry("Dealer dealt: " + dealerCard1.toString() + " (up card)");

    Card playerCard2 = shoe.dealCard();
    playerHands[0].hand.addCard(playerCard2);
    addLogEntry("Player hand 1 dealt: " + playerCard2.toString());

    Card dealerCard2 = shoe.dealCard();
    dealerHand.addCard(dealerCard2);
    addLogEntry("Dealer dealt: " + dealerCard2.toString() + " (hole card)");

    addLogEntry("Player starting hand 1: " + playerHands[0].hand.toString() +
                " | value = " + std::to_string(playerHands[0].hand.getValue()));
    addLogEntry("Dealer showing: " + dealerCard1.toString());
}

void BlackjackGame::dealCardToHand(PlayerHandState& handState, const string& prefix)
{
    Card newCard = shoe.dealCard();
    handState.hand.addCard(newCard);
    addLogEntry(prefix + newCard.toString());
}

// Reveals dealer hole card once.
void BlackjackGame::revealDealerHoleCard()
{
    if (!dealerHoleCardRevealed)
    {
        dealerHoleCardRevealed = true;
        addLogEntry("Dealer hole card revealed. Dealer hand: " + dealerHand.toString() +
                    " | value = " + std::to_string(dealerHand.getValue()));
    }
}

// Updates the strategy recommendation for the active hand.
void BlackjackGame::updateRecommendedActionForActiveHand()
{
    if (!isValidHandIndex(activeHandIndex))
    {
        return;
    }

    PlayerHandState& handState = playerHands[activeHandIndex];

    if (dealerHand.getCards().empty())
    {
        return;
    }

    handState.recommendedAction = strategyEngine.getRecommendedAction(
        handState.hand,
        dealerHand.getCards()[0],
        canDoubleDownOnHand(activeHandIndex),
        canSplitHand(activeHandIndex)
    );

    handState.recommendationReady = true;
}

// Starts a new decision window for hint and accuracy tracking.
void BlackjackGame::beginNewDecisionWindow()
{
    if (!isValidHandIndex(activeHandIndex))
    {
        return;
    }

    PlayerHandState& handState = playerHands[activeHandIndex];
    handState.hintUsedForCurrentDecision = false;
    handState.currentDecisionExcludedFromAccuracy = false;
    handState.recommendationReady = false;

    updateRecommendedActionForActiveHand();
}

// Records whether the player followed basic strategy.
void BlackjackGame::recordPlayerDecision(StrategyAction actualAction)
{
    if (!isValidHandIndex(activeHandIndex))
    {
        return;
    }

    PlayerHandState& handState = playerHands[activeHandIndex];

    if (!handState.recommendationReady)
    {
        return;
    }

    if (handState.currentDecisionExcludedFromAccuracy)
    {
        return;
    }

    handState.hadAnyCountedAction = true;

    if (actualAction == handState.recommendedAction)
    {
        handState.optimalActions++;
        roundAccuracyStats.optimalActions++;
        sessionAccuracyStats.optimalActions++;
    }
    else
    {
        handState.suboptimalActions++;
        handState.hadSuboptimalCountedAction = true;
        roundAccuracyStats.suboptimalActions++;
        sessionAccuracyStats.suboptimalActions++;
    }
}

// Applies lucky or unlucky outcome tracking based on decision quality.
void BlackjackGame::applyLuckResult(PlayerHandState& handState)
{
    if (!handState.hadAnyCountedAction)
    {
        return;
    }

    bool playerWonHand =
        handState.outcome == RoundOutcome::PlayerWin ||
        handState.outcome == RoundOutcome::PlayerBlackjack ||
        handState.outcome == RoundOutcome::DealerBust;

    bool playerPushed = handState.outcome == RoundOutcome::Push;

    bool playerLostHand =
        handState.outcome == RoundOutcome::DealerWin ||
        handState.outcome == RoundOutcome::DealerBlackjack ||
        handState.outcome == RoundOutcome::PlayerBust;

    if (handState.hadSuboptimalCountedAction)
    {
        if (playerWonHand)
        {
            roundLuckStats.luckyHands++;
            roundLuckStats.luckyScore += 1.0;
            sessionLuckStats.luckyHands++;
            sessionLuckStats.luckyScore += 1.0;
        }
        else if (playerPushed)
        {
            roundLuckStats.semiLuckyPushes++;
            roundLuckStats.luckyScore += 0.5;
            sessionLuckStats.semiLuckyPushes++;
            sessionLuckStats.luckyScore += 0.5;
        }
    }
    else if (playerLostHand)
    {
        roundLuckStats.unluckyHands++;
        roundLuckStats.unluckyScore += 1.0;
        sessionLuckStats.unluckyHands++;
        sessionLuckStats.unluckyScore += 1.0;
    }
}

StrategyAction BlackjackGame::getRecommendedActionForHand(int handIndex) const
{
    if (!isValidHandIndex(handIndex))
    {
        return StrategyAction::Hit;
    }

    return playerHands[handIndex].recommendedAction;
}

string BlackjackGame::strategyActionToString(StrategyAction action) const
{
    return BlackjackStrategy::actionToString(action);
}

bool BlackjackGame::canHit() const
{
    return roundState == RoundState::PlayerTurn &&
           isValidHandIndex(activeHandIndex) &&
           !playerHands[activeHandIndex].finished &&
           !playerHands[activeHandIndex].hand.isBust() &&
           !playerHands[activeHandIndex].isSplitAceHand;
}

bool BlackjackGame::canStand() const
{
    return roundState == RoundState::PlayerTurn &&
           isValidHandIndex(activeHandIndex) &&
           !playerHands[activeHandIndex].finished;
}

// Enforces double down rules for normal and split hands.
bool BlackjackGame::canDoubleDownOnHand(int handIndex) const
{
    if (roundState != RoundState::PlayerTurn || !isValidHandIndex(handIndex))
    {
        return false;
    }

    const PlayerHandState& handState = playerHands[handIndex];

    if (handState.finished ||
        handState.doubledDown ||
        handState.isSplitAceHand ||
        tableBalance < handState.bet)
    {
        return false;
    }

    if (handState.cameFromSplit)
    {
        return handState.receivedSplitCard && handState.hand.getCardCount() == 2;
    }

    return handState.hand.getCardCount() == 2;
}

bool BlackjackGame::canDoubleDown() const
{
    return canDoubleDownOnHand(activeHandIndex);
}

bool BlackjackGame::canSplitHand(int handIndex) const
{
    if (roundState != RoundState::PlayerTurn || !isValidHandIndex(handIndex))
    {
        return false;
    }

    const PlayerHandState& handState = playerHands[handIndex];

    return !handState.finished &&
           !handState.doubledDown &&
           handState.hand.canSplit() &&
           tableBalance >= handState.bet;
}

bool BlackjackGame::canSplit() const
{
    return canSplitHand(activeHandIndex);
}

bool BlackjackGame::canRequestHint() const
{
    return roundState == RoundState::PlayerTurn &&
           isValidHandIndex(activeHandIndex) &&
           !playerHands[activeHandIndex].finished &&
           playerHands[activeHandIndex].recommendationReady;
}

string BlackjackGame::getHintText() const
{
    if (!canRequestHint())
    {
        return "Hint is not available right now.";
    }

    const PlayerHandState& handState = playerHands[activeHandIndex];
    return "Suggested move: " + strategyActionToString(handState.recommendedAction);
}

// Logs hint usage and excludes that decision from accuracy stats.
void BlackjackGame::requestHint()
{
    if (!canRequestHint())
    {
        return;
    }

    PlayerHandState& handState = playerHands[activeHandIndex];

    if (handState.hintUsedForCurrentDecision)
    {
        addLogEntry("Hint already used for this decision on hand " + std::to_string(activeHandIndex + 1) + ".");
        return;
    }

    handState.hintUsedForCurrentDecision = true;
    handState.currentDecisionExcludedFromAccuracy = true;
    handState.hintExcludedActions++;
    handState.hintsUsed++;

    roundAccuracyStats.hintExcludedActions++;
    roundAccuracyStats.hintsUsed++;

    sessionAccuracyStats.hintExcludedActions++;
    sessionAccuracyStats.hintsUsed++;

    addLogEntry("Hint requested for hand " + std::to_string(activeHandIndex + 1) +
                ". Recommended action: " + strategyActionToString(handState.recommendedAction));
}

// Handles player hit logic for the active hand.
void BlackjackGame::playerHit()
{
    if (!canHit())
    {
        return;
    }

    PlayerHandState& handState = playerHands[activeHandIndex];

    recordPlayerDecision(StrategyAction::Hit);

    addLogEntry("Player hand " + std::to_string(activeHandIndex + 1) + " action: Hit");
    handState.actionSequence.push_back("Hit");
    dealCardToHand(handState, "Player hand " + std::to_string(activeHandIndex + 1) + " draws: ");
    addLogEntry("Player hand " + std::to_string(activeHandIndex + 1) + ": " + handState.hand.toString() +
                " | value = " + std::to_string(handState.hand.getValue()));

    if (handState.hand.isBust())
    {
        handState.finished = true;
        addLogEntry("Player hand " + std::to_string(activeHandIndex + 1) + " busts.");
        advanceToNextHandOrDealer();
    }
    else
    {
        beginNewDecisionWindow();
    }
}

// Handles player stand logic for the active hand.
void BlackjackGame::playerStand()
{
    if (!canStand())
    {
        return;
    }

    PlayerHandState& handState = playerHands[activeHandIndex];

    recordPlayerDecision(StrategyAction::Stand);

    addLogEntry("Player hand " + std::to_string(activeHandIndex + 1) + " action: Stand");
    handState.actionSequence.push_back("Stand");
    handState.stood = true;
    handState.finished = true;

    advanceToNextHandOrDealer();
}

// Handles double down logic and finishes the hand.
void BlackjackGame::playerDoubleDown()
{
    if (!canDoubleDown())
    {
        return;
    }

    PlayerHandState& handState = playerHands[activeHandIndex];

    recordPlayerDecision(StrategyAction::Double);

    addLogEntry("Player hand " + std::to_string(activeHandIndex + 1) + " action: Double Down");
    handState.actionSequence.push_back("DoubleDown");

    tableBalance -= handState.bet;
    totalBetCommitted += handState.bet;
    handState.bet *= 2.0;
    handState.doubledDown = true;

    addLogEntry("Player hand " + std::to_string(activeHandIndex + 1) + " bet doubled to: " + formatMoney(handState.bet));
    addLogEntry("Table balance after double down deduction: " + formatMoney(tableBalance));

    dealCardToHand(handState, "Player hand " + std::to_string(activeHandIndex + 1) + " draws: ");
    addLogEntry("Player hand " + std::to_string(activeHandIndex + 1) + ": " + handState.hand.toString() +
                " | value = " + std::to_string(handState.hand.getValue()));

    if (handState.hand.isBust())
    {
        addLogEntry("Player hand " + std::to_string(activeHandIndex + 1) + " busts after double down.");
    }
    else
    {
        addLogEntry("Double down completes player hand " + std::to_string(activeHandIndex + 1) + ".");
    }

    handState.finished = true;
    advanceToNextHandOrDealer();
}

// Splits the active hand into two playable hands.
void BlackjackGame::playerSplit()
{
    if (!canSplit())
    {
        return;
    }

    PlayerHandState& originalHand = playerHands[activeHandIndex];
    double newHandBet = originalHand.bet;

    vector<Card> originalCards = originalHand.hand.getCards();
    bool splittingAces =
        originalCards.size() == 2 &&
        originalCards[0].getRank() == Rank::Ace &&
        originalCards[1].getRank() == Rank::Ace;

    recordPlayerDecision(StrategyAction::Split);

    tableBalance -= newHandBet;
    totalBetCommitted += newHandBet;
    roundIncludedSplit = true;

    Card movedCard = originalHand.hand.removeCardAt(1);

    PlayerHandState splitHand;
    splitHand.bet = newHandBet;
    splitHand.cameFromSplit = true;
    splitHand.hand.addCard(movedCard);
    splitHand.isSplitAceHand = splittingAces;
    splitHand.recommendationReady = false;
    splitHand.hintUsedForCurrentDecision = false;
    splitHand.currentDecisionExcludedFromAccuracy = false;

    originalHand.cameFromSplit = true;
    originalHand.receivedSplitCard = false;
    originalHand.isSplitAceHand = splittingAces;
    originalHand.recommendationReady = false;
    originalHand.hintUsedForCurrentDecision = false;
    originalHand.currentDecisionExcludedFromAccuracy = false;
    originalHand.actionSequence.push_back("Split");

    playerHands.insert(playerHands.begin() + activeHandIndex + 1, splitHand);

    addLogEntry("Player hand " + std::to_string(activeHandIndex + 1) + " action: Split");
    addLogEntry("Additional split bet placed: " + formatMoney(newHandBet));
    addLogEntry("Table balance after split deduction: " + formatMoney(tableBalance));

    dealCardToHand(playerHands[activeHandIndex],
                   "Player hand " + std::to_string(activeHandIndex + 1) + " receives split card: ");
    playerHands[activeHandIndex].receivedSplitCard = true;
    addLogEntry("Player hand " + std::to_string(activeHandIndex + 1) + ": " +
                playerHands[activeHandIndex].hand.toString() +
                " | value = " + std::to_string(playerHands[activeHandIndex].hand.getValue()));

    dealCardToHand(playerHands[activeHandIndex + 1],
                   "Player hand " + std::to_string(activeHandIndex + 2) + " receives split card: ");
    playerHands[activeHandIndex + 1].receivedSplitCard = true;
    addLogEntry("Player hand " + std::to_string(activeHandIndex + 2) + ": " +
                playerHands[activeHandIndex + 1].hand.toString() +
                " | value = " + std::to_string(playerHands[activeHandIndex + 1].hand.getValue()));

    if (splittingAces)
    {
        playerHands[activeHandIndex].finished = true;
        playerHands[activeHandIndex + 1].finished = true;

        addLogEntry("Split aces rule applied: each split ace hand receives one card only.");
        addLogEntry("No further hit or double down is allowed on split aces.");

        advanceToNextHandOrDealer();
    }
    else
    {
        beginNewDecisionWindow();
    }
}

// Plays dealer hand using stand-on-17 rules.
void BlackjackGame::playDealerTurn()
{
    revealDealerHoleCard();

    while (dealerHand.getValue() < 17)
    {
        Card newCard = shoe.dealCard();
        dealerHand.addCard(newCard);

        addLogEntry("Dealer draws: " + newCard.toString());
        addLogEntry("Dealer hand: " + dealerHand.toString() +
                    " | value = " + std::to_string(dealerHand.getValue()));
    }

    int dealerValue = dealerHand.getValue();

    if (dealerHand.isBust())
    {
        addLogEntry("Dealer busts with value = " + std::to_string(dealerValue));
    }
    else
    {
        addLogEntry("Dealer stands with value = " + std::to_string(dealerValue));
    }
}

// Resolves payout and outcome for one player hand.
void BlackjackGame::resolveHandOutcome(PlayerHandState& handState, int dealerValue)
{
    int playerValue = handState.hand.getValue();
    handState.payout = 0.0;

    if (handState.hand.isBust())
    {
        handState.outcome = RoundOutcome::PlayerBust;
        handState.payout = 0.0;
    }
    else if (dealerHand.isBlackjack() && !handState.hand.isBlackjack())
    {
        handState.outcome = RoundOutcome::DealerBlackjack;
        handState.payout = 0.0;
    }
    else if (dealerHand.isBust())
    {
        handState.outcome = RoundOutcome::DealerBust;
        handState.payout = handState.bet * 2.0;
    }
    else if (playerValue > dealerValue)
    {
        handState.outcome = RoundOutcome::PlayerWin;
        handState.payout = handState.bet * 2.0;
    }
    else if (dealerValue > playerValue)
    {
        handState.outcome = RoundOutcome::DealerWin;
        handState.payout = 0.0;
    }
    else
    {
        handState.outcome = RoundOutcome::Push;
        handState.payout = handState.bet;
    }

    tableBalance += handState.payout;
}

bool BlackjackGame::areAllPlayerHandsFinished() const
{
    for (const PlayerHandState& handState : playerHands)
    {
        if (!handState.finished)
        {
            return false;
        }
    }

    return true;
}

// Advances through split hands, then transitions to dealer play.
void BlackjackGame::advanceToNextHandOrDealer()
{
    if (!areAllPlayerHandsFinished())
    {
        for (int i = activeHandIndex + 1; i < static_cast<int>(playerHands.size()); i++)
        {
            if (!playerHands[i].finished)
            {
                activeHandIndex = i;
                addLogEntry("Player turn moves to hand " + std::to_string(activeHandIndex + 1) + ".");
                beginNewDecisionWindow();
                return;
            }
        }

        for (int i = 0; i < activeHandIndex; i++)
        {
            if (!playerHands[i].finished)
            {
                activeHandIndex = i;
                addLogEntry("Player turn moves to hand " + std::to_string(activeHandIndex + 1) + ".");
                beginNewDecisionWindow();
                return;
            }
        }
    }

    roundState = RoundState::DealerTurn;

    bool anyLiveHand = false;
    for (const PlayerHandState& handState : playerHands)
    {
        if (!handState.hand.isBust())
        {
            anyLiveHand = true;
            break;
        }
    }

    if (anyLiveHand)
    {
        playDealerTurn();
    }
    else
    {
        revealDealerHoleCard();
        addLogEntry("All player hands busted. Dealer turn skipped.");
    }

    resolveRound();
}

// Resolves all hands and finalizes round results.
void BlackjackGame::resolveRound()
{
    payoutAmount = 0.0;

    if (playerHands.size() == 1 && playerHands[0].hand.isBlackjack() && dealerHand.isBlackjack())
    {
        playerHands[0].outcome = RoundOutcome::Push;
        playerHands[0].payout = playerHands[0].bet;
        tableBalance += playerHands[0].payout;
        payoutAmount += playerHands[0].payout;
        playerHands[0].finished = true;
        roundOutcome = RoundOutcome::Push;
        applyLuckResult(playerHands[0]);
    }
    else if (playerHands.size() == 1 && playerHands[0].hand.isBlackjack())
    {
        playerHands[0].outcome = RoundOutcome::PlayerBlackjack;
        playerHands[0].payout = playerHands[0].bet * 2.5;
        tableBalance += playerHands[0].payout;
        payoutAmount += playerHands[0].payout;
        playerHands[0].finished = true;
        roundOutcome = RoundOutcome::PlayerBlackjack;
        applyLuckResult(playerHands[0]);
    }
    else if (dealerHand.isBlackjack() && playerHands.size() == 1)
    {
        playerHands[0].outcome = RoundOutcome::DealerBlackjack;
        playerHands[0].payout = 0.0;
        playerHands[0].finished = true;
        roundOutcome = RoundOutcome::DealerBlackjack;
        applyLuckResult(playerHands[0]);
    }
    else
    {
        bool anyPlayerWin = false;
        bool anyPush = false;
        bool anyDealerWin = false;
        bool allBust = true;

        int dealerValue = dealerHand.getValue();

        for (PlayerHandState& handState : playerHands)
        {
            resolveHandOutcome(handState, dealerValue);
            applyLuckResult(handState);
            payoutAmount += handState.payout;

            if (!handState.hand.isBust())
            {
                allBust = false;
            }

            if (handState.outcome == RoundOutcome::PlayerWin || handState.outcome == RoundOutcome::DealerBust)
            {
                anyPlayerWin = true;
            }
            else if (handState.outcome == RoundOutcome::Push)
            {
                anyPush = true;
            }
            else
            {
                anyDealerWin = true;
            }
        }

        if (allBust)
        {
            roundOutcome = RoundOutcome::PlayerBust;
        }
        else if (dealerHand.isBust())
        {
            roundOutcome = RoundOutcome::DealerBust;
        }
        else if (anyPlayerWin)
        {
            roundOutcome = RoundOutcome::PlayerWin;
        }
        else if (anyPush)
        {
            roundOutcome = RoundOutcome::Push;
        }
        else if (dealerHand.isBlackjack())
        {
            roundOutcome = RoundOutcome::DealerBlackjack;
        }
        else if (anyDealerWin)
        {
            roundOutcome = RoundOutcome::DealerWin;
        }
        else
        {
            roundOutcome = RoundOutcome::None;
        }
    }

    for (int i = 0; i < static_cast<int>(playerHands.size()); i++)
    {
        addLogEntry("Final player hand " + std::to_string(i + 1) + ": " + playerHands[i].hand.toString() +
                    " | value = " + std::to_string(playerHands[i].hand.getValue()) +
                    " | outcome = " + outcomeToString(playerHands[i].outcome) +
                    " | payout = " + formatMoney(playerHands[i].payout));
    }

    addLogEntry("Final dealer hand: " + dealerHand.toString() +
                " | value = " + std::to_string(dealerHand.getValue()));
    addLogEntry("Round outcome: " + outcomeToString(roundOutcome));
    addLogEntry("Total payout returned to table balance: " + formatMoney(payoutAmount));
    addLogEntry("Ending table balance: " + formatMoney(tableBalance));

    if (lastHandBeforeShuffle)
    {
        reshufflePending = true;
        addLogEntry("Shoe will be reshuffled before the next round.");
    }

    roundResultText = buildRoundResultText();
    roundState = RoundState::RoundOver;
}

// Builds the full round result text shown to the player.
string BlackjackGame::buildRoundResultText() const
{
    string result;
    int dealerValue = dealerHand.getValue();

    result += "Dealer:\n";
    result += "\t" + dealerHand.toString() + " (" + std::to_string(dealerValue) + ")\n\n";

    result += "Player Hands:\n";
    for (int i = 0; i < static_cast<int>(playerHands.size()); i++)
    {
        const PlayerHandState& handState = playerHands[i];

        result += "\tHand " + std::to_string(i + 1) + ": ";
        result += handState.hand.toString();
        result += " (" + std::to_string(handState.hand.getValue()) + ")";
        result += "\tBet: " + formatMoney(handState.bet);

        if (handState.doubledDown)
        {
            result += "\t[Doubled]";
        }

        result += "\n";
    }

    result += "\nResults:\n";
    for (int i = 0; i < static_cast<int>(playerHands.size()); i++)
    {
        const PlayerHandState& handState = playerHands[i];
        int playerValue = handState.hand.getValue();

        result += "\tHand " + std::to_string(i + 1) + ": ";

        switch (handState.outcome)
        {
            case RoundOutcome::PlayerBlackjack:
                result += "Player Blackjack (Player: " + std::to_string(playerValue) +
                          " vs Dealer: " + std::to_string(dealerValue) + ")";
                break;

            case RoundOutcome::DealerBlackjack:
                result += "Dealer Blackjack (Player: " + std::to_string(playerValue) +
                          " vs Dealer: " + std::to_string(dealerValue) + ")";
                break;

            case RoundOutcome::PlayerBust:
                result += "Player Bust (Player: " + std::to_string(playerValue) +
                          " vs Dealer: " + std::to_string(dealerValue) + ")";
                break;

            case RoundOutcome::DealerBust:
                result += "Player Win (Dealer Bust, Player: " + std::to_string(playerValue) + ")";
                break;

            case RoundOutcome::PlayerWin:
                result += "Player Win (Player: " + std::to_string(playerValue) +
                          " vs Dealer: " + std::to_string(dealerValue) + ")";
                break;

            case RoundOutcome::DealerWin:
                result += "Dealer Win (Player: " + std::to_string(playerValue) +
                          " vs Dealer: " + std::to_string(dealerValue) + ")";
                break;

            case RoundOutcome::Push:
                result += "Push (Player: " + std::to_string(playerValue) +
                          " vs Dealer: " + std::to_string(dealerValue) + ")";
                break;

            case RoundOutcome::None:
            default:
                result += "No Result";
                break;
        }

        if (i < static_cast<int>(playerHands.size()) - 1)
        {
            result += "\n";
        }
    }

    result += "\n\nRound Insight:\n";

    bool hadHintOnlyRound =
        roundAccuracyStats.optimalActions == 0 &&
        roundAccuracyStats.suboptimalActions == 0 &&
        roundAccuracyStats.hintsUsed > 0;

    bool badOutcomeOccurred = false;
    bool goodOutcomeOccurred = false;

    for (const PlayerHandState& handState : playerHands)
    {
        if (handState.outcome == RoundOutcome::DealerWin ||
            handState.outcome == RoundOutcome::DealerBlackjack ||
            handState.outcome == RoundOutcome::PlayerBust)
        {
            badOutcomeOccurred = true;
        }

        if (handState.outcome == RoundOutcome::PlayerWin ||
            handState.outcome == RoundOutcome::PlayerBlackjack ||
            handState.outcome == RoundOutcome::DealerBust)
        {
            goodOutcomeOccurred = true;
        }
    }

    // Prioritizes luck-based insight before strategy-based insight.
    if (roundLuckStats.luckyHands > 0)
    {
        result += "\tYou got lucky that hand.";
    }
    else if (roundLuckStats.semiLuckyPushes > 0)
    {
        result += "\tYou got a little lucky to escape with a push.";
    }
    else if (roundLuckStats.unluckyHands > 0)
    {
        result += "\tThat was an unlucky hand.";
    }
    else if (hadHintOnlyRound && badOutcomeOccurred)
    {
        result += "\tThat was an unlucky outcome, even with the hint.";
    }
    else if (roundAccuracyStats.suboptimalActions > 0)
    {
        result += "\tYou did not play that hand optimally.";
    }
    else if (roundAccuracyStats.optimalActions > 0)
    {
        result += "\tYou played that hand well.";
    }
    else if (hadHintOnlyRound && goodOutcomeOccurred)
    {
        result += "\tThe hint helped guide that hand.";
    }
    else
    {
        result += "\tNo major insight for this hand.";
    }

    result += "\n\n----------------------------------------";
    result += "\nTotal Wager:\t" + formatMoney(totalBetCommitted);
    result += "\nPayout:\t\t" + formatMoney(payoutAmount);
    result += "\nNet Change:\t" + formatMoney(payoutAmount - totalBetCommitted);
    result += "\nTable Balance:\t" + formatMoney(tableBalance);
    result += "\n----------------------------------------";

    return result;
}

// Formats money with dollar sign and two decimals.
string BlackjackGame::formatMoney(double amount) const
{
    ostringstream oss;

    if (amount < 0.0)
    {
        oss << "-$" << fixed << setprecision(2) << std::fabs(amount);
    }
    else
    {
        oss << "$" << fixed << setprecision(2) << amount;
    }

    return oss.str();
}

// Converts round outcomes to display text.
string BlackjackGame::outcomeToString(RoundOutcome outcome) const
{
    switch (outcome)
    {
        case RoundOutcome::PlayerWin:
            return "Player Win";
        case RoundOutcome::DealerWin:
            return "Dealer Win";
        case RoundOutcome::Push:
            return "Push";
        case RoundOutcome::PlayerBlackjack:
            return "Player Blackjack";
        case RoundOutcome::PlayerBust:
            return "Player Bust";
        case RoundOutcome::DealerBust:
            return "Dealer Bust";
        case RoundOutcome::DealerBlackjack:
            return "Dealer Blackjack";
        case RoundOutcome::None:
        default:
            return "None";
    }
}

bool BlackjackGame::isValidHandIndex(int handIndex) const
{
    return handIndex >= 0 && handIndex < static_cast<int>(playerHands.size());
}

void BlackjackGame::setTableBalance(double amount)
{
    if (amount >= 0.0)
    {
        tableBalance = amount;
    }
}

double BlackjackGame::getTableBalance() const
{
    return tableBalance;
}

double BlackjackGame::cashOut() const
{
    return tableBalance;
}

double BlackjackGame::getCurrentBet() const
{
    if (!isValidHandIndex(activeHandIndex))
    {
        return 0.0;
    }

    return playerHands[activeHandIndex].bet;
}

double BlackjackGame::getInitialBet() const
{
    return initialBet;
}

double BlackjackGame::getTotalBetCommitted() const
{
    return totalBetCommitted;
}

double BlackjackGame::getPayoutAmount() const
{
    return payoutAmount;
}

int BlackjackGame::getRoundNumber() const
{
    return roundNumber;
}

int BlackjackGame::getCardsRemaining() const
{
    return shoe.getCardsRemaining();
}

int BlackjackGame::getReshuffleCutoff() const
{
    return reshuffleCutoff;
}

int BlackjackGame::getPlayerValue() const
{
    if (!isValidHandIndex(activeHandIndex))
    {
        return 0;
    }

    return playerHands[activeHandIndex].hand.getValue();
}

int BlackjackGame::getDealerValue() const
{
    return dealerHand.getValue();
}

int BlackjackGame::getActiveHandIndex() const
{
    return activeHandIndex;
}

int BlackjackGame::getHandCount() const
{
    return static_cast<int>(playerHands.size());
}

Hand BlackjackGame::getPlayerHand() const
{
    if (!isValidHandIndex(activeHandIndex))
    {
        return Hand();
    }

    return playerHands[activeHandIndex].hand;
}

Hand BlackjackGame::getPlayerHand(int handIndex) const
{
    if (!isValidHandIndex(handIndex))
    {
        return Hand();
    }

    return playerHands[handIndex].hand;
}

Hand BlackjackGame::getDealerHand() const
{
    return dealerHand;
}

double BlackjackGame::getPlayerHandBet(int handIndex) const
{
    if (!isValidHandIndex(handIndex))
    {
        return 0.0;
    }

    return playerHands[handIndex].bet;
}

bool BlackjackGame::didPlayerDoubleDown() const
{
    for (const PlayerHandState& handState : playerHands)
    {
        if (handState.doubledDown)
        {
            return true;
        }
    }

    return false;
}

bool BlackjackGame::didPlayerDoubleDown(int handIndex) const
{
    return isValidHandIndex(handIndex) && playerHands[handIndex].doubledDown;
}

bool BlackjackGame::didRoundIncludeSplit() const
{
    return roundIncludedSplit;
}

RoundState BlackjackGame::getRoundState() const
{
    return roundState;
}

RoundOutcome BlackjackGame::getRoundOutcome() const
{
    return roundOutcome;
}

// Builds structured round data for the stats module.
BlackjackRoundSummary BlackjackGame::getRoundSummary() const
{
    BlackjackRoundSummary summary;

    summary.roundNumber = roundNumber;
    summary.startingBankroll = startingBankrollForRound;
    summary.initialBetAmount = initialBet;
    summary.totalBetCommitted = totalBetCommitted;
    summary.bankrollAfterBetDeduction = startingBankrollForRound - totalBetCommitted;
    summary.payoutAmount = payoutAmount;
    summary.endingBankroll = tableBalance;
    summary.netChange = tableBalance - startingBankrollForRound;
    summary.roundOutcome = roundOutcome;
    summary.dealerFinalValue = dealerHand.getValue();
    summary.activeHandIndexAtRoundEnd = activeHandIndex;
    summary.wasNaturalBlackjack = (roundOutcome == RoundOutcome::PlayerBlackjack);
    summary.dealerBusted = (roundOutcome == RoundOutcome::DealerBust);
    summary.roundIncludedSplit = roundIncludedSplit;
    summary.accuracyStats = roundAccuracyStats;
    summary.luckStats = roundLuckStats;

    summary.playerWon = false;
    summary.dealerWon = false;
    summary.push = false;

    for (const PlayerHandState& handState : playerHands)
    {
        SplitHandState splitSummary;
        splitSummary.betAmount = handState.bet;
        splitSummary.payoutAmount = handState.payout;
        splitSummary.finalValue = handState.hand.getValue();
        splitSummary.handIndex = static_cast<int>(summary.playerHands.size());
        splitSummary.outcome = handState.outcome;
        splitSummary.won = (handState.outcome == RoundOutcome::PlayerWin ||
                            handState.outcome == RoundOutcome::PlayerBlackjack ||
                            handState.outcome == RoundOutcome::DealerBust);
        splitSummary.push = (handState.outcome == RoundOutcome::Push);
        splitSummary.busted = (handState.outcome == RoundOutcome::PlayerBust);
        splitSummary.doubledDown = handState.doubledDown;
        splitSummary.cameFromSplit = handState.cameFromSplit;
        splitSummary.finished = handState.finished;
        splitSummary.actionSequence = handState.actionSequence;

        splitSummary.optimalActions = handState.optimalActions;
        splitSummary.suboptimalActions = handState.suboptimalActions;
        splitSummary.hintExcludedActions = handState.hintExcludedActions;
        splitSummary.hintsUsed = handState.hintsUsed;
        splitSummary.hadSuboptimalCountedAction = handState.hadSuboptimalCountedAction;
        splitSummary.hadOnlyOptimalCountedActions =
            handState.hadAnyCountedAction && !handState.hadSuboptimalCountedAction;

        splitSummary.wasLucky =
            handState.hadSuboptimalCountedAction &&
            (handState.outcome == RoundOutcome::PlayerWin ||
             handState.outcome == RoundOutcome::PlayerBlackjack ||
             handState.outcome == RoundOutcome::DealerBust);

        splitSummary.wasSemiLucky =
            handState.hadSuboptimalCountedAction &&
            handState.outcome == RoundOutcome::Push;

        splitSummary.wasUnlucky =
            handState.hadAnyCountedAction &&
            !handState.hadSuboptimalCountedAction &&
            (handState.outcome == RoundOutcome::DealerWin ||
             handState.outcome == RoundOutcome::DealerBlackjack ||
             handState.outcome == RoundOutcome::PlayerBust);

        for (const Card& card : handState.hand.getCards())
        {
            splitSummary.cards.push_back(card.toString());
        }

        summary.playerHands.push_back(splitSummary);

        if (splitSummary.won)
        {
            summary.playerWon = true;
        }
        else if (splitSummary.push)
        {
            summary.push = true;
        }
        else
        {
            summary.dealerWon = true;
        }
    }

    for (const Card& card : dealerHand.getCards())
    {
        summary.dealerCards.push_back(card.toString());
    }

    summary.lastHandBeforeShuffle = lastHandBeforeShuffle;
    summary.reshuffledBeforeRound = reshuffledBeforeCurrentRound;
    summary.cardsRemainingAfterRound = shoe.getCardsRemaining();

    return summary;
}

string BlackjackGame::getRoundResultText() const
{
    return roundResultText;
}

BlackjackAccuracyStats BlackjackGame::getSessionAccuracyStats() const
{
    return sessionAccuracyStats;
}

BlackjackLuckStats BlackjackGame::getSessionLuckStats() const
{
    return sessionLuckStats;
}

bool BlackjackGame::isRoundOver() const
{
    return roundState == RoundState::RoundOver;
}

bool BlackjackGame::isDealerHoleCardRevealed() const
{
    return dealerHoleCardRevealed;
}

bool BlackjackGame::isLastHandBeforeShuffle() const
{
    return lastHandBeforeShuffle;
}

bool BlackjackGame::isReshufflePending() const
{
    return reshufflePending;
}

bool BlackjackGame::wasShoeReshuffledBeforeCurrentRound() const
{
    return reshuffledBeforeCurrentRound;
}

vector<string> BlackjackGame::getRoundLog() const
{
    return roundLog;
}

void BlackjackGame::clearRoundLog()
{
    roundLog.clear();
}