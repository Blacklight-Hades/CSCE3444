#include "SlotsUI.h"
#include "../../../core/SessionStats.h"
#include <string>

SlotsUI::SlotsUI(sf::Font& sharedFont)
    : game(1000.0),
    sessionStats(nullptr),
    font(sharedFont),
    titleText(font, "SLOTS", 54),
    bankrollText(font, "", 28),
    betText(font, "", 28),
    resultText(font, "", 26),
    payoutText(font, "", 24),
    backText(font, "BACK TO MENU", 20),
    spinText(font, "SPIN", 20),
    plus10Text(font, "+10", 18),
    minus10Text(font, "-10", 18),
    betInputBoxText(font, "", 20),
    hasSpun(false),
    currentBet(50.0),
    lastPayout(0.0),
    betInputString("50"),
    isTypingBet(false)
{
    titleText.setFillColor(sf::Color(210, 120, 255));
    titleText.setPosition({ 425.f, 40.f });

    bankrollText.setFillColor(sf::Color::White);
    bankrollText.setPosition({ 20.f, 20.f });

    betText.setFillColor(sf::Color::White);
    betText.setPosition({ 20.f, 55.f });

    resultText.setFillColor(sf::Color(230, 230, 255));
    resultText.setPosition({ 0.f, 500.f });

    payoutText.setFillColor(sf::Color::Yellow);
    payoutText.setPosition({ 0.f, 545.f });

    spinButton.setSize({ 180.f, 50.f });
    spinButton.setPosition({ 595.f, 660.f });

    backButton.setSize({ 180.f, 50.f });
    backButton.setPosition({ 785.f, 660.f });

    minus10Button.setSize({ 60.f, 40.f });
    minus10Button.setPosition({ 20.f, 90.f });
    minus10Button.setFillColor(sf::Color(70, 70, 110));

    betInputBox.setSize({ 100.f, 40.f });
    betInputBox.setPosition({ 90.f, 90.f });
    betInputBox.setFillColor(sf::Color(30, 30, 60));
    betInputBox.setOutlineThickness(2.f);
    betInputBox.setOutlineColor(sf::Color(210, 120, 255));

    plus10Button.setSize({ 60.f, 40.f });
    plus10Button.setPosition({ 200.f, 90.f });
    plus10Button.setFillColor(sf::Color(70, 70, 110));

    spinText.setFillColor(sf::Color::White);
    backText.setFillColor(sf::Color::White);
    plus10Text.setFillColor(sf::Color::White);
    minus10Text.setFillColor(sf::Color::White);
    betInputBoxText.setFillColor(sf::Color::White);

    centerTextInButton(spinText, spinButton);
    centerTextInButton(backText, backButton);
    centerTextInButton(plus10Text, plus10Button);
    centerTextInButton(minus10Text, minus10Button);

    updateText();
}

void SlotsUI::setStartingBankroll(double bankroll)
{
    game = Slots(bankroll);
    hasSpun = false;
    currentBet = 50.0;
    betInputString = "50";
    isTypingBet = false;
    lastPayout = 0.0;
    refreshBetInputText();
    updateText();
}

void SlotsUI::setSessionStats(SessionStats* stats)
{
    sessionStats = stats;
}

double SlotsUI::getCurrentBankroll() const
{
    return game.getBankroll();
}

void SlotsUI::centerTextInButton(sf::Text& text, const sf::RectangleShape& button)
{
    sf::FloatRect bounds = text.getLocalBounds();
    sf::Vector2f pos = button.getPosition();
    sf::Vector2f size = button.getSize();

    text.setPosition({
        pos.x + (size.x - bounds.size.x) / 2.f - bounds.position.x,
        pos.y + (size.y - bounds.size.y) / 2.f - bounds.position.y
        });
}

void SlotsUI::refreshBetInputText()
{
    betInputBoxText.setString(betInputString.empty() ? "$" : "$" + betInputString);
    centerTextInButton(betInputBoxText, betInputBox);
}

void SlotsUI::handleTextEntered(unsigned int unicode)
{
    if (!isTypingBet) return;

    if (unicode >= '0' && unicode <= '9')
    {
        if (betInputString.size() < 6)
        {
            betInputString += static_cast<char>(unicode);
            if (!betInputString.empty())
            {
                currentBet = std::stod(betInputString);
            }
            refreshBetInputText();
            updateText();
        }
    }
}

void SlotsUI::handleBackspace()
{
    if (!isTypingBet) return;

    if (!betInputString.empty())
    {
        betInputString.pop_back();
        if (betInputString.empty()) {
            currentBet = 0.0;
        } else {
            currentBet = std::stod(betInputString);
        }
        refreshBetInputText();
        updateText();
    }
}

std::string SlotsUI::symbolToString(char c) const
{
    switch (c)
    {
    case 'B': return "BAR";
    case 'V': return "7";
    case 'J': return "J";
    case 'Q': return "Q";
    case 'K': return "K";
    case 'T': return "10";
    case 'S': return "STAR";
    case 'A': return "ALIEN";
    case 'M': return "MOON";
    case 'R': return "ROCKET";
    case 'W': return "2X";
    case 'F': return "5X";
    default:  return "?";
    }
}

void SlotsUI::updateText()
{
    bankrollText.setString("Bankroll: $" + std::to_string(static_cast<int>(game.getBankroll())));
    betText.setString("Set Bet:");

    if (!hasSpun)
    {
        resultText.setString("Click SPIN to play slots");
        payoutText.setString("");
    }
    else
    {
        if (lastPayout > 0.0)
        {
            resultText.setString("Nice spin. You won.");
            payoutText.setString("Payout: $" + std::to_string(static_cast<int>(lastPayout)));
        }
        else
        {
            resultText.setString("No payout this spin.");
            payoutText.setString("Payout: $0");
        }
    }

    sf::FloatRect resultBounds = resultText.getLocalBounds();
    resultText.setPosition({
        500.f - resultBounds.size.x / 2.f - resultBounds.position.x,
        500.f
        });

    sf::FloatRect payoutBounds = payoutText.getLocalBounds();
    payoutText.setPosition({
        500.f - payoutBounds.size.x / 2.f - payoutBounds.position.x,
        545.f
        });
}

void SlotsUI::handleScreenClick(sf::Vector2f mousePos, bool& backToMenu)
{
    backToMenu = false;

    if (backButton.getGlobalBounds().contains(mousePos))
    {
        backToMenu = true;
        return;
    }

    if (minus10Button.getGlobalBounds().contains(mousePos))
    {
        currentBet = std::max(0.0, currentBet - 10.0);
        betInputString = std::to_string(static_cast<int>(currentBet));
        refreshBetInputText();
    }

    if (plus10Button.getGlobalBounds().contains(mousePos))
    {
        if (currentBet + 10.0 <= game.getBankroll())
        {
            currentBet += 10.0;
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
        betInputBox.setOutlineColor(sf::Color(210, 120, 255));
    }

    if (spinButton.getGlobalBounds().contains(mousePos))
    {
        if (game.getBankroll() >= currentBet && currentBet > 0.0)
        {
            currentWindow = game.reelsSpin(currentBet);
            lastPayout = game.paytable();
            hasSpun = true;
            isTypingBet = false;
            betInputBox.setOutlineColor(sf::Color(210, 120, 255));

            
            if (sessionStats)
            {
                SlotsRoundSummary summary;
                summary.betAmount = currentBet;
                summary.payoutAmount = lastPayout;
                summary.netChange = lastPayout - currentBet;
                
                // check 3 in a row
                summary.wasThreeInARow = (currentWindow.getDisplay(0, 1) == currentWindow.getDisplay(1, 1) &&
                                          currentWindow.getDisplay(1, 1) == currentWindow.getDisplay(2, 1));
                
                // check jackpot (bar or seven triple)
                summary.wasJackpot = summary.wasThreeInARow && 
                                     (currentWindow.getDisplay(0, 1) == 'B' || currentWindow.getDisplay(0, 1) == 'V');
                                     
                sessionStats->recordSlotsRound(summary);
            }
            
            updateText();
        }
    }
}

void SlotsUI::draw(sf::RenderWindow& window)
{
    sf::RectangleShape background({ 1000.f, 760.f });
    background.setFillColor(sf::Color(18, 0, 40));
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

    sf::RectangleShape topLine({ 880.f, 3.f });
    topLine.setPosition({ 58.f, 130.f });
    topLine.setFillColor(sf::Color(210, 120, 255));
    window.draw(topLine);

    sf::RectangleShape bottomLine({ 880.f, 3.f });
    bottomLine.setPosition({ 58.f, 610.f });
    bottomLine.setFillColor(sf::Color(210, 120, 255));
    window.draw(bottomLine);

    window.draw(titleText);
    window.draw(bankrollText);
    window.draw(betText);

    sf::RectangleShape machineBody({ 520.f, 300.f });
    machineBody.setPosition({ 240.f, 180.f });
    machineBody.setFillColor(sf::Color(45, 35, 75));
    machineBody.setOutlineThickness(3.f);
    machineBody.setOutlineColor(sf::Color(210, 120, 255));
    window.draw(machineBody);

    const float startX = 290.f;
    const float startY = 210.f;
    const float cellW = 130.f;
    const float cellH = 80.f;

    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            sf::RectangleShape cell({ cellW - 10.f, cellH - 10.f });
            cell.setPosition({
                startX + col * cellW,
                startY + row * cellH
                });
            cell.setFillColor(sf::Color(20, 20, 35));
            cell.setOutlineThickness(2.f);
            cell.setOutlineColor(sf::Color(110, 80, 160));
            window.draw(cell);

            std::string displayText = "?";

            if (hasSpun)
            {
                displayText = symbolToString(currentWindow.getDisplay(col, row));
            }

            sf::Text symbol(font, displayText, 24);
            symbol.setFillColor(sf::Color::White);

            sf::FloatRect bounds = symbol.getLocalBounds();
            symbol.setPosition({
                cell.getPosition().x + (cell.getSize().x - bounds.size.x) / 2.f - bounds.position.x,
                cell.getPosition().y + (cell.getSize().y - bounds.size.y) / 2.f - bounds.position.y
                });

            window.draw(symbol);
        }
    }

    spinButton.setFillColor(
        (game.getBankroll() >= currentBet && currentBet > 0.0)
        ? sf::Color(125, 45, 180)
        : sf::Color(70, 70, 110)
    );

    minus10Button.setFillColor(sf::Color(180, 65, 85));
    plus10Button.setFillColor(sf::Color(60, 180, 60));

    backButton.setFillColor(sf::Color(180, 65, 85));

    window.draw(minus10Button);
    window.draw(betInputBox);
    window.draw(plus10Button);

    window.draw(spinButton);
    window.draw(backButton);
    window.draw(spinText);
    window.draw(backText);

    window.draw(minus10Text);
    refreshBetInputText();
    window.draw(betInputBoxText);
    window.draw(plus10Text);

    window.draw(resultText);
    window.draw(payoutText);
}