#pragma once

#include <SFML/Graphics.hpp>
#include "../../../core/SessionStats.h"
#include "../../app/AppState.h"

class StatsUI
{
public:
    enum StatsTab { 
        GlobalTab, 
        BlackjackTab, 
        SlotsTab, 
        RouletteTab 
    };

    explicit StatsUI(sf::Font& sharedFont, SessionStats* sessionStatsPtr);

    void setSessionStats(SessionStats* statsPtr);
    void setReturnState(AppState state);
    
    void updateStats();
    void handleScreenClick(sf::Vector2f mousePos, bool& backToMenu);
    void draw(sf::RenderWindow& window);

    AppState returnState;

private:
    sf::Font& font;
    SessionStats* sessionStats;

    sf::Text titleText;
    
    // Tabs
    sf::RectangleShape globalTab;
    sf::Text globalTabText;
    sf::RectangleShape blackjackTab;
    sf::Text blackjackTabText;
    sf::RectangleShape slotsTab;
    sf::Text slotsTabText;
    sf::RectangleShape rouletteTab;
    sf::Text rouletteTabText;

    // Content Display
    sf::Text dataText;

    sf::Text backText;
    sf::RectangleShape backButton;

    StatsTab currentTab;

    void centerTextInButton(sf::Text& text, const sf::RectangleShape& button);
    std::string formatMoney(double amount) const;
    
    void setTabActive(StatsTab tab);
};
