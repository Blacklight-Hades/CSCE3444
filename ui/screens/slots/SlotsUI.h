#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "../../../games/slots/SlotsGame.h"
#include "../../../games/slots/SlotWindow.h"

class SlotsUI
{
public:
    explicit SlotsUI(sf::Font& sharedFont);

    void setStartingBankroll(double bankroll);
    double getCurrentBankroll() const;
    void setSessionStats(class SessionStats* stats);

    void handleScreenClick(sf::Vector2f mousePos, bool& backToMenu);
    void handleTextEntered(unsigned int unicode);
    void handleBackspace();
    void draw(sf::RenderWindow& window);

private:
    Slots game;
    class SessionStats* sessionStats;
    sf::Font& font;

    sf::Text titleText;
    sf::Text bankrollText;
    sf::Text betText;
    sf::Text resultText;
    sf::Text payoutText;
    sf::Text backText;
    sf::Text spinText;

    sf::RectangleShape spinButton;
    sf::RectangleShape backButton;
    sf::RectangleShape plus10Button;
    sf::RectangleShape minus10Button;
    sf::RectangleShape betInputBox;

    SlotWindow currentWindow;
    bool hasSpun;
    double currentBet;
    double lastPayout;
    
    sf::Text plus10Text;
    sf::Text minus10Text;
    sf::Text betInputBoxText;

    std::string betInputString;
    bool isTypingBet;

    void centerTextInButton(sf::Text& text, const sf::RectangleShape& button);
    void refreshBetInputText();
    std::string symbolToString(char c) const;
    void updateText();
};
