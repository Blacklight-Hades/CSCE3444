#include "StatsUI.h"
#include <sstream>
#include <iomanip>

StatsUI::StatsUI(sf::Font& sharedFont, SessionStats* sessionStatsPtr)
    : font(sharedFont),
      sessionStats(sessionStatsPtr),
      titleText(font, "SESSION STATS", 46),
      sessionLabelText(font, "Global", 28),
      sessionDataText(font, "", 20),
      blackjackLabelText(font, "Blackjack", 28),
      blackjackDataText(font, "", 20),
      slotsLabelText(font, "Slots", 28),
      slotsDataText(font, "", 20),
      rouletteLabelText(font, "Roulette", 28),
      rouletteDataText(font, "", 20),
      backText(font, "BACK TO MENU", 20)
{
    titleText.setFillColor(sf::Color(90, 210, 255));
    auto bounds = titleText.getLocalBounds();
    titleText.setPosition({ 500.f - bounds.size.x / 2.f - bounds.position.x, 30.f });

    sessionLabelText.setFillColor(sf::Color(235, 220, 90));
    sessionLabelText.setPosition({ 80.f, 120.f });
    sessionDataText.setFillColor(sf::Color::White);
    sessionDataText.setPosition({ 80.f, 160.f });

    blackjackLabelText.setFillColor(sf::Color(235, 220, 90));
    blackjackLabelText.setPosition({ 520.f, 120.f });
    blackjackDataText.setFillColor(sf::Color::White);
    blackjackDataText.setPosition({ 520.f, 160.f });

    slotsLabelText.setFillColor(sf::Color(235, 220, 90));
    slotsLabelText.setPosition({ 80.f, 380.f });
    slotsDataText.setFillColor(sf::Color::White);
    slotsDataText.setPosition({ 80.f, 420.f });

    rouletteLabelText.setFillColor(sf::Color(235, 220, 90));
    rouletteLabelText.setPosition({ 520.f, 380.f });
    rouletteDataText.setFillColor(sf::Color::White);
    rouletteDataText.setPosition({ 520.f, 420.f });

    backButton.setSize({ 220.f, 60.f });
    backButton.setPosition({ 390.f, 660.f });
    backButton.setFillColor(sf::Color(170, 50, 50));

    backText.setFillColor(sf::Color::White);
    centerTextInButton(backText, backButton);

    updateStats();
}

void StatsUI::setSessionStats(SessionStats* statsPtr)
{
    sessionStats = statsPtr;
}

std::string StatsUI::formatMoney(double amount) const
{
    std::ostringstream out;
    if (amount >= 0)
        out << "+$" << std::fixed << std::setprecision(2) << amount;
    else
        out << "-$" << std::fixed << std::setprecision(2) << std::abs(amount);
    return out.str();
}

void StatsUI::updateStats()
{
    if (!sessionStats) return;

    // Global
    std::ostringstream ss;
    ss << "Starting Bankroll: $" << std::fixed << std::setprecision(2) << sessionStats->getBankroll().getStartingBalance() << "\n\n"
       << "Current Balance: $" << std::fixed << std::setprecision(2) << sessionStats->getCurrentBalance() << "\n\n"
       << "Net Gain/Loss: " << formatMoney(sessionStats->getNetGainLoss()) << "\n\n"
       << "Peak Balance: $" << std::fixed << std::setprecision(2) << sessionStats->getPeakBalance() << "\n\n"
       << "Lowest Balance: $" << std::fixed << std::setprecision(2) << sessionStats->getLowestBalance() << "\n\n"
       << "Total Rounds Played: " << sessionStats->getTotalRounds() << "\n\n"
       << "Unique Games Played: " << sessionStats->getGamesPlayed();
    sessionDataText.setString(ss.str());

    // Blackjack
    std::ostringstream bj;
    const auto& bjStats = sessionStats->getBlackjackStats();
    bj << "Hands Played: " << bjStats.getTotalRounds() << "\n\n"
       << "Wins: " << bjStats.getTotalWins() << "\n"
       << "Losses: " << bjStats.getTotalLosses() << "\n"
       << "Pushes: " << bjStats.getTotalPushes() << "\n\n"
       << "Blackjacks: " << bjStats.getTotalBlackjacks() << "\n"
       << "Player Busts: " << bjStats.getTotalPlayerBusts() << "\n"
       << "Double Downs: " << bjStats.getTotalDoubleDowns() << "\n\n"
       << "Win Streak: " << bjStats.getLongestWinStreak() << "\n"
       << "Loss Streak: " << bjStats.getLongestLossStreak() << "\n\n"
       << "Win Rate: " << std::fixed << std::setprecision(1) << (bjStats.getWinRate() * 100.0) << "%   |   "
       << "Net: " << formatMoney(bjStats.getNetProfit());
    blackjackDataText.setString(bj.str());

    // Slots
    std::ostringstream sl;
    const auto& slStats = sessionStats->getSlotsStats();
    sl << "Spins Played: " << slStats.getTotalRounds() << "\n\n"
       << "Wins: " << slStats.getTotalWins() << "\n"
       << "Losses: " << slStats.getTotalLosses() << "\n\n"
       << "Three in a Row: " << slStats.getThreeInARowHits() << "\n"
       << "Jackpots: " << slStats.getJackpotHits() << "\n\n\n\n\n"
       << "Win Rate: " << std::fixed << std::setprecision(1) << (slStats.getWinRate() * 100.0) << "%   |   "
       << "Net: " << formatMoney(slStats.getNetProfit());
    slotsDataText.setString(sl.str());

    // Roulette
    std::ostringstream rl;
    const auto& rlStats = sessionStats->getRouletteStats();
    rl << "Spins Played: " << rlStats.getTotalRounds() << "\n\n"
       << "Wins: " << rlStats.getTotalWins() << "\n"
       << "Losses: " << rlStats.getTotalLosses() << "\n\n"
       << "Straight Up Hits: " << rlStats.getStraightUpHits() << "\n\n\n\n\n\n"
       << "Win Rate: " << std::fixed << std::setprecision(1) << (rlStats.getWinRate() * 100.0) << "%   |   "
       << "Net: " << formatMoney(rlStats.getNetProfit());
    rouletteDataText.setString(rl.str());
}

void StatsUI::centerTextInButton(sf::Text& text, const sf::RectangleShape& button)
{
    auto bounds = text.getLocalBounds();
    auto pos = button.getPosition();
    auto size = button.getSize();

    text.setPosition({
        pos.x + (size.x - bounds.size.x) / 2.f - bounds.position.x,
        pos.y + (size.y - bounds.size.y) / 2.f - bounds.position.y
    });
}

void StatsUI::handleScreenClick(sf::Vector2f mousePos, bool& backToMenu)
{
    backToMenu = false;
    if (backButton.getGlobalBounds().contains(mousePos))
    {
        backToMenu = true;
    }
}

void StatsUI::draw(sf::RenderWindow& window)
{
    sf::RectangleShape background({ 1000.f, 760.f });
    background.setFillColor(sf::Color(10, 20, 30));
    window.draw(background);

    window.draw(titleText);
    
    window.draw(sessionLabelText);
    window.draw(sessionDataText);
    
    window.draw(blackjackLabelText);
    window.draw(blackjackDataText);
    
    window.draw(slotsLabelText);
    window.draw(slotsDataText);
    
    window.draw(rouletteLabelText);
    window.draw(rouletteDataText);
    
    window.draw(backButton);
    window.draw(backText);
}
