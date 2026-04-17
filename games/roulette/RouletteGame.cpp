#include "RouletteGame.h"
#include "RoulettePayout.h"

RouletteGame::RouletteGame(int startingBalance)
    : balance(startingBalance) {}

int RouletteGame::getBalance() const {
    return balance;
}

bool RouletteGame::canPlaceBet(int amount) const {
    return amount > 0 && amount <= balance;
}

void RouletteGame::placeBet(const RouletteBet& bet) {
    if (canPlaceBet(bet.amount)) {
        bets.push_back(bet);
        balance -= bet.amount;
    }
}

void RouletteGame::clearBets() {
    bets.clear();
}

void RouletteGame::spin() {
    lastResult.number = wheel.spin();
    lastResult.color = wheel.getColor(lastResult.number);
}

void RouletteGame::resolve() {
    for (auto& bet : bets) {
        int payout = RoulettePayout::calculate(
            bet,
            lastResult.number,
            lastResult.color
        );

        if (payout > 0) {
            // Won: return original bet + net profit
            balance += bet.amount + payout;
        }
        // If payout <= 0, the bet was already deducted via placeBet()
    }
    // Don't clear bets here; caller can read them for stats then call clearBets()
}

RouletteRoundResult RouletteGame::getLastResult() const {
    return lastResult;
}
