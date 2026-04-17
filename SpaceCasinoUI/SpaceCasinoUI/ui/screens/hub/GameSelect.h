#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class GameSelect
{
public:
    explicit GameSelect(sf::Font& sharedFont);

    void handleMouseClick(sf::Vector2f mousePos, bool& openBlackjack, bool& openRoulette, bool& openSlots, bool& backToMain);
    void handleTextEntered(unsigned int unicode);
    void handleBackspace();

    void setBankroll(double bankroll);
    double getBankroll() const;

    void draw(sf::RenderWindow& window);

private:
    sf::Font& font;

    sf::Text titleText;
    sf::Text subtitleText;
    sf::Text messageText;
    sf::Text bankrollText;

    sf::RectangleShape blackjackButton;
    sf::RectangleShape rouletteButton;
    sf::RectangleShape slotsButton;
    sf::RectangleShape backButton;

    sf::Text blackjackText;
    sf::Text rouletteText;
    sf::Text slotsText;
    sf::Text backText;

    sf::RectangleShape overlay;
    sf::RectangleShape popupPanel;
    sf::RectangleShape addMoneyBox;
    sf::RectangleShape addConfirmButton;
    sf::RectangleShape addCancelButton;

    sf::Text addMoneyLabelText;
    sf::Text addMoneyInputText;
    sf::Text addHintText;
    sf::Text addErrorText;
    sf::Text addConfirmText;
    sf::Text addCancelText;

    bool showingGameOverPopup;
    sf::RectangleShape gameOverPanel;
    sf::Text gameOverTitle;
    sf::Text gameOverMessage;
    sf::RectangleShape gameOverButton;
    sf::Text gameOverButtonText;

    double currentBankroll;
    std::string addMoneyInput;
    bool showingAddMoneyPopup;

    void centerTextInButton(sf::Text& text, const sf::RectangleShape& button);
    void refreshBankrollDisplay();
    void refreshAddMoneyDisplay();
    bool hasValidAddAmount() const;
};