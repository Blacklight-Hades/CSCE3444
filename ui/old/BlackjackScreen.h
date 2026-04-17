#ifndef BLACKJACKSCREEN_H
#define BLACKJACKSCREEN_H

#include <SFML/Graphics.hpp>
#include <string>
#include "Button.h"
#include "../core/SessionManager.h"

class BlackjackScreen
{
public:
    enum class Action
    {
        None,
        ReturnToHub
    };

    BlackjackScreen(sf::Font& appFont, SessionManager& sessionManager);
    ~BlackjackScreen();

    void resetForTableEntry();

    Action handleEvent(const sf::RenderWindow& window, const sf::Event& event);
    void update(const sf::RenderWindow& window);
    void render(sf::RenderWindow& window);

private:
    void setupButtons();

    void renderCenteredText(sf::RenderWindow& window, const std::string& text, float y, unsigned int size = 28);
    void renderLeftText(sf::RenderWindow& window, const std::string& text, float x, float y, unsigned int size = 22);
    void updateRoundTextFromGame();

    std::string buildDealerDisplay() const;
    std::string buildPlayerHandsDisplay() const;
    double getDisplayedTableBalance() const;

    sf::Font& font;
    SessionManager& session;

    double pendingBet;
    std::string roundText;
 
    Button* add5Button;
    Button* add25Button;
    Button* add100Button;
    Button* clearBetButton;
    Button* dealButton;

    Button* hitButton;
    Button* standButton;
    Button* doubleButton;
    Button* splitButton;

    Button* hintButton;
    std::string hintText;

    Button* nextHandButton;
    Button* backToHubButton;
};

#endif