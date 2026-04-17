#pragma once
#include <SFML/Graphics.hpp>
#include "../core/RouletteGame.h"

class RouletteScreen {
private:
    RouletteGame game;

    sf::Font font;
    sf::Text welcomeText;
    sf::Text balanceText;
    sf::Text resultText;

    float introTimer;
    bool showIntro;

public:
    RouletteScreen();

    void handleEvent(sf::Event& event);
    void update(float dt);
    void render(sf::RenderWindow& window);
};
