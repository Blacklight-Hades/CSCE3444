#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <string>

class Button
{
public:
    Button(sf::Vector2f size, sf::Vector2f position, const std::string& text, sf::Font& font);

    void update(const sf::RenderWindow& window);
    void render(sf::RenderWindow& window);
    bool isClicked(const sf::RenderWindow& window, const sf::Event& event) const;

private:
    void centerText();

    sf::RectangleShape shape;
    sf::Text label;

    sf::Color idleColor = sf::Color(50, 50, 100);
    sf::Color hoverColor = sf::Color(100, 100, 200);
};

#endif