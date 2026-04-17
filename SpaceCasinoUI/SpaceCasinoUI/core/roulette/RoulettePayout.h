#pragma once
#include "RouletteBet.h"

class RoulettePayout {
public:
    static int calculate(const RouletteBet& bet, int winNum, Color winColor);
};
