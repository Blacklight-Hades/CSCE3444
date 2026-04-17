#include "MainMenu.h"

MainMenu::MainMenu(sf::Font& sharedFont)
    : font(sharedFont),
    titleText(font, "WELCOME TO SPACE CASINO", 46),
    bankrollLabelText(font, "ENTER STARTING BANKROLL", 24),
    bankrollInputText(font, "", 28),
    bankrollHintText(font, "", 18),
    errorText(font, "", 18),
    startText(font, "START GAME", 22),
    exitText(font, "EXIT", 22),
    confirmText(font, "CONFIRM", 20),
    cancelText(font, "CANCEL", 20),
    bankrollInput("")
{
    titleText.setFillColor(sf::Color::White);
    sf::FloatRect bounds = titleText.getLocalBounds();
    titleText.setPosition({
        500.f - bounds.size.x / 2.f - bounds.position.x,
        120.f
        });

    startButton.setSize({ 300.f, 80.f });
    startButton.setPosition({ 350.f, 320.f });
    startButton.setFillColor(sf::Color(60, 60, 180));

    exitButton.setSize({ 300.f, 80.f });
    exitButton.setPosition({ 350.f, 440.f });
    exitButton.setFillColor(sf::Color(170, 50, 50));

    startText.setFillColor(sf::Color::White);
    exitText.setFillColor(sf::Color::White);

    centerTextInButton(startText, startButton);
    centerTextInButton(exitText, exitButton);

    overlay.setSize({ 1000.f, 760.f });
    overlay.setFillColor(sf::Color(0, 0, 0, 165));

    popupPanel.setSize({ 470.f, 290.f });
    popupPanel.setPosition({ 265.f, 190.f });
    popupPanel.setFillColor(sf::Color(12, 28, 55));
    popupPanel.setOutlineThickness(3.f);
    popupPanel.setOutlineColor(sf::Color(90, 210, 255));

    bankrollLabelText.setFillColor(sf::Color(235, 220, 90));
    sf::FloatRect labelBounds = bankrollLabelText.getLocalBounds();
    bankrollLabelText.setPosition({
        500.f - labelBounds.size.x / 2.f - labelBounds.position.x,
        225.f
        });

    bankrollBox.setSize({ 320.f, 58.f });
    bankrollBox.setPosition({ 340.f, 275.f });
    bankrollBox.setFillColor(sf::Color(20, 20, 60));
    bankrollBox.setOutlineThickness(2.f);
    bankrollBox.setOutlineColor(sf::Color(90, 210, 255));

    bankrollInputText.setFillColor(sf::Color::White);
    bankrollInputText.setPosition({ 360.f, 285.f });

    bankrollHintText.setFillColor(sf::Color(180, 180, 180));
    bankrollHintText.setPosition({ 340.f, 345.f });

    errorText.setFillColor(sf::Color(255, 120, 120));
    errorText.setPosition({ 340.f, 372.f });

    confirmButton.setSize({ 160.f, 52.f });
    confirmButton.setPosition({ 340.f, 410.f });
    confirmButton.setFillColor(sf::Color(70, 70, 100));

    cancelButton.setSize({ 160.f, 52.f });
    cancelButton.setPosition({ 520.f, 410.f });
    cancelButton.setFillColor(sf::Color(120, 120, 120));

    confirmText.setFillColor(sf::Color::White);
    cancelText.setFillColor(sf::Color::White);

    centerTextInButton(confirmText, confirmButton);
    centerTextInButton(cancelText, cancelButton);
}

void MainMenu::centerTextInButton(sf::Text& text, const sf::RectangleShape& button)
{
    sf::FloatRect textBounds = text.getLocalBounds();
    sf::Vector2f buttonPos = button.getPosition();
    sf::Vector2f buttonSize = button.getSize();

    text.setPosition({
        buttonPos.x + (buttonSize.x - textBounds.size.x) / 2.f - textBounds.position.x,
        buttonPos.y + (buttonSize.y - textBounds.size.y) / 2.f - textBounds.position.y - 3.f
        });
}

void MainMenu::refreshBankrollText()
{
    if (bankrollInput.empty())
    {
        bankrollInputText.setString("$");
    }
    else
    {
        bankrollInputText.setString("$" + bankrollInput);
    }

    if (bankrollInput == "0")
    {
        errorText.setString("Bankroll must be greater than 0.");
    }
    else
    {
        errorText.setString("");
    }

    if (!bankrollInput.empty() && bankrollInput != "0")
    {
        confirmButton.setFillColor(sf::Color(60, 60, 180));
    }
    else
    {
        confirmButton.setFillColor(sf::Color(70, 70, 100));
    }
}

void MainMenu::handleTextEntered(unsigned int unicode)
{
    if (!showingBankrollInput)
    {
        return;
    }

    bankrollHintText.setString("");

    if (unicode >= '0' && unicode <= '9')
    {
        if (bankrollInput.size() < 7)
        {
            bankrollInput += static_cast<char>(unicode);
        }
    }
    else if (unicode >= 32 && unicode <= 126)
    {
        bankrollHintText.setString("Type numbers only.");
    }

    refreshBankrollText();
}

void MainMenu::handleBackspace()
{
    if (!showingBankrollInput)
    {
        return;
    }

    if (!bankrollInput.empty())
    {
        bankrollInput.pop_back();
    }

    bankrollHintText.setString("");
    refreshBankrollText();
}

bool MainMenu::hasValidBankroll() const
{
    if (bankrollInput.empty())
    {
        return false;
    }

    try
    {
        return std::stod(bankrollInput) > 0.0;
    }
    catch (...)
    {
        return false;
    }
}

double MainMenu::getEnteredBankroll() const
{
    if (!hasValidBankroll())
    {
        return 1000.0;
    }

    return std::stod(bankrollInput);
}

void MainMenu::resetSession()
{
    bankrollSetForSession = false;
    showingBankrollInput = false;
    bankrollInput.clear();
    bankrollHintText.setString("");
    errorText.setString("");
    refreshBankrollText();
}

void MainMenu::handleMouseClick(sf::Vector2f mousePos, bool& startGame, bool& exitGame)
{
    startGame = false;
    exitGame = false;

    if (!showingBankrollInput)
    {
        if (startButton.getGlobalBounds().contains(mousePos))
        {
            if (bankrollSetForSession)
            {
                startGame = true;
            }
            else
            {
                showingBankrollInput = true;
                bankrollHintText.setString("");
                errorText.setString("");
                refreshBankrollText();
            }
            return;
        }

        if (exitButton.getGlobalBounds().contains(mousePos))
        {
            exitGame = true;
            return;
        }
    }
    else
    {
        if (confirmButton.getGlobalBounds().contains(mousePos))
        {
            if (bankrollInput == "0")
            {
                errorText.setString("Bankroll must be greater than 0.");
                return;
            }

            if (hasValidBankroll())
            {
                bankrollSetForSession = true;
                showingBankrollInput = false;
                bankrollHintText.setString("");
                errorText.setString("");
                startGame = true;
                return;
            }
        }

        if (cancelButton.getGlobalBounds().contains(mousePos))
        {
            showingBankrollInput = false;
            bankrollInput.clear();
            bankrollInputText.setString("$");
            bankrollHintText.setString("");
            errorText.setString("");
            refreshBankrollText();
            return;
        }
    }
}

void MainMenu::draw(sf::RenderWindow& window)
{
    sf::RectangleShape background({ 1000.f, 760.f });
    background.setFillColor(sf::Color(0, 85, 20));
    window.draw(background);

    window.draw(titleText);
    window.draw(startButton);
    window.draw(exitButton);
    window.draw(startText);
    window.draw(exitText);

    if (showingBankrollInput)
    {
        window.draw(overlay);
        window.draw(popupPanel);
        window.draw(bankrollLabelText);
        window.draw(bankrollBox);
        window.draw(bankrollInputText);

        if (bankrollHintText.getString() != "")
        {
            window.draw(bankrollHintText);
        }

        if (errorText.getString() != "")
        {
            window.draw(errorText);
        }

        window.draw(confirmButton);
        window.draw(cancelButton);
        window.draw(confirmText);
        window.draw(cancelText);
    }
}