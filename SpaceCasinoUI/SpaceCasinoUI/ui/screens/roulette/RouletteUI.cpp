#include "RouletteUI.h"
#include <string>

RouletteUI::RouletteUI(sf::Font& sharedFont)
    : game(1000),
    font(sharedFont),
    titleText(font, "ROULETTE", 54),
    bankrollText(font, "", 28),
    betText(font, "", 28),
    selectedBetText(font, "", 24),
    numberInputLabelText(font, "Straight Number:", 22),
    numberInputText(font, "", 24),
    resultText(font, "", 26),
    payoutText(font, "", 24),
    spinText(font, "SPIN", 20),
    backText(font, "BACK TO MENU", 20),
    redText(font, "RED", 18),
    blackText(font, "BLACK", 18),
    evenText(font, "EVEN", 18),
    oddText(font, "ODD", 18),
    lowText(font, "1 TO 18", 18),
    highText(font, "19 TO 36", 18),
    straightText(font, "STRAIGHT", 18),
    currentBet(50.0),
    numberInput("0"),
    enteringNumber(false),
    selectedBet(SelectedBet::Red),
    hasSpun(false)
{
    titleText.setFillColor(sf::Color(255, 85, 85));
    titleText.setPosition({ 355.f, 35.f });

    bankrollText.setFillColor(sf::Color::White);
    bankrollText.setPosition({ 20.f, 20.f });

    betText.setFillColor(sf::Color::White);
    betText.setPosition({ 20.f, 55.f });

    selectedBetText.setFillColor(sf::Color(255, 230, 230));
    selectedBetText.setPosition({ 345.f, 150.f });

    numberInputLabelText.setFillColor(sf::Color(255, 230, 230));
    numberInputLabelText.setPosition({ 350.f, 195.f });

    numberBox.setSize({ 120.f, 46.f });
    numberBox.setPosition({ 445.f, 230.f });
    numberBox.setFillColor(sf::Color(28, 10, 18));
    numberBox.setOutlineThickness(2.f);
    numberBox.setOutlineColor(sf::Color(255, 70, 70));

    numberInputText.setFillColor(sf::Color::White);
    numberInputText.setPosition({ 0.f, 0.f });

    resultText.setFillColor(sf::Color::White);
    resultText.setPosition({ 0.f, 520.f });

    payoutText.setFillColor(sf::Color(255, 220, 90));
    payoutText.setPosition({ 0.f, 560.f });

    redButton.setSize({ 150.f, 55.f });
    redButton.setPosition({ 120.f, 320.f });

    blackButton.setSize({ 150.f, 55.f });
    blackButton.setPosition({ 300.f, 320.f });

    evenButton.setSize({ 150.f, 55.f });
    evenButton.setPosition({ 480.f, 320.f });

    oddButton.setSize({ 150.f, 55.f });
    oddButton.setPosition({ 660.f, 320.f });

    lowButton.setSize({ 150.f, 55.f });
    lowButton.setPosition({ 210.f, 400.f });

    highButton.setSize({ 150.f, 55.f });
    highButton.setPosition({ 390.f, 400.f });

    straightButton.setSize({ 150.f, 55.f });
    straightButton.setPosition({ 570.f, 400.f });

    spinButton.setSize({ 180.f, 50.f });
    spinButton.setPosition({ 595.f, 660.f });

    backButton.setSize({ 180.f, 50.f });
    backButton.setPosition({ 785.f, 660.f });

    redText.setFillColor(sf::Color::White);
    blackText.setFillColor(sf::Color::White);
    evenText.setFillColor(sf::Color::White);
    oddText.setFillColor(sf::Color::White);
    lowText.setFillColor(sf::Color::White);
    highText.setFillColor(sf::Color::White);
    straightText.setFillColor(sf::Color::White);
    spinText.setFillColor(sf::Color::White);
    backText.setFillColor(sf::Color::White);

    centerTextInButton(redText, redButton);
    centerTextInButton(blackText, blackButton);
    centerTextInButton(evenText, evenButton);
    centerTextInButton(oddText, oddButton);
    centerTextInButton(lowText, lowButton);
    centerTextInButton(highText, highButton);
    centerTextInButton(straightText, straightButton);
    centerTextInButton(spinText, spinButton);
    centerTextInButton(backText, backButton);

    updateText();
}

void RouletteUI::setStartingBankroll(double bankroll)
{
    game = RouletteGame(static_cast<int>(bankroll));
    currentBet = 50.0;
    numberInput = "0";
    enteringNumber = false;
    selectedBet = SelectedBet::Red;
    hasSpun = false;
    updateText();
}

double RouletteUI::getCurrentBankroll() const
{
    return static_cast<double>(game.getBalance());
}

void RouletteUI::centerTextInButton(sf::Text& text, const sf::RectangleShape& button)
{
    sf::FloatRect bounds = text.getLocalBounds();
    sf::Vector2f pos = button.getPosition();
    sf::Vector2f size = button.getSize();

    text.setPosition({
        pos.x + (size.x - bounds.size.x) / 2.f - bounds.position.x,
        pos.y + (size.y - bounds.size.y) / 2.f - bounds.position.y
        });
}

std::string RouletteUI::getSelectedBetLabel() const
{
    switch (selectedBet)
    {
    case SelectedBet::Red: return "Selected Bet: Red";
    case SelectedBet::Black: return "Selected Bet: Black";
    case SelectedBet::Even: return "Selected Bet: Even";
    case SelectedBet::Odd: return "Selected Bet: Odd";
    case SelectedBet::Low: return "Selected Bet: 1 to 18";
    case SelectedBet::High: return "Selected Bet: 19 to 36";
    case SelectedBet::Straight: return "Selected Bet: Straight Up";
    default: return "Selected Bet";
    }
}

void RouletteUI::updateText()
{
    bankrollText.setString("Bankroll: $" + std::to_string(game.getBalance()));
    betText.setString("Current Bet: $" + std::to_string(static_cast<int>(currentBet)));
    selectedBetText.setString(getSelectedBetLabel());

    numberInputText.setString(numberInput);

    auto numberBounds = numberInputText.getLocalBounds();
    numberInputText.setPosition({
        numberBox.getPosition().x + (numberBox.getSize().x - numberBounds.size.x) / 2.f - numberBounds.position.x,
        numberBox.getPosition().y + (numberBox.getSize().y - numberBounds.size.y) / 2.f - numberBounds.position.y
        });

    auto resultBounds = resultText.getLocalBounds();
    resultText.setPosition({
        500.f - resultBounds.size.x / 2.f - resultBounds.position.x,
        520.f
        });

    auto payoutBounds = payoutText.getLocalBounds();
    payoutText.setPosition({
        500.f - payoutBounds.size.x / 2.f - payoutBounds.position.x,
        560.f
        });
}

void RouletteUI::spinRound()
{
    if (!game.canPlaceBet(static_cast<int>(currentBet)))
    {
        resultText.setString("Not enough bankroll for that bet.");
        payoutText.setString("");
        updateText();
        return;
    }

    RouletteBet bet(Color::Red, static_cast<int>(currentBet));

    switch (selectedBet)
    {
    case SelectedBet::Red:
        bet = RouletteBet(Color::Red, static_cast<int>(currentBet));
        break;
    case SelectedBet::Black:
        bet = RouletteBet(Color::Black, static_cast<int>(currentBet));
        break;
    case SelectedBet::Even:
        bet = RouletteBet(BetType::EvenOdd, 0, static_cast<int>(currentBet));
        break;
    case SelectedBet::Odd:
        bet = RouletteBet(BetType::EvenOdd, 1, static_cast<int>(currentBet));
        break;
    case SelectedBet::Low:
        bet = RouletteBet(BetType::HighLow, 0, static_cast<int>(currentBet));
        break;
    case SelectedBet::High:
        bet = RouletteBet(BetType::HighLow, 1, static_cast<int>(currentBet));
        break;
    case SelectedBet::Straight:
    {
        int chosenNumber = 0;
        try
        {
            chosenNumber = std::stoi(numberInput);
        }
        catch (...)
        {
            chosenNumber = 0;
        }

        if (chosenNumber < 0 || chosenNumber > 36)
        {
            resultText.setString("Straight number must be from 0 to 36.");
            payoutText.setString("");
            updateText();
            return;
        }

        bet = RouletteBet(BetType::StraightUp, chosenNumber, static_cast<int>(currentBet));
        break;
    }
    }

    int before = game.getBalance();

    game.placeBet(bet);
    game.spin();
    game.resolve();

    RouletteRoundResult round = game.getLastResult();
    int after = game.getBalance();
    int net = after - before;

    resultText.setString(round.toString());

    if (net > 0)
    {
        payoutText.setString("You won $" + std::to_string(net));
    }
    else if (net < 0)
    {
        payoutText.setString("You lost $" + std::to_string(-net));
    }
    else
    {
        payoutText.setString("Push");
    }

    hasSpun = true;
    updateText();
}

void RouletteUI::handleTextEntered(unsigned int unicode)
{
    if (!enteringNumber || selectedBet != SelectedBet::Straight)
    {
        return;
    }

    if (unicode >= '0' && unicode <= '9')
    {
        if (numberInput.size() < 2)
        {
            if (numberInput == "0")
            {
                numberInput = "";
            }
            numberInput += static_cast<char>(unicode);
        }
    }

    updateText();
}

void RouletteUI::handleBackspace()
{
    if (!enteringNumber || selectedBet != SelectedBet::Straight)
    {
        return;
    }

    if (!numberInput.empty())
    {
        numberInput.pop_back();
    }

    if (numberInput.empty())
    {
        numberInput = "0";
    }

    updateText();
}

void RouletteUI::handleScreenClick(sf::Vector2f mousePos, bool& backToMenu)
{
    backToMenu = false;

    if (backButton.getGlobalBounds().contains(mousePos))
    {
        backToMenu = true;
        return;
    }

    enteringNumber = false;

    if (numberBox.getGlobalBounds().contains(mousePos))
    {
        enteringNumber = true;
    }
    else if (redButton.getGlobalBounds().contains(mousePos))
    {
        selectedBet = SelectedBet::Red;
    }
    else if (blackButton.getGlobalBounds().contains(mousePos))
    {
        selectedBet = SelectedBet::Black;
    }
    else if (evenButton.getGlobalBounds().contains(mousePos))
    {
        selectedBet = SelectedBet::Even;
    }
    else if (oddButton.getGlobalBounds().contains(mousePos))
    {
        selectedBet = SelectedBet::Odd;
    }
    else if (lowButton.getGlobalBounds().contains(mousePos))
    {
        selectedBet = SelectedBet::Low;
    }
    else if (highButton.getGlobalBounds().contains(mousePos))
    {
        selectedBet = SelectedBet::High;
    }
    else if (straightButton.getGlobalBounds().contains(mousePos))
    {
        selectedBet = SelectedBet::Straight;
        enteringNumber = true;
    }
    else if (spinButton.getGlobalBounds().contains(mousePos))
    {
        spinRound();
        return;
    }

    updateText();
}

void RouletteUI::draw(sf::RenderWindow& window)
{
    sf::RectangleShape background({ 1000.f, 760.f });
    background.setFillColor(sf::Color(26, 0, 4));
    window.draw(background);

    for (int i = 0; i < 85; ++i)
    {
        sf::CircleShape star(1.2f);
        star.setFillColor(sf::Color::White);

        float x = static_cast<float>((i * 97) % 1000);
        float y = static_cast<float>((i * 53) % 760);

        if (i % 7 == 0)
        {
            star.setRadius(1.8f);
        }

        star.setPosition({ x, y });
        window.draw(star);
    }

    sf::RectangleShape topLine({ 880.f, 3.f });
    topLine.setPosition({ 58.f, 125.f });
    topLine.setFillColor(sf::Color(255, 70, 70));
    window.draw(topLine);

    sf::RectangleShape bottomLine({ 880.f, 3.f });
    bottomLine.setPosition({ 58.f, 610.f });
    bottomLine.setFillColor(sf::Color(255, 70, 70));
    window.draw(bottomLine);

    window.draw(titleText);
    window.draw(bankrollText);
    window.draw(betText);
    window.draw(selectedBetText);
    window.draw(numberInputLabelText);
    window.draw(numberBox);
    window.draw(numberInputText);

    redButton.setFillColor(sf::Color(200, 30, 40));
    blackButton.setFillColor(sf::Color(35, 35, 35));
    evenButton.setFillColor(sf::Color(120, 30, 40));
    oddButton.setFillColor(sf::Color(120, 30, 40));
    lowButton.setFillColor(sf::Color(150, 40, 40));
    highButton.setFillColor(sf::Color(150, 40, 40));
    straightButton.setFillColor(sf::Color(200, 90, 40));
    spinButton.setFillColor(sf::Color(220, 50, 50));
    backButton.setFillColor(sf::Color(180, 40, 60));

    window.draw(redButton);
    window.draw(blackButton);
    window.draw(evenButton);
    window.draw(oddButton);
    window.draw(lowButton);
    window.draw(highButton);
    window.draw(straightButton);
    window.draw(spinButton);
    window.draw(backButton);

    window.draw(redText);
    window.draw(blackText);
    window.draw(evenText);
    window.draw(oddText);
    window.draw(lowText);
    window.draw(highText);
    window.draw(straightText);
    window.draw(spinText);
    window.draw(backText);
    window.draw(resultText);
    window.draw(payoutText);
}