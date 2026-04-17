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

        if (payout > 0)
            balance += payout + bet.amount;
    }

    bets.clear();
}

RouletteRoundResult RouletteGame::getLastResult() const {
    return lastResult;
}
