#include "BlackjackUI.h"
#include "../../../core/SessionStats.h"
#include <string>
#include <vector>

BlackjackUI::BlackjackUI(sf::Font& sharedFont)
    : game(1000.0),
    sessionStats(nullptr),
    font(sharedFont),
    titleText(font, "BLACKJACK", 54),
    bankrollText(font, "", 28),
    betText(font, "", 28),
    dealerText(font, "", 24),
    playerText(font, "", 24),
    messageText(font, "", 24),
    statsText(font, "", 24),
    hitText(font, "HIT", 20),
    standText(font, "STAND", 20),
    doubleText(font, "DOUBLE", 20),
    newRoundText(font, "NEW ROUND", 20),
    backText(font, "BACK TO MENU", 20),
    plus10Text(font, "+10", 18),
    minus10Text(font, "-10", 18),
    betInputBoxText(font, "", 20),
    currentBet(50.0f),
    roundStarted(false),
    betInputString("50"),
    isTypingBet(false)
{
    titleText.setFillColor(sf::Color(90, 210, 255));
    titleText.setPosition({ 355.f, 40.f });

    bankrollText.setFillColor(sf::Color::White);
    bankrollText.setPosition({ 20.f, 20.f });

    betText.setFillColor(sf::Color::White);
    betText.setPosition({ 20.f, 55.f });

    dealerText.setFillColor(sf::Color(190, 235, 235));
    dealerText.setPosition({ 0.f, 160.f });

    messageText.setFillColor(sf::Color(190, 235, 235));
    messageText.setPosition({ 0.f, 320.f });

    playerText.setFillColor(sf::Color(190, 235, 235));
    playerText.setPosition({ 0.f, 445.f });

    statsText.setFillColor(sf::Color::Yellow);
    statsText.setPosition({ 0.f, 0.f });

    setupButtons();
    updateText();
}

void BlackjackUI::setStartingBankroll(double bankroll)
{
    game = BlackjackGame(bankroll);
    currentBet = 50.0f;
    betInputString = "50";
    isTypingBet = false;
    roundStarted = false;
    refreshBetInputText();
    updateText();
}

void BlackjackUI::setSessionStats(SessionStats* stats)
{
    sessionStats = stats;
}

double BlackjackUI::getCurrentBankroll() const
{
    return game.getTableBalance();
}

void BlackjackUI::setupButtons()
{
    hitButton.setSize({ 150.f, 50.f });
    hitButton.setPosition({ 55.f, 660.f });

    standButton.setSize({ 150.f, 50.f });
    standButton.setPosition({ 235.f, 660.f });

    doubleButton.setSize({ 150.f, 50.f });
    doubleButton.setPosition({ 415.f, 660.f });

    newRoundButton.setSize({ 180.f, 50.f });
    newRoundButton.setPosition({ 595.f, 660.f });

    backButton.setSize({ 180.f, 50.f });
    backButton.setPosition({ 785.f, 660.f });

    minus10Button.setSize({ 60.f, 40.f });
    minus10Button.setPosition({ 20.f, 90.f });
    minus10Button.setFillColor(sf::Color(70, 70, 110));

    betInputBox.setSize({ 100.f, 40.f });
    betInputBox.setPosition({ 90.f, 90.f });
    betInputBox.setFillColor(sf::Color(30, 30, 60));
    betInputBox.setOutlineThickness(2.f);
    betInputBox.setOutlineColor(sf::Color(90, 210, 255));

    plus10Button.setSize({ 60.f, 40.f });
    plus10Button.setPosition({ 200.f, 90.f });
    plus10Button.setFillColor(sf::Color(70, 70, 110));

    hitText.setFillColor(sf::Color::White);
    standText.setFillColor(sf::Color::White);
    doubleText.setFillColor(sf::Color::White);
    newRoundText.setFillColor(sf::Color::White);
    backText.setFillColor(sf::Color::White);
    plus10Text.setFillColor(sf::Color::White);
    minus10Text.setFillColor(sf::Color::White);
    betInputBoxText.setFillColor(sf::Color::White);

    centerTextInButton(hitText, hitButton);
    centerTextInButton(standText, standButton);
    centerTextInButton(doubleText, doubleButton);
    centerTextInButton(newRoundText, newRoundButton);
    centerTextInButton(backText, backButton);
    centerTextInButton(plus10Text, plus10Button);
    centerTextInButton(minus10Text, minus10Button);
}

void BlackjackUI::centerTextInButton(sf::Text& text, const sf::RectangleShape& button)
{
    auto bounds = text.getLocalBounds();
    auto pos = button.getPosition();
    auto size = button.getSize();

    text.setPosition({
        pos.x + (size.x - bounds.size.x) / 2.f - bounds.position.x,
        pos.y + (size.y - bounds.size.y) / 2.f - bounds.position.y
        });
}

void BlackjackUI::refreshBetInputText()
{
    betInputBoxText.setString(betInputString.empty() ? "$" : "$" + betInputString);
    centerTextInButton(betInputBoxText, betInputBox);
}

void BlackjackUI::handleTextEntered(unsigned int unicode)
{
    if (!isTypingBet) return;
    if (roundStarted && !game.isRoundOver()) return;

    if (unicode >= '0' && unicode <= '9')
    {
        if (betInputString.size() < 6)
        {
            betInputString += static_cast<char>(unicode);
            if (!betInputString.empty())
            {
                currentBet = std::stof(betInputString);
            }
            refreshBetInputText();
            updateText();
        }
    }
}

void BlackjackUI::handleBackspace()
{
    if (!isTypingBet) return;
    if (roundStarted && !game.isRoundOver()) return;

    if (!betInputString.empty())
    {
        betInputString.pop_back();
        if (betInputString.empty()) {
            currentBet = 0.0f;
        } else {
            currentBet = std::stof(betInputString);
        }
        refreshBetInputText();
        updateText();
    }
}

std::string BlackjackUI::getDealerDisplay() const
{
    if (!roundStarted)
    {
        return "Dealer\nNo round started";
    }

    if (game.isDealerHoleCardRevealed())
    {
        return "Dealer\n" + game.getDealerHand().toString() +
            "\nValue: " + std::to_string(game.getDealerValue());
    }

    std::vector<Card> cards = game.getDealerHand().getCards();

    if (cards.empty())
    {
        return "Dealer\nNo cards";
    }

    if (cards.size() == 1)
    {
        return "Dealer\n" + cards[0].toString();
    }

    return "Dealer\n" + cards[0].toString() + "  ??";
}

std::string BlackjackUI::getPlayerDisplay() const
{
    if (!roundStarted)
    {
        return "Player\nNo round started";
    }

    if (game.getHandCount() <= 1)
    {
        return "Player\n" + game.getPlayerHand().toString() +
            "\nValue: " + std::to_string(game.getPlayerHand().getValue()) +
            "\nBet: $" + std::to_string(static_cast<int>(currentBet));
    }

    std::string out = "Player Hands\n";

    for (int i = 0; i < game.getHandCount(); ++i)
    {
        out += "Hand " + std::to_string(i + 1);

        if (!game.isRoundOver() && i == game.getActiveHandIndex())
        {
            out += " [ACTIVE]";
        }

        out += "\n";
        out += game.getPlayerHand(i).toString();
        out += "\nValue: " + std::to_string(game.getPlayerHand(i).getValue());

        if (i < game.getHandCount() - 1)
        {
            out += "\n\n";
        }
    }

    out += "\nBet: $" + std::to_string(static_cast<int>(currentBet));
    return out;
}

std::string BlackjackUI::shortenResultText(const std::string& text) const
{
    if (text.find("Dealer Bust") != std::string::npos || text.find("Dealer bust") != std::string::npos)
    {
        return "Dealer busts. You win.";
    }

    if (text.find("Player Bust") != std::string::npos || text.find("Player bust") != std::string::npos)
    {
        return "Player busts. Dealer wins.";
    }

    if (text.find("Push") != std::string::npos || text.find("Tie") != std::string::npos)
    {
        return "Push.";
    }

    if (text.find("Blackjack") != std::string::npos)
    {
        return "Blackjack. You win.";
    }

    if (text.find("Player Win") != std::string::npos)
    {
        return "You win.";
    }

    if (text.find("Dealer Win") != std::string::npos)
    {
        return "Dealer wins.";
    }

    return text;
}

std::string BlackjackUI::getPostRoundStats() const
{
    if (!roundStarted || !game.isRoundOver())
    {
        return "";
    }

    BlackjackRoundSummary summary = game.getRoundSummary();

    std::string text;
    text += "Starting Bankroll: $" + std::to_string(static_cast<int>(summary.startingBankroll)) + "\n";
    text += "Ending Bankroll: $" + std::to_string(static_cast<int>(summary.endingBankroll)) + "\n";
    text += "Net Change: $" + std::to_string(static_cast<int>(summary.netChange)) + "\n";
    text += "Result: " + shortenResultText(game.getRoundResultText());

    return text;
}

std::string BlackjackUI::getStatusMessage() const
{
    if (!roundStarted)
    {
        return "Choose action: Click NEW ROUND to begin blackjack";
    }

    if (game.isRoundOver())
    {
        return shortenResultText(game.getRoundResultText());
    }

    std::string actions = "Choose action: ";
    bool first = true;

    if (game.canHit())
    {
        actions += "Hit";
        first = false;
    }

    if (game.canStand())
    {
        if (!first)
        {
            actions += ", ";
        }
        actions += "Stand";
        first = false;
    }

    if (game.canDoubleDown())
    {
        if (!first)
        {
            actions += ", ";
        }
        actions += "Double Down";
        first = false;
    }

    if (game.canSplit())
    {
        if (!first)
        {
            actions += ", ";
        }
        actions += "Split";
    }

    return actions;
}

void BlackjackUI::updateText()
{
    bankrollText.setString("Bankroll: $" + std::to_string(static_cast<int>(game.getTableBalance())));
    betText.setString("Set Bet:");

    dealerText.setString(getDealerDisplay());
    messageText.setString(getStatusMessage());
    playerText.setString(getPlayerDisplay());
    statsText.setString(getPostRoundStats());

    auto dealerBounds = dealerText.getLocalBounds();
    dealerText.setPosition({
        500.f - dealerBounds.size.x / 2.f - dealerBounds.position.x,
        160.f
        });

    auto messageBounds = messageText.getLocalBounds();
    messageText.setPosition({
        500.f - messageBounds.size.x / 2.f - messageBounds.position.x,
        320.f
        });

    auto playerBounds = playerText.getLocalBounds();
    playerText.setPosition({
        500.f - playerBounds.size.x / 2.f - playerBounds.position.x,
        445.f
        });
}

void BlackjackUI::handleGameClick(sf::Vector2f mousePos)
{
    bool roundWasAlreadyOver = (!roundStarted || game.isRoundOver());

    if (hitButton.getGlobalBounds().contains(mousePos))
    {
        if (roundStarted && !game.isRoundOver() && game.canHit())
        {
            game.playerHit();
        }
    }

    if (standButton.getGlobalBounds().contains(mousePos))
    {
        if (roundStarted && !game.isRoundOver() && game.canStand())
        {
            game.playerStand();
        }
    }

    if (doubleButton.getGlobalBounds().contains(mousePos))
    {
        if (roundStarted && !game.isRoundOver() && game.canDoubleDown())
        {
            game.playerDoubleDown();
        }
    }

    if (newRoundButton.getGlobalBounds().contains(mousePos))
    {
        if (!roundStarted || game.isRoundOver())
        {
            if (game.startNewRound(currentBet))
            {
                roundStarted = true;
                roundWasAlreadyOver = false;
                isTypingBet = false;
                betInputBox.setOutlineColor(sf::Color(90, 210, 255));
            }
        }
    }

    if (!roundStarted || game.isRoundOver())
    {
        if (minus10Button.getGlobalBounds().contains(mousePos))
        {
            currentBet = std::max(0.0f, currentBet - 10.0f);
            betInputString = std::to_string(static_cast<int>(currentBet));
            refreshBetInputText();
        }

        if (plus10Button.getGlobalBounds().contains(mousePos))
        {
            if (currentBet + 10.0f <= game.getTableBalance())
            {
                currentBet += 10.0f;
            }
            betInputString = std::to_string(static_cast<int>(currentBet));
            refreshBetInputText();
        }

        if (betInputBox.getGlobalBounds().contains(mousePos))
        {
            isTypingBet = true;
            betInputBox.setOutlineColor(sf::Color::Yellow);
        }
        else
        {
            isTypingBet = false;
            betInputBox.setOutlineColor(sf::Color(90, 210, 255));
        }
    }

    // Check if the round just ended as a result of the click
    if (roundStarted && !roundWasAlreadyOver && game.isRoundOver())
    {
        if (sessionStats)
        {
            sessionStats->recordBlackjackRound(game.getRoundSummary());
        }
    }

    updateText();
}

void BlackjackUI::handleScreenClick(sf::Vector2f mousePos, bool& backToMenu)
{
    backToMenu = false;

    if (backButton.getGlobalBounds().contains(mousePos))
    {
        roundStarted = false;
        updateText();
        backToMenu = true;
        return;
    }

    handleGameClick(mousePos);
}

void BlackjackUI::draw(sf::RenderWindow& window)
{
    sf::RectangleShape background({ 1000.f, 760.f });
    background.setFillColor(sf::Color(0, 0, 40));
    window.draw(background);

    for (int i = 0; i < 70; ++i)
    {
        sf::CircleShape star(1.2f);
        star.setFillColor(sf::Color(235, 235, 255));
        star.setPosition({
            static_cast<float>((i * 137) % 980),
            static_cast<float>((i * 83) % 740)
            });
        window.draw(star);
    }

    sf::RectangleShape line1({ 880.f, 3.f });
    line1.setPosition({ 58.f, 130.f });
    line1.setFillColor(sf::Color(90, 210, 255));
    window.draw(line1);

    auto playerBounds = playerText.getGlobalBounds();

    sf::RectangleShape line2({ 880.f, 3.f });
    line2.setPosition({ 58.f, playerBounds.position.y + playerBounds.size.y + 20.f });
    line2.setFillColor(sf::Color(90, 210, 255));
    window.draw(line2);

    window.draw(titleText);
    window.draw(bankrollText);
    window.draw(betText);

    if (roundStarted && game.isRoundOver())
    {
        auto drawCenteredYellowLine = [&](const std::string& text, float y, unsigned int size)
            {
                sf::Text line(font, text, size);
                line.setFillColor(sf::Color::Yellow);

                auto bounds = line.getLocalBounds();
                line.setPosition({
                    500.f - bounds.size.x / 2.f - bounds.position.x,
                    y
                    });

                window.draw(line);
            };

        float y = 150.f;

        drawCenteredYellowLine(
            "Dealer: " + game.getDealerHand().toString() +
            " (" + std::to_string(game.getDealerValue()) + ")",
            y,
            26
        );
        y += 50.f;

        drawCenteredYellowLine("Player Hands:", y, 26);
        y += 36.f;

        drawCenteredYellowLine(
            game.getPlayerHand().toString() +
            " (" + std::to_string(game.getPlayerHand().getValue()) + ")",
            y,
            26
        );
        y += 50.f;

        drawCenteredYellowLine("Results:", y, 26);
        y += 36.f;

        drawCenteredYellowLine(shortenResultText(game.getRoundResultText()), y, 26);
        y += 50.f;

        drawCenteredYellowLine("Round Insight:", y, 26);
        y += 36.f;

        drawCenteredYellowLine("You played that hand well.", y, 26);
        y += 50.f;

        drawCenteredYellowLine("Post Round Stats", y, 26);
        y += 36.f;

        drawCenteredYellowLine(
            "Bankroll: $" + std::to_string(static_cast<int>(game.getTableBalance())),
            y,
            26
        );
        y += 36.f;

        drawCenteredYellowLine(
            "Bet: $" + std::to_string(static_cast<int>(currentBet)),
            y,
            26
        );
    }
    else
    {
        dealerText.setFillColor(sf::Color(190, 235, 235));
        messageText.setFillColor(sf::Color(190, 235, 235));
        playerText.setFillColor(sf::Color(190, 235, 235));

        window.draw(dealerText);
        window.draw(messageText);
        window.draw(playerText);
    }

    bool controlsLocked = (roundStarted && !game.isRoundOver());

    minus10Button.setFillColor(controlsLocked ? sf::Color(40, 40, 60) : sf::Color(180, 65, 85));
    plus10Button.setFillColor(controlsLocked ? sf::Color(40, 40, 60) : sf::Color(60, 180, 60));
    betInputBox.setFillColor(controlsLocked ? sf::Color(20, 20, 40) : sf::Color(30, 30, 60));

    hitButton.setFillColor(
        (roundStarted && !game.isRoundOver() && game.canHit())
        ? sf::Color(50, 115, 230)
        : sf::Color(70, 70, 110)
    );

    standButton.setFillColor(
        (roundStarted && !game.isRoundOver() && game.canStand())
        ? sf::Color(50, 115, 230)
        : sf::Color(70, 70, 110)
    );

    doubleButton.setFillColor(
        (roundStarted && !game.isRoundOver() && game.canDoubleDown())
        ? sf::Color(50, 115, 230)
        : sf::Color(70, 70, 110)
    );

    newRoundButton.setFillColor(
        (!roundStarted || game.isRoundOver())
        ? sf::Color(50, 115, 230)
        : sf::Color(70, 70, 110)
    );

    backButton.setFillColor(sf::Color(180, 65, 85));

    window.draw(minus10Button);
    window.draw(betInputBox);
    window.draw(plus10Button);

    window.draw(hitButton);
    window.draw(standButton);
    window.draw(doubleButton);
    window.draw(newRoundButton);
    window.draw(backButton);

    window.draw(minus10Text);
    refreshBetInputText(); /* Make sure it's drawn correct */
    window.draw(betInputBoxText);
    window.draw(plus10Text);

    window.draw(hitText);
    window.draw(standText);
    window.draw(doubleText);
    window.draw(newRoundText);
    window.draw(backText);
}