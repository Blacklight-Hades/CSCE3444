#include "RouletteWheel.h"
#include <cstdlib>

int RouletteWheel::spin() {
    return rand() % 37;
}

Color RouletteWheel::getColor(int number) {
    if (number == 0) return Color::Green;
    if (number % 2 == 0) return Color::Black;
    return Color::Red;
}
