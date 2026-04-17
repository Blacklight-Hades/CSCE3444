#include "RouletteScreen.h"

RouletteScreen::RouletteScreen()
    : game(1000), introTimer(0), showIntro(true)
{
    font.loadFromFile("assets/arial.ttf");

    welcomeText.setFont(font);
    welcomeText.setString("Welcome to Space Casino\nLet's Play Some Roulette!");
    welcomeText.setCharacterSize(40);
    welcomeText.setPosition(200, 200);

    balanceText.setFont(font);
    balanceText.setCharacterSize(24);
    balanceText.setPosition(20, 20);

    resultText.setFont(font);
    resultText.setCharacterSize(24);
    resultText.setPosition(20, 60);
}

void RouletteScreen::handleEvent(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Space) {
            game.spin();
            game.resolve();
        }

        if (event.key.code == sf::Keyboard::Num1) {
            game.placeBet(RouletteBet(Color::Red, 50));
        }

        if (event.key.code == sf::Keyboard::Num2) {
            game.placeBet(RouletteBet(Color::Black, 50));
        }
    }
}

void RouletteScreen::update(float dt) {
    if (showIntro) {
        introTimer += dt;
        if (introTimer > 3)
            showIntro = false;
    }

    balanceText.setString("Balance: " + std::to_string(game.getBalance()));

    auto result = game.getLastResult();
    resultText.setString(result.toString());
}

void RouletteScreen::render(sf::RenderWindow& window) {
    if (showIntro) {
        window.draw(welcomeText);
        return;
    }

    window.draw(balanceText);
    window.draw(resultText);
}
