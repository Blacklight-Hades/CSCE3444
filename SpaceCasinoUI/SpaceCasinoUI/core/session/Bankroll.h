/*
    Name:       Prayush Panta
    UID:        PP1008
    Team:       Group 11 - Team Galactic - Space Casino
    Course:     CSCE 3444 Software Engineering
    Instructor: Bahareh M. Dorri

    Bankroll
    -------
    Shared session-level money tracker.

    Design intent:
      - SessionManager owns one Bankroll instance.
      - Game modules do NOT own the real bankroll.
      - Game modules calculate local bets/payouts and return round updates.
      - SessionManager applies those updates to Bankroll.

    Responsibilities:
      - Store current balance
      - Validate affordability
      - Withdraw and deposit funds
      - Track starting / peak / lowest balances
      - Provide summary information

    Non-responsibilities:
      - No game-specific payout logic
      - No blackjack / roulette / slots betting rules
      - No UI or terminal messaging
*/

#ifndef BANKROLL_H
#define BANKROLL_H

class Bankroll
{
private:
    double balance;
    double startingBalance;
    double peakBalance;
    double lowestBalance;

    void updateTracking();

public:
    explicit Bankroll(double startingAmount = 1000.0);

    bool canAfford(double amount) const;
    bool withdraw(double amount);
    bool deposit(double amount);
    bool applyNetChange(double amount);

    bool validateBalance() const;

    double getBalance() const;
    double getStartingBalance() const;
    double getNetGainLoss() const;
    double getPeakBalance() const;
    double getLowestBalance() const;
    bool isBroke() const;
};

#endif