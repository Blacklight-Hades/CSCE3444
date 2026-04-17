#include "StatsUI.h"
#include <sstream>
#include <iomanip>

StatsUI::StatsUI(sf::Font& sharedFont, SessionStats* sessionStatsPtr)
    : returnState(AppState::MainMenu),
      font(sharedFont),
      sessionStats(sessionStatsPtr),
      titleText(font, "SESSION STATS", 46),
      globalTabText(font, "GLOBAL", 20),
      blackjackTabText(font, "BLACKJACK", 20),
      slotsTabText(font, "SLOTS", 20),
      rouletteTabText(font, "ROULETTE", 20),
      dataText(font, "", 24),
      backText(font, "BACK", 20),
      currentTab(GlobalTab)
{
    titleText.setFillColor(sf::Color(90, 210, 255));
    auto bounds = titleText.getLocalBounds();
    titleText.setPosition({ 500.f - bounds.size.x / 2.f - bounds.position.x, 30.f });

    // Setup Tabs
    float tabWidth = 200.f;
    float tabHeight = 50.f;
    float startX = 100.f;
    float tabY = 120.f;

    globalTab.setSize({ tabWidth, tabHeight });
    globalTab.setPosition({ startX, tabY });
    
    blackjackTab.setSize({ tabWidth, tabHeight });
    blackjackTab.setPosition({ startX + tabWidth, tabY });
    
    slotsTab.setSize({ tabWidth, tabHeight });
    slotsTab.setPosition({ startX + 2*tabWidth, tabY });
    
    rouletteTab.setSize({ tabWidth, tabHeight });
    rouletteTab.setPosition({ startX + 3*tabWidth, tabY });

    globalTabText.setFillColor(sf::Color::White);
    blackjackTabText.setFillColor(sf::Color::White);
    slotsTabText.setFillColor(sf::Color::White);
    rouletteTabText.setFillColor(sf::Color::White);

    centerTextInButton(globalTabText, globalTab);
    centerTextInButton(blackjackTabText, blackjackTab);
    centerTextInButton(slotsTabText, slotsTab);
    centerTextInButton(rouletteTabText, rouletteTab);

    dataText.setFillColor(sf::Color::White);
    dataText.setPosition({ 120.f, 220.f });

    backButton.setSize({ 220.f, 60.f });
    backButton.setPosition({ 390.f, 660.f });
    backButton.setFillColor(sf::Color(170, 50, 50));

    backText.setFillColor(sf::Color::White);
    centerTextInButton(backText, backButton);

    setTabActive(GlobalTab);
}

void StatsUI::setSessionStats(SessionStats* statsPtr)
{
    sessionStats = statsPtr;
}

void StatsUI::setReturnState(AppState state)
{
    returnState = state;
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

void StatsUI::setTabActive(StatsTab tab)
{
    currentTab = tab;
    
    sf::Color activeColor(45, 120, 200);
    sf::Color inactiveColor(30, 40, 60);

    globalTab.setFillColor(currentTab == GlobalTab ? activeColor : inactiveColor);
    blackjackTab.setFillColor(currentTab == BlackjackTab ? activeColor : inactiveColor);
    slotsTab.setFillColor(currentTab == SlotsTab ? activeColor : inactiveColor);
    rouletteTab.setFillColor(currentTab == RouletteTab ? activeColor : inactiveColor);
    
    updateStats();
}

void StatsUI::updateStats()
{
    if (!sessionStats) return;

    std::ostringstream ss;
    
    if (currentTab == GlobalTab)
    {
        ss << "Starting Bankroll: $" << std::fixed << std::setprecision(2) << sessionStats->getBankroll().getStartingBalance() << "\n\n"
           << "Current Balance: $" << std::fixed << std::setprecision(2) << sessionStats->getCurrentBalance() << "\n\n"
           << "Net Gain/Loss: " << formatMoney(sessionStats->getNetGainLoss()) << "\n\n"
           << "Peak Balance: $" << std::fixed << std::setprecision(2) << sessionStats->getPeakBalance() << "\n\n"
           << "Lowest Balance: $" << std::fixed << std::setprecision(2) << sessionStats->getLowestBalance() << "\n\n"
           << "Total Rounds Played: " << sessionStats->getTotalRounds() << "\n\n"
           << "Unique Games Played: " << sessionStats->getGamesPlayed();
    }
    else if (currentTab == BlackjackTab)
    {
        const auto& bjStats = sessionStats->getBlackjackStats();
        ss << "Hands Played: " << bjStats.getTotalRounds() << "\n\n"
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
    }
    else if (currentTab == SlotsTab)
    {
        const auto& slStats = sessionStats->getSlotsStats();
        ss << "Spins Played: " << slStats.getTotalRounds() << "\n\n"
           << "Wins: " << slStats.getTotalWins() << "\n"
           << "Losses: " << slStats.getTotalLosses() << "\n\n"
           << "Three in a Row: " << slStats.getThreeInARowHits() << "\n"
           << "Jackpots: " << slStats.getJackpotHits() << "\n\n\n\n\n"
           << "Win Rate: " << std::fixed << std::setprecision(1) << (slStats.getWinRate() * 100.0) << "%   |   "
           << "Net: " << formatMoney(slStats.getNetProfit());
    }
    else if (currentTab == RouletteTab)
    {
        const auto& rlStats = sessionStats->getRouletteStats();
        ss << "Spins Played: " << rlStats.getTotalRounds() << "\n\n"
           << "Wins: " << rlStats.getTotalWins() << "\n"
           << "Losses: " << rlStats.getTotalLosses() << "\n\n"
           << "Best Win (1 spin):  " << formatMoney(rlStats.getBiggestWin()) << "\n"
           << "Worst Loss (1 spin):  " << formatMoney(-rlStats.getBiggestLoss()) << "\n\n"
           << "Total Wagered: " << formatMoney(rlStats.getTotalAmountBet()) << "\n"
           << "Total Payout: " << formatMoney(rlStats.getTotalPayoutReceived()) << "\n\n"
           << "Straight Up Hits: " << rlStats.getStraightUpHits() << "\n\n"
           << "Win Rate: " << std::fixed << std::setprecision(1) << (rlStats.getWinRate() * 100.0) << "%   |   "
           << "Net: " << formatMoney(rlStats.getNetProfit());
    }
    
    dataText.setString(ss.str());
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
    
    if (globalTab.getGlobalBounds().contains(mousePos)) setTabActive(GlobalTab);
    if (blackjackTab.getGlobalBounds().contains(mousePos)) setTabActive(BlackjackTab);
    if (slotsTab.getGlobalBounds().contains(mousePos)) setTabActive(SlotsTab);
    if (rouletteTab.getGlobalBounds().contains(mousePos)) setTabActive(RouletteTab);
}

void StatsUI::draw(sf::RenderWindow& window)
{
    sf::RectangleShape background({ 1000.f, 760.f });
    background.setFillColor(sf::Color(10, 20, 30));
    window.draw(background);

    window.draw(titleText);
    
    window.draw(globalTab);
    window.draw(globalTabText);
    window.draw(blackjackTab);
    window.draw(blackjackTabText);
    window.draw(slotsTab);
    window.draw(slotsTabText);
    window.draw(rouletteTab);
    window.draw(rouletteTabText);
    
    window.draw(dataText);
    
    window.draw(backButton);
    window.draw(backText);
}
