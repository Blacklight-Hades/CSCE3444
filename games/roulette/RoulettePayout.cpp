#include "RoulettePayout.h"

// International Roulette Payout Rules:
// Straight Up (single number): 35 to 1
// Color (Red/Black):           1 to 1
// Even/Odd:                    1 to 1
// High/Low (1-18 / 19-36):    1 to 1
// All outside bets lose on 0 (green).
//
// Returns NET profit on win (positive), or NET loss (negative).
// The caller is responsible for returning the original bet on a win.

int RoulettePayout::calculate(const RouletteBet& bet, int winNum, Color winColor) {
    switch (bet.type) {
        case BetType::StraightUp:
            // 35 to 1: win = bet * 35 net profit
            return (bet.value == winNum) ? bet.amount * 35 : -bet.amount;

        case BetType::Color:
            // 0 is green, all color bets lose
            if (winNum == 0) return -bet.amount;
            // 1 to 1: win = bet * 1 net profit
            return (bet.colorValue == winColor) ? bet.amount : -bet.amount;

        case BetType::EvenOdd:
            // 0 is neither even nor odd in roulette
            if (winNum == 0) return -bet.amount;
            // 1 to 1
            return ((winNum % 2 == bet.value) ? bet.amount : -bet.amount);

        case BetType::HighLow:
            // 0 loses
            if (winNum == 0) return -bet.amount;
            // 1 to 1
            if ((bet.value == 0 && winNum >= 1 && winNum <= 18) ||
                (bet.value == 1 && winNum >= 19 && winNum <= 36))
                return bet.amount;
            return -bet.amount;
    }
    return -bet.amount;
}

