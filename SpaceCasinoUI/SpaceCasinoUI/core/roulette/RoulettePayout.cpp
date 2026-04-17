#include "RoulettePayout.h"

int RoulettePayout::calculate(const RouletteBet& bet, int winNum, Color winColor) {
    switch (bet.type) {
        case BetType::StraightUp:
            return (bet.value == winNum) ? bet.amount * 35 : -bet.amount;

        case BetType::Color:
            return (bet.colorValue == winColor) ? bet.amount : -bet.amount;

        case BetType::EvenOdd:
            if (winNum == 0) return -bet.amount;
            return ((winNum % 2 == bet.value) ? bet.amount : -bet.amount);

        case BetType::HighLow:
            if (winNum == 0) return -bet.amount;
            if ((bet.value == 0 && winNum <= 18) ||
                (bet.value == 1 && winNum >= 19))
                return bet.amount;
            return -bet.amount;
    }
    return -bet.amount;
}
