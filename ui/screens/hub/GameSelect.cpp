#include "GameSelect.h"
#include <string>

GameSelect::GameSelect(sf::Font& sharedFont)
    : font(sharedFont),
    titleText(font, "CHOOSE YOUR GAME", 48),
    subtitleText(font, "Welcome to the casino floor", 20),
    messageText(font, "", 18),
    bankrollText(font, "Bankroll: $1000", 24),
    blackjackText(font, "BLACKJACK", 22),
    rouletteText(font, "ROULETTE", 22),
    slotsText(font, "SLOTS", 22),
    backText(font, "BACK", 22),
    addMoneyLabelText(font, "ADD MORE MONEY", 24),
    addMoneyInputText(font, "", 28),
    addHintText(font, "", 18),
    addErrorText(font, "", 18),
    addConfirmText(font, "CONFIRM", 20),
    addCancelText(font, "CANCEL", 20),
    showingGameOverPopup(false),
    gameOverTitle(font, "GAME OVER!", 34),
    gameOverMessage(font, "You're Broke!", 24),
    gameOverButtonText(font, "MAIN MENU", 20),
    currentBankroll(1000.0),
    addMoneyInput(""),
    showingAddMoneyPopup(false)
{
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition({ 285.f, 85.f });

    subtitleText.setFillColor(sf::Color(235, 220, 90));
    subtitleText.setPosition({ 350.f, 145.f });

    bankrollText.setFillColor(sf::Color::White);
    bankrollText.setPosition({ 20.f, 20.f });

    messageText.setFillColor(sf::Color::Yellow);
    messageText.setPosition({ 315.f, 670.f });

    blackjackButton.setSize({ 320.f, 80.f });
    blackjackButton.setPosition({ 340.f, 220.f });
    blackjackButton.setFillColor(sf::Color(45, 130, 60));

    rouletteButton.setSize({ 320.f, 80.f });
    rouletteButton.setPosition({ 340.f, 330.f });
    rouletteButton.setFillColor(sf::Color(150, 80, 30));

    slotsButton.setSize({ 320.f, 80.f });
    slotsButton.setPosition({ 340.f, 440.f });
    slotsButton.setFillColor(sf::Color(110, 35, 140));

    backButton.setSize({ 220.f, 65.f });
    backButton.setPosition({ 390.f, 575.f });
    backButton.setFillColor(sf::Color(110, 110, 110));

    blackjackText.setFillColor(sf::Color::White);
    rouletteText.setFillColor(sf::Color::White);
    slotsText.setFillColor(sf::Color::White);
    backText.setFillColor(sf::Color::White);

    centerTextInButton(blackjackText, blackjackButton);
    centerTextInButton(rouletteText, rouletteButton);
    centerTextInButton(slotsText, slotsButton);
    centerTextInButton(backText, backButton);

    overlay.setSize({ 1000.f, 760.f });
    overlay.setFillColor(sf::Color(0, 0, 0, 165));

    popupPanel.setSize({ 470.f, 290.f });
    popupPanel.setPosition({ 265.f, 190.f });
    popupPanel.setFillColor(sf::Color(12, 28, 55));
    popupPanel.setOutlineThickness(3.f);
    popupPanel.setOutlineColor(sf::Color(90, 210, 255));

    addMoneyLabelText.setFillColor(sf::Color(235, 220, 90));
    addMoneyLabelText.setPosition({ 385.f, 225.f });

    addMoneyBox.setSize({ 320.f, 58.f });
    addMoneyBox.setPosition({ 340.f, 275.f });
    addMoneyBox.setFillColor(sf::Color(20, 20, 60));
    addMoneyBox.setOutlineThickness(2.f);
    addMoneyBox.setOutlineColor(sf::Color(90, 210, 255));

    addMoneyInputText.setFillColor(sf::Color::White);
    addMoneyInputText.setPosition({ 360.f, 285.f });

    addHintText.setFillColor(sf::Color(180, 180, 180));
    addHintText.setPosition({ 340.f, 345.f });

    addErrorText.setFillColor(sf::Color(255, 120, 120));
    addErrorText.setPosition({ 340.f, 372.f });

    addConfirmButton.setSize({ 160.f, 52.f });
    addConfirmButton.setPosition({ 340.f, 410.f });
    addConfirmButton.setFillColor(sf::Color(70, 70, 100));

    addCancelButton.setSize({ 160.f, 52.f });
    addCancelButton.setPosition({ 520.f, 410.f });
    addCancelButton.setFillColor(sf::Color(120, 120, 120));

    addConfirmText.setFillColor(sf::Color::White);
    addCancelText.setFillColor(sf::Color::White);

    centerTextInButton(addConfirmText, addConfirmButton);
    centerTextInButton(addCancelText, addCancelButton);

    gameOverPanel.setSize({ 430.f, 230.f });
    gameOverPanel.setPosition({ 285.f, 250.f });
    gameOverPanel.setFillColor(sf::Color(30, 10, 10));
    gameOverPanel.setOutlineThickness(3.f);
    gameOverPanel.setOutlineColor(sf::Color(220, 70, 70));

    gameOverTitle.setFillColor(sf::Color(255, 90, 90));
    gameOverTitle.setPosition({ 380.f, 285.f });

    gameOverMessage.setFillColor(sf::Color::White);
    gameOverMessage.setPosition({ 410.f, 340.f });

    gameOverButton.setSize({ 180.f, 55.f });
    gameOverButton.setPosition({ 410.f, 395.f });
    gameOverButton.setFillColor(sf::Color(110, 110, 110));

    gameOverButtonText.setFillColor(sf::Color::White);
    centerTextInButton(gameOverButtonText, gameOverButton);

    refreshBankrollDisplay();
    refreshAddMoneyDisplay();
}

void GameSelect::setBankroll(double bankroll)
{
    currentBankroll = bankroll;

    if (currentBankroll <= 0)
    {
        currentBankroll = 0;
        showingGameOverPopup = true;
        showingAddMoneyPopup = false;
        addMoneyInput.clear();
        addHintText.setString("");
        addErrorText.setString("");
    }

    refreshBankrollDisplay();
    refreshAddMoneyDisplay();
}

double GameSelect::getBankroll() const
{
    return currentBankroll;
}

void GameSelect::refreshBankrollDisplay()
{
    bankrollText.setString("Bankroll: $" + std::to_string(static_cast<int>(currentBankroll)));
}

void GameSelect::refreshAddMoneyDisplay()
{
    if (addMoneyInput.empty())
    {
        addMoneyInputText.setString("$");
    }
    else
    {
        addMoneyInputText.setString("$" + addMoneyInput);
    }

    if (addMoneyInput == "0")
    {
        addErrorText.setString("Amount must be greater than 0.");
    }
    else
    {
        addErrorText.setString("");
    }

    if (!addMoneyInput.empty() && addMoneyInput != "0")
    {
        addConfirmButton.setFillColor(sf::Color(60, 60, 180));
    }
    else
    {
        addConfirmButton.setFillColor(sf::Color(70, 70, 100));
    }
}

bool GameSelect::hasValidAddAmount() const
{
    if (addMoneyInput.empty())
    {
        return false;
    }

    try
    {
        return std::stod(addMoneyInput) > 0.0;
    }
    catch (...)
    {
        return false;
    }
}

void GameSelect::handleTextEntered(unsigned int unicode)
{
    if (!showingAddMoneyPopup || showingGameOverPopup)
    {
        return;
    }

    addHintText.setString("");

    if (unicode >= '0' && unicode <= '9')
    {
        if (addMoneyInput.size() < 7)
        {
            addMoneyInput += static_cast<char>(unicode);
        }
    }
    else if (unicode >= 32 && unicode <= 126)
    {
        addHintText.setString("Type numbers only.");
    }

    refreshAddMoneyDisplay();
}

void GameSelect::handleBackspace()
{
    if (!showingAddMoneyPopup || showingGameOverPopup)
    {
        return;
    }

    if (!addMoneyInput.empty())
    {
        addMoneyInput.pop_back();
    }

    addHintText.setString("");
    refreshAddMoneyDisplay();
}

void GameSelect::centerTextInButton(sf::Text& text, const sf::RectangleShape& button)
{
    sf::FloatRect textBounds = text.getLocalBounds();
    sf::Vector2f buttonPos = button.getPosition();
    sf::Vector2f buttonSize = button.getSize();

    text.setPosition({
        buttonPos.x + (buttonSize.x - textBounds.size.x) / 2.f - textBounds.position.x,
        buttonPos.y + (buttonSize.y - textBounds.size.y) / 2.f - textBounds.position.y - 2.f
        });
}

void GameSelect::handleMouseClick(sf::Vector2f mousePos, bool& openBlackjack, bool& openSlots, bool& openRoulette, bool& backToMain)
{
    openBlackjack = false;
    openSlots = false;
    openRoulette = false;
    backToMain = false;

    if (showingGameOverPopup)
    {
        if (gameOverButton.getGlobalBounds().contains(mousePos))
        {
            showingGameOverPopup = false;
            backToMain = true;
        }
        return;
    }

    if (showingAddMoneyPopup)
    {
        if (addConfirmButton.getGlobalBounds().contains(mousePos))
        {
            if (hasValidAddAmount())
            {
                currentBankroll += std::stod(addMoneyInput);
                addMoneyInput.clear();
                addHintText.setString("");
                addErrorText.setString("");
                showingAddMoneyPopup = false;
                refreshBankrollDisplay();
                refreshAddMoneyDisplay();
            }
            return;
        }

        if (addCancelButton.getGlobalBounds().contains(mousePos))
        {
            addMoneyInput.clear();
            addHintText.setString("");
            addErrorText.setString("");
            showingAddMoneyPopup = false;
            refreshAddMoneyDisplay();
            return;
        }

        return;
    }

    if (bankrollText.getGlobalBounds().contains(mousePos))
    {
        showingAddMoneyPopup = true;
        addMoneyInput.clear();
        addHintText.setString("");
        addErrorText.setString("");
        refreshAddMoneyDisplay();
        return;
    }

    if (blackjackButton.getGlobalBounds().contains(mousePos))
    {
        openBlackjack = true;
        messageText.setString("");
    }
    else if (rouletteButton.getGlobalBounds().contains(mousePos))
    {
        openRoulette = true;
        messageText.setString("");
    }
    else if (slotsButton.getGlobalBounds().contains(mousePos))
    {
        openSlots = true;
        messageText.setString("");
    }
    else if (backButton.getGlobalBounds().contains(mousePos))
    {
        backToMain = true;
        messageText.setString("");
    }
}

void GameSelect::draw(sf::RenderWindow& window)
{
    sf::RectangleShape background({ 1000.f, 760.f });
    background.setFillColor(sf::Color(0, 85, 20));
    window.draw(background);

    sf::RectangleShape topLine({ 700.f, 3.f });
    topLine.setPosition({ 150.f, 185.f });
    topLine.setFillColor(sf::Color(220, 180, 40));
    window.draw(topLine);

    sf::RectangleShape bottomLine({ 700.f, 3.f });
    bottomLine.setPosition({ 150.f, 550.f });
    bottomLine.setFillColor(sf::Color(220, 180, 40));
    window.draw(bottomLine);

    window.draw(bankrollText);
    window.draw(titleText);
    window.draw(subtitleText);

    window.draw(blackjackButton);
    window.draw(rouletteButton);
    window.draw(slotsButton);
    window.draw(backButton);

    window.draw(blackjackText);
    window.draw(rouletteText);
    window.draw(slotsText);
    window.draw(backText);
    window.draw(messageText);

    if (showingAddMoneyPopup)
    {
        window.draw(overlay);
        window.draw(popupPanel);
        window.draw(addMoneyLabelText);
        window.draw(addMoneyBox);
        window.draw(addMoneyInputText);

        if (addHintText.getString() != "")
        {
            window.draw(addHintText);
        }

        if (addErrorText.getString() != "")
        {
            window.draw(addErrorText);
        }

        window.draw(addConfirmButton);
        window.draw(addCancelButton);
        window.draw(addConfirmText);
        window.draw(addCancelText);
    }

    if (showingGameOverPopup)
    {
        window.draw(overlay);
        window.draw(gameOverPanel);
        window.draw(gameOverTitle);
        window.draw(gameOverMessage);
        window.draw(gameOverButton);
        window.draw(gameOverButtonText);
    }
}