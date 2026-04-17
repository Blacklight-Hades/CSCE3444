#pragma once
#include "RouletteTypes.h"

class RouletteBet {
public:
    BetType type;
    int value;
    int amount;
    Color colorValue;

    RouletteBet(BetType t, int val, int amt);
    RouletteBet(Color c, int amt);
};
