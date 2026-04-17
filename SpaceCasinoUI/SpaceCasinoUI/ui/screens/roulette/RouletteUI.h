#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "../../../core/roulette/RouletteGame.h"
#include "../../../core/roulette/RouletteBet.h"
#include "../../../core/roulette/RouletteTypes.h"

class RouletteUI
{
public:
    explicit RouletteUI(sf::Font& sharedFont);

    void setStartingBankroll(double bankroll);
    double getCurrentBankroll() const;

    void handleScreenClick(sf::Vector2f mousePos, bool& backToMenu);
    void handleTextEntered(unsigned int unicode);
    void handleBackspace();

    void draw(sf::RenderWindow& window);

private:
    RouletteGame game;
    sf::Font& font;

    sf::Text titleText;
    sf::Text bankrollText;
    sf::Text betText;
    sf::Text selectedBetText;
    sf::Text numberInputLabelText;
    sf::Text numberInputText;
    sf::Text resultText;
    sf::Text payoutText;
    sf::Text spinText;
    sf::Text backText;

    sf::RectangleShape redButton;
    sf::RectangleShape blackButton;
    sf::RectangleShape evenButton;
    sf::RectangleShape oddButton;
    sf::RectangleShape lowButton;
    sf::RectangleShape highButton;
    sf::RectangleShape straightButton;
    sf::RectangleShape spinButton;
    sf::RectangleShape backButton;
    sf::RectangleShape numberBox;

    sf::Text redText;
    sf::Text blackText;
    sf::Text evenText;
    sf::Text oddText;
    sf::Text lowText;
    sf::Text highText;
    sf::Text straightText;

    double currentBet;
    std::string numberInput;
    bool enteringNumber;

    enum class SelectedBet
    {
        Red,
        Black,
        Even,
        Odd,
        Low,
        High,
        Straight
    };

    SelectedBet selectedBet;
    bool hasSpun;

    void centerTextInButton(sf::Text& text, const sf::RectangleShape& button);
    void updateText();
    void spinRound();
    std::string getSelectedBetLabel() const;
};
