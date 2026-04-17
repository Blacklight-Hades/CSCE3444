#pragma once
#include "RouletteTypes.h"
#include <string>

struct RouletteRoundResult {
    int number;
    Color color;

    std::string toString() const {
        return "Result: " + std::to_string(number) +
               " (" + colorToString(color) + ")";
    }
};
