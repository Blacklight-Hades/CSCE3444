#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "../../../core/blackjack/BlackjackGame.h"
#include "../../../core/blackjack/Card.h"

class BlackjackUI
{
public:
    explicit BlackjackUI(sf::Font& sharedFont);

    void setStartingBankroll(double bankroll);
    double getCurrentBankroll() const;

    void handleScreenClick(sf::Vector2f mousePos, bool& backToMenu);
    void draw(sf::RenderWindow& window);

private:
    BlackjackGame game;
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

    sf::Text hitText;
    sf::Text standText;
    sf::Text doubleText;
    sf::Text newRoundText;
    sf::Text backText;

    float currentBet;
    bool roundStarted;

    void setupButtons();
    void centerTextInButton(sf::Text& text, const sf::RectangleShape& button);

    std::string getDealerDisplay() const;
    std::string getPlayerDisplay() const;
    std::string getStatusMessage() const;
    std::string shortenResultText(const std::string& text) const;
    std::string getPostRoundStats() const;

    void updateText();
    void handleGameClick(sf::Vector2f mousePos);
};