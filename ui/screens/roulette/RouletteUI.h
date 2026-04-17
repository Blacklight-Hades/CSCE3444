#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <string>
#include <vector>
#include "../../../games/roulette/RouletteGame.h"
#include "../../../core/SessionStats.h"

struct PlacedChip {
    std::string text;
    sf::Vector2f position;
    int amount;
    sf::Color color;
};

class RouletteUI
{
public:
    explicit RouletteUI(sf::Font& sharedFont);

    void setStartingBankroll(double bankroll);
    double getCurrentBankroll() const;
    void setSessionStats(class SessionStats* stats);

    void handleScreenClick(sf::Vector2f mousePos, bool& backToMenu, bool& openStats);
    void handleTextEntered(unsigned int unicode);
    void handleBackspace();
    void draw(sf::RenderWindow& window);

private:
    RouletteGame game;
    class SessionStats* sessionStats;
    sf::Font& font;

    sf::Text titleText;
    sf::Text bankrollText;
    sf::Text messageText;
    sf::Text spinText;
    sf::Text clearText;
    sf::Text backText;
    sf::Text chipSizeText;
    sf::Text infoText;

    sf::RectangleShape spinButton;
    sf::RectangleShape clearButton;
    sf::RectangleShape backButton;
    sf::RectangleShape plus10Button;
    sf::RectangleShape minus10Button;
    
    sf::Text plus10Text;
    sf::Text minus10Text;

    int currentChipSize;
    std::vector<PlacedChip> renderedChips;

    sf::RectangleShape betInputBox;
    sf::Text betInputBoxText;
    std::string betInputString;
    bool isTypingBet;

    bool isSpinning;
    sf::Clock spinClock;
    float spinDuration;
    int resultingNumber;
    Color resultingColor;
    int lastPayout;

    void handleTableClick(sf::Vector2f mousePos);
    void pushChip(const std::string& label, sf::Vector2f pos, RouletteBet bet);
    void centerTextInButton(sf::Text& text, const sf::RectangleShape& button);
    sf::Color getStandardColor(int number) const;
    
    void drawWheel(sf::RenderWindow& window, float elapsedTime);
    void drawTable(sf::RenderWindow& window);
    void updateText();
    void refreshBetInputText();
};
