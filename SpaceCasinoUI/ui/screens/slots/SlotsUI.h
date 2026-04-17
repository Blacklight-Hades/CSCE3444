#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "../../../core/slots/SlotsGame.h"
#include "../../../core/slots/SlotWindow.h"

class SlotsUI
{
public:
    explicit SlotsUI(sf::Font& sharedFont);

    void setStartingBankroll(double bankroll);
    double getCurrentBankroll() const;

    void handleScreenClick(sf::Vector2f mousePos, bool& backToMenu);
    void draw(sf::RenderWindow& window);

private:
    Slots game;
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

    SlotWindow currentWindow;
    bool hasSpun;
    double currentBet;
    double lastPayout;

    void centerTextInButton(sf::Text& text, const sf::RectangleShape& button);
    std::string symbolToString(char c) const;
    void updateText();
};
