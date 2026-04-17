#include "RouletteWheel.h"
#include <cstdlib>

int RouletteWheel::spin() {
    return rand() % 37;
}

Color RouletteWheel::getColor(int number) {
    if (number == 0) return Color::Green;
    
    int redNumbers[] = {1, 3, 5, 7, 9, 12, 14, 16, 18, 19, 21, 23, 25, 27, 30, 32, 34, 36};
    for (int r : redNumbers) {
        if (number == r) return Color::Red;
    }
    return Color::Black;
}
