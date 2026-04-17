#ifndef CASINOAPP_H
#define CASINOAPP_H

#include <SFML/Graphics.hpp>
#include <string>
#include "AppState.h"
#include "Button.h"
#include "BlackjackScreen.h"
#include "../core/SessionManager.h"

class CasinoApp
{
public:
    CasinoApp();
    ~CasinoApp();

    void run();

private:
    void processEvents();
    void update();
    void render();
    void changeState(AppState newState);

    void setupButtons();
    void renderCenteredText(const std::string& text, float y, unsigned int size = 30);

    void renderWelcome();
    void renderBankrollSetup();
    void renderMainHub();

    sf::RenderWindow window;
    sf::Font font;
    SessionManager session;
    AppState currentState;

    double pendingBankroll;

    Button* startButton;
    Button* exitButton;

    Button* add5;
    Button* add25;
    Button* add100;
    Button* add500;
    Button* clearButton;
    Button* confirmButton;

    Button* blackjackButton;
    Button* slotsButton;
    Button* rouletteButton;
    Button* statsButton;
    Button* backButton;

    BlackjackScreen* blackjackScreen;
};

#endif