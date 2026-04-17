#pragma once

#include <SFML/Graphics.hpp>
#include "../../../core/SessionStats.h"

class StatsUI
{
public:
    explicit StatsUI(sf::Font& sharedFont, SessionStats* sessionStatsPtr);

    void setSessionStats(SessionStats* statsPtr);
    void updateStats();
    void handleScreenClick(sf::Vector2f mousePos, bool& backToMenu);
    void draw(sf::RenderWindow& window);

private:
    sf::Font& font;
    SessionStats* sessionStats;

    sf::Text titleText;
    sf::Text sessionLabelText;
    sf::Text sessionDataText;
    sf::Text blackjackLabelText;
    sf::Text blackjackDataText;
    sf::Text slotsLabelText;
    sf::Text slotsDataText;
    sf::Text rouletteLabelText;
    sf::Text rouletteDataText;

    sf::Text backText;
    sf::RectangleShape backButton;

    void centerTextInButton(sf::Text& text, const sf::RectangleShape& button);
    std::string formatMoney(double amount) const;
};
