#pragma once
#include <vector>
#include "RouletteBet.h"
#include "RouletteWheel.h"
#include "RouletteRoundResult.h"

class RouletteGame {
private:
    int balance;
    std::vector<RouletteBet> bets;

    RouletteWheel wheel;
    RouletteRoundResult lastResult;

public:
    RouletteGame(int startingBalance);

    int getBalance() const;

    bool canPlaceBet(int amount) const;
    void placeBet(const RouletteBet& bet);
    void clearBets();

    void spin();
    void resolve();

    RouletteRoundResult getLastResult() const;
};
