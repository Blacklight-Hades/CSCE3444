#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "../../../games/blackjack/BlackjackGame.h"
#include "../../../games/blackjack/Card.h"

class BlackjackUI
{
public:
    explicit BlackjackUI(sf::Font& sharedFont);

    void setStartingBankroll(double bankroll);
    double getCurrentBankroll() const;
    
    void setSessionStats(class SessionStats* stats);

    void handleScreenClick(sf::Vector2f mousePos, bool& backToMenu);
    void handleTextEntered(unsigned int unicode);
    void handleBackspace();
    void draw(sf::RenderWindow& window);

private:
    BlackjackGame game;
    class SessionStats* sessionStats;
    sf::Font& font;

    sf::Text titleText;
    sf::Text bankrollText;
    sf::Text betText;
    sf::Text dealerText;
    sf::Text playerText;
    sf::Text messageText;
    sf::Text statsText;

    sf::RectangleShape hitButton;
    sf::RectangleShape standButton;
    sf::RectangleShape doubleButton;
    sf::RectangleShape newRoundButton;
    sf::RectangleShape backButton;

    sf::RectangleShape plus10Button;
    sf::RectangleShape minus10Button;
    sf::RectangleShape betInputBox;

    sf::Text hitText;
    sf::Text standText;
    sf::Text doubleText;
    sf::Text newRoundText;
    sf::Text backText;
    
    sf::Text plus10Text;
    sf::Text minus10Text;
    sf::Text betInputBoxText;

    float currentBet;
    bool roundStarted;
    
    std::string betInputString;
    bool isTypingBet;

    void setupButtons();
    void centerTextInButton(sf::Text& text, const sf::RectangleShape& button);
    void refreshBetInputText();

    std::string getDealerDisplay() const;
    std::string getPlayerDisplay() const;
    std::string getStatusMessage() const;
    std::string shortenResultText(const std::string& text) const;
    std::string getPostRoundStats() const;

    void updateText();
    void handleGameClick(sf::Vector2f mousePos);
};
