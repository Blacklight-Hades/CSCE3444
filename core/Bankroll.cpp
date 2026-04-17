/*
    Name:       Prayush Panta
    UID:        PP1008
    Team:       Group 11 - Team Galactic - Space Casino
    Course:     CSCE 3444 Software Engineering
    Instructor: Bahareh M. Dorri
*/

#include "Bankroll.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace
{
    constexpr double EPSILON = 1e-9;
}

Bankroll::Bankroll(double startingAmount)
    : balance(startingAmount),
      startingBalance(startingAmount),
      peakBalance(startingAmount),
      lowestBalance(startingAmount)
{
    if (startingAmount <= 0.0)
    {
        throw std::invalid_argument("Starting bankroll must be greater than 0.");
    }
}

void Bankroll::updateTracking()
{
    peakBalance = std::max(peakBalance, balance);
    lowestBalance = std::min(lowestBalance, balance);
}

bool Bankroll::canAfford(double amount) const
{
    if (amount <= 0.0)
    {
        return false;
    }

    return amount <= balance + EPSILON;
}

bool Bankroll::withdraw(double amount)
{
    if (!canAfford(amount))
    {
        return false;
    }

    balance -= amount;

    if (std::abs(balance) < EPSILON)
    {
        balance = 0.0;
    }

    updateTracking();
    return true;
}

bool Bankroll::deposit(double amount)
{
    if (amount <= 0.0)
    {
        return false;
    }

    balance += amount;
    updateTracking();
    return true;
}

bool Bankroll::applyNetChange(double amount)
{
    if (std::abs(amount) < EPSILON)
    {
        return true;
    }

    if (amount > 0.0)
    {
        return deposit(amount);
    }

    return withdraw(-amount);
}

bool Bankroll::validateBalance() const
{
    return balance >= -EPSILON;
}

double Bankroll::getBalance() const
{
    return balance;
}

double Bankroll::getStartingBalance() const
{
    return startingBalance;
}

double Bankroll::getNetGainLoss() const
{
    return balance - startingBalance;
}

double Bankroll::getPeakBalance() const
{
    return peakBalance;
}

double Bankroll::getLowestBalance() const
{
    return lowestBalance;
}

bool Bankroll::isBroke() const
{
    return balance <= EPSILON;
}

void Bankroll::setBalance(double amount)
{
    if (amount >= 0.0)
    {
        balance = amount;
        updateTracking();
    }
}