#pragma once
#include <string>

enum class BetType {
    StraightUp,
    Color,
    EvenOdd,
    HighLow
};

enum class Color {
    Red,
    Black,
    Green
};

inline std::string colorToString(Color c) {
    switch (c) {
        case Color::Red: return "Red";
        case Color::Black: return "Black";
        default: return "Green";
    }
}
