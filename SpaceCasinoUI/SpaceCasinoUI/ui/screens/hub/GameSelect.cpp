#include "GameSelect.h"
#include <string>

GameSelect::GameSelect(sf::Font& sharedFont)
    : font(sharedFont),
    titleText(font, "SELECT A GAME", 52),
    subtitleText(font, "Choose where you want to play", 24),
    messageText(font, "", 22),
    bankrollText(font, "", 28),

    blackjackButton({ 220.f, 90.f }),
    rouletteButton({ 220.f, 90.f }),
    slotsButton({ 220.f, 90.f }),
    backButton({ 180.f, 50.f }),

    blackjackText(font, "BLACKJACK", 26),
    rouletteText(font, "ROULETTE", 26),
    slotsText(font, "SLOTS", 26),
    backText(font, "MAIN MENU", 20),

    overlay({ 1000.f, 760.f }),
    popupPanel({ 420.f, 240.f }),
    addMoneyBox({ 180.f, 48.f }),
    addConfirmButton({ 140.f, 45.f }),
    addCancelButton({ 140.f, 45.f }),

    addMoneyLabelText(font, "Add Money", 28),
    addMoneyInputText(font, "", 26),
    addHintText(font, "Enter amount and click confirm", 18),
    addErrorText(font, "", 18),
    addConfirmText(font, "CONFIRM", 18),
    addCancelText(font, "CANCEL", 18),

    showingGameOverPopup(false),
    gameOverPanel({ 460.f, 230.f }),
    gameOverTitle(font, "Game Over!", 34),
    gameOverMessage(font, "You're Broke!", 28),
    gameOverButton({ 180.f, 50.f }),
    gameOverButtonText(font, "MAIN MENU", 20),

    currentBankroll(0.0),
    addMoneyInput(""),
    showingAddMoneyPopup(false)
{
    titleText.setFillColor(sf::Color(255, 210, 90));
    subtitleText.setFillColor(sf::Color::White);
    messageText.setFillColor(sf::Color(255, 170, 170));
    bankrollText.setFillColor(sf::Color::White);

    titleText.setPosition({ 295.f, 35.f });
    subtitleText.setPosition({ 315.f, 105.f });
    messageText.setPosition({ 260.f, 620.f });
    bankrollText.setPosition({ 20.f, 20.f });

    blackjackButton.setPosition({ 390.f, 180.f });
    rouletteButton.setPosition({ 390.f, 310.f });
    slotsButton.setPosition({ 390.f, 440.f });
    backButton.setPosition({ 785.f, 660.f });

    blackjackButton.setFillColor(sf::Color(50, 115, 230));
    rouletteButton.setFillColor(sf::Color(180, 65, 85));
    slotsButton.setFillColor(sf::Color(150, 80, 220));
    backButton.setFillColor(sf::Color(80, 80, 90));

    blackjackButton.setOutlineThickness(2.f);
    rouletteButton.setOutlineThickness(2.f);
    slotsButton.setOutlineThickness(2.f);
    backButton.setOutlineThickness(2.f);

    blackjackButton.setOutlineColor(sf::Color(255, 210, 90));
    rouletteButton.setOutlineColor(sf::Color(255, 210, 90));
    slotsButton.setOutlineColor(sf::Color(255, 210, 90));
    backButton.setOutlineColor(sf::Color(255, 210, 90));

    blackjackText.setFillColor(sf::Color::White);
    rouletteText.setFillColor(sf::Color::White);
    slotsText.setFillColor(sf::Color::White);
    backText.setFillColor(sf::Color::White);

    centerTextInButton(blackjackText, blackjackButton);
    centerTextInButton(rouletteText, rouletteButton);
    centerTextInButton(slotsText, slotsButton);
    centerTextInButton(backText, backButton);

    overlay.setFillColor(sf::Color(0, 0, 0, 160));

    popupPanel.setPosition({ 290.f, 220.f });
    popupPanel.setFillColor(sf::Color(30, 35, 45));
    popupPanel.setOutlineThickness(2.f);
    popupPanel.setOutlineColor(sf::Color(255, 210, 90));

    addMoneyLabelText.setFillColor(sf::Color(255, 210, 90));
    addMoneyLabelText.setPosition({ 415.f, 245.f });

    addMoneyBox.setPosition({ 410.f, 305.f });
    addMoneyBox.setFillColor(sf::Color(20, 20, 35));
    addMoneyBox.setOutlineThickness(2.f);
    addMoneyBox.setOutlineColor(sf::Color(255, 210, 90));

    addMoneyInputText.setFillColor(sf::Color::White);

    addHintText.setFillColor(sf::Color(200, 200, 200));
    addHintText.setPosition({ 355.f, 365.f });

    addErrorText.setFillColor(sf::Color(255, 140, 140));
    addErrorText.setPosition({ 350.f, 395.f });

    addConfirmButton.setPosition({ 330.f, 430.f });
    addCancelButton.setPosition({ 530.f, 430.f });

    addConfirmButton.setFillColor(sf::Color(50, 115, 230));
    addCancelButton.setFillColor(sf::Color(180, 65, 85));

    addConfirmButton.setOutlineThickness(2.f);
    addCancelButton.setOutlineThickness(2.f);
    addConfirmButton.setOutlineColor(sf::Color(255, 210, 90));
    addCancelButton.setOutlineColor(sf::Color(255, 210, 90));

    addConfirmText.setFillColor(sf::Color::White);
    addCancelText.setFillColor(sf::Color::White);

    centerTextInButton(addConfirmText, addConfirmButton);
    centerTextInButton(addCancelText, addCancelButton);

    gameOverPanel.setPosition({ 270.f, 225.f });
    gameOverPanel.setFillColor(sf::Color(30, 35, 45));
    gameOverPanel.setOutlineThickness(2.f);
    gameOverPanel.setOutlineColor(sf::Color(255, 210, 90));

    gameOverTitle.setFillColor(sf::Color(255, 140, 140));
    gameOverMessage.setFillColor(sf::Color::White);

    gameOverTitle.setPosition({ 385.f, 255.f });
    gameOverMessage.setPosition({ 390.f, 310.f });

    gameOverButton.setPosition({ 410.f, 385.f });
    gameOverButton.setFillColor(sf::Color(180, 65, 85));
    gameOverButton.setOutlineThickness(2.f);
    gameOverButton.setOutlineColor(sf::Color(255, 210, 90));

    gameOverButtonText.setFillColor(sf::Color::White);
    centerTextInButton(gameOverButtonText, gameOverButton);

    refreshBankrollDisplay();
    refreshAddMoneyDisplay();
}

void GameSelect::centerTextInButton(sf::Text& text, const sf::RectangleShape& button)
{
    sf::FloatRect bounds = text.getLocalBounds();
    sf::Vector2f pos = button.getPosition();
    sf::Vector2f size = button.getSize();

    text.setPosition({
        pos.x + (size.x - bounds.size.x) / 2.f - bounds.position.x,
        pos.y + (size.y - bounds.size.y) / 2.f - bounds.position.y
        });
}

void GameSelect::refreshBankrollDisplay()
{
    bankrollText.setString("Bankroll: $" + std::to_string(static_cast<int>(currentBankroll)));
}

void GameSelect::refreshAddMoneyDisplay()
{
    std::string shown = addMoneyInput.empty() ? "$" : "$" + addMoneyInput;
    addMoneyInputText.setString(shown);

    sf::FloatRect bounds = addMoneyInputText.getLocalBounds();
    addMoneyInputText.setPosition({
        addMoneyBox.getPosition().x + (addMoneyBox.getSize().x - bounds.size.x) / 2.f - bounds.position.x,
        addMoneyBox.getPosition().y + (addMoneyBox.getSize().y - bounds.size.y) / 2.f - bounds.position.y
        });
}

bool GameSelect::hasValidAddAmount() const
{
    if (addMoneyInput.empty())
        return false;

    try
    {
        int amount = std::stoi(addMoneyInput);
        return amount > 0;
    }
    catch (...)
    {
        return false;
    }
}

void GameSelect::setBankroll(double bankroll)
{
    currentBankroll = bankroll;
    refreshBankrollDisplay();

    if (currentBankroll <= 0.0)
    {
        showingGameOverPopup = true;
        showingAddMoneyPopup = false;
    }
    else
    {
        showingGameOverPopup = false;
    }
}

double GameSelect::getBankroll() const
{
    return currentBankroll;
}

void GameSelect::handleTextEntered(unsigned int unicode)
{
    if (!showingAddMoneyPopup)
        return;

    if (unicode >= '0' && unicode <= '9')
    {
        if (addMoneyInput.size() < 7)
        {
            addMoneyInput += static_cast<char>(unicode);
            addErrorText.setString("");
            refreshAddMoneyDisplay();
        }
    }
}

void GameSelect::handleBackspace()
{
    if (!showingAddMoneyPopup)
        return;

    if (!addMoneyInput.empty())
    {
        addMoneyInput.pop_back();
        addErrorText.setString("");
        refreshAddMoneyDisplay();
    }
}

void GameSelect::handleMouseClick(sf::Vector2f mousePos, bool& openBlackjack, bool& openRoulette, bool& openSlots, bool& backToMain)
{
    openBlackjack = false;
    openRoulette = false;
    openSlots = false;
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
        if (addMoneyBox.getGlobalBounds().contains(mousePos))
        {
            return;
        }

        if (addConfirmButton.getGlobalBounds().contains(mousePos))
        {
            if (hasValidAddAmount())
            {
                currentBankroll += std::stoi(addMoneyInput);
                addMoneyInput.clear();
                addErrorText.setString("");
                showingAddMoneyPopup = false;
                refreshBankrollDisplay();
                refreshAddMoneyDisplay();
            }
            else
            {
                addErrorText.setString("Enter a valid amount greater than 0.");
            }
            return;
        }

        if (addCancelButton.getGlobalBounds().contains(mousePos))
        {
            addMoneyInput.clear();
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
        addErrorText.setString("");
        refreshAddMoneyDisplay();
        return;
    }

    if (blackjackButton.getGlobalBounds().contains(mousePos))
    {
        if (currentBankroll > 0.0)
        {
            openBlackjack = true;
            messageText.setString("");
        }
        return;
    }

    if (rouletteButton.getGlobalBounds().contains(mousePos))
    {
        if (currentBankroll > 0.0)
        {
            openRoulette = true;
            messageText.setString("");
        }
        return;
    }

    if (slotsButton.getGlobalBounds().contains(mousePos))
    {
        if (currentBankroll > 0.0)
        {
            openSlots = true;
            messageText.setString("");
        }
        return;
    }

    if (backButton.getGlobalBounds().contains(mousePos))
    {
        backToMain = true;
        return;
    }
}

void GameSelect::draw(sf::RenderWindow& window)
{
    sf::RectangleShape background({ 1000.f, 760.f });
    background.setFillColor(sf::Color(20, 55, 30));
    window.draw(background);

    sf::RectangleShape topLine({ 880.f, 3.f });
    topLine.setPosition({ 58.f, 140.f });
    topLine.setFillColor(sf::Color(255, 210, 90));
    window.draw(topLine);

    sf::RectangleShape bottomLine({ 880.f, 3.f });
    bottomLine.setPosition({ 58.f, 600.f });
    bottomLine.setFillColor(sf::Color(255, 210, 90));
    window.draw(bottomLine);

    window.draw(titleText);
    window.draw(subtitleText);
    window.draw(bankrollText);
    window.draw(messageText);

    window.draw(blackjackButton);
    window.draw(rouletteButton);
    window.draw(slotsButton);
    window.draw(backButton);

    window.draw(blackjackText);
    window.draw(rouletteText);
    window.draw(slotsText);
    window.draw(backText);

    if (showingAddMoneyPopup)
    {
        window.draw(overlay);
        window.draw(popupPanel);
        window.draw(addMoneyLabelText);
        window.draw(addMoneyBox);
        window.draw(addMoneyInputText);
        window.draw(addHintText);
        window.draw(addErrorText);
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