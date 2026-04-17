#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class MainMenu
{
public:
    explicit MainMenu(sf::Font& sharedFont);

    void handleMouseClick(sf::Vector2f mousePos, bool& startGame, bool& openStats, bool& exitGame);
    void handleTextEntered(unsigned int unicode);
    void handleBackspace();

    bool hasValidBankroll() const;
    double getEnteredBankroll() const;
    void resetSession();

    void draw(sf::RenderWindow& window);

private:
    sf::Font& font;

    sf::Text titleText;
    sf::Text bankrollLabelText;
    sf::Text bankrollInputText;
    sf::Text bankrollHintText;
    sf::Text errorText;

    sf::Text startText;
    sf::Text statsText;
    sf::Text exitText;
    sf::Text confirmText;
    sf::Text cancelText;

    sf::RectangleShape startButton;
    sf::RectangleShape statsButton;
    sf::RectangleShape exitButton;

    sf::RectangleShape overlay;
    sf::RectangleShape popupPanel;
    sf::RectangleShape bankrollBox;
    sf::RectangleShape confirmButton;
    sf::RectangleShape cancelButton;

    std::string bankrollInput;
    bool showingBankrollInput = false;
    bool bankrollSetForSession = false;

    void centerTextInButton(sf::Text& text, const sf::RectangleShape& button);
    void refreshBankrollText();
};