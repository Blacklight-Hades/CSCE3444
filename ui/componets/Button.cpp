#include "Button.h"

Button::Button(sf::Vector2f size, sf::Vector2f position, const std::string& text, sf::Font& font)
    : label(font, text)
{
    shape.setSize(size);
    shape.setPosition(position);
    shape.setFillColor(idleColor);

    label.setCharacterSize(20);
    label.setFillColor(sf::Color::White);

    centerText();
}

void Button::update(const sf::RenderWindow& window)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePos);

    if (shape.getGlobalBounds().contains(mouseWorld))
    {
        shape.setFillColor(hoverColor);
    }
    else
    {
        shape.setFillColor(idleColor);
    }
}

bool Button::isClicked(const sf::RenderWindow& window, const sf::Event& event) const
{
    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mousePressed->button == sf::Mouse::Button::Left)
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePos);

            return shape.getGlobalBounds().contains(mouseWorld);
        }
    }

    return false;
}

void Button::render(sf::RenderWindow& window)
{
    window.draw(shape);
    window.draw(label);
}

void Button::centerText()
{
    sf::FloatRect bounds = label.getLocalBounds();

    label.setOrigin({
        bounds.position.x + bounds.size.x / 2.0f,
        bounds.position.y + bounds.size.y / 2.0f
    });

    label.setPosition({
        shape.getPosition().x + shape.getSize().x / 2.0f,
        shape.getPosition().y + shape.getSize().y / 2.0f
    });
}