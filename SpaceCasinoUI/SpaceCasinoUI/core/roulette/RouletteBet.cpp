#include "RouletteBet.h"

RouletteBet::RouletteBet(BetType t, int val, int amt)
    : type(t), value(val), amount(amt), colorValue(Color::Green) {}

RouletteBet::RouletteBet(Color c, int amt)
    : type(BetType::Color), value(0), amount(amt), colorValue(c) {}
