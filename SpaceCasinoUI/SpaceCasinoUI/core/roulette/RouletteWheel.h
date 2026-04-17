#pragma once
#include "RouletteTypes.h"

class RouletteWheel {
public:
    int spin();
    Color getColor(int number);
};
