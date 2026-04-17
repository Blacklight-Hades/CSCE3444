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
    messageText(font, "", 32),
    hitText(font, "HIT", 20),
    standText(font, "STAND", 20),
    doubleText(font, "DOUBLE", 20),
    newRoundText(font, "NEW ROUND", 20),
    backText(font, "BACK TO MENU", 20),
    statsText(font, "", 20),
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

    messageText.setFillColor(sf::Color(230, 230, 255));
    messageText.setPosition({ 0.f, 400.f });

    statsText.setFillColor(sf::Color::Yellow);
    statsText.setPosition({ 0.f, 0.f });

    setupButtons();
    loadCardTextures();
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

    newRoundButton.setSize({ 150.f, 50.f });
    newRoundButton.setPosition({ 595.f, 660.f });

    statsButton.setSize({ 150.f, 50.f });
    statsButton.setPosition({ 830.f, 20.f });

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
    statsText.setFillColor(sf::Color::White);
    plus10Text.setFillColor(sf::Color::White);
    minus10Text.setFillColor(sf::Color::White);
    betInputBoxText.setFillColor(sf::Color::White);

    centerTextInButton(hitText, hitButton);
    centerTextInButton(standText, standButton);
    centerTextInButton(doubleText, doubleButton);
    centerTextInButton(newRoundText, newRoundButton);
    centerTextInButton(backText, backButton);
    centerTextInButton(statsText, statsButton);
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

std::string BlackjackUI::getCardFilename(const Card& card) const
{
    std::string rankStr;
    switch (card.getRank())
    {
    case Rank::Two: rankStr = "2"; break;
    case Rank::Three: rankStr = "3"; break;
    case Rank::Four: rankStr = "4"; break;
    case Rank::Five: rankStr = "5"; break;
    case Rank::Six: rankStr = "6"; break;
    case Rank::Seven: rankStr = "7"; break;
    case Rank::Eight: rankStr = "8"; break;
    case Rank::Nine: rankStr = "9"; break;
    case Rank::Ten: rankStr = "10"; break;
    case Rank::Jack: rankStr = "jack"; break;
    case Rank::Queen: rankStr = "queen"; break;
    case Rank::King: rankStr = "king"; break;
    case Rank::Ace: rankStr = "ace"; break;
    }

    std::string suitStr;
    switch (card.getSuit())
    {
    case Suit::Hearts: suitStr = "hearts"; break;
    case Suit::Diamonds: suitStr = "diamonds"; break;
    case Suit::Clubs: suitStr = "clubs"; break;
    case Suit::Spades: suitStr = "spades"; break;
    }

    return rankStr + "_of_" + suitStr + ".png";
}

void BlackjackUI::loadCardTextures()
{
    // Load standard 52 deck
    std::vector<Suit> suits = { Suit::Hearts, Suit::Diamonds, Suit::Clubs, Suit::Spades };
    std::vector<Rank> ranks = {
        Rank::Two, Rank::Three, Rank::Four, Rank::Five, Rank::Six, Rank::Seven,
        Rank::Eight, Rank::Nine, Rank::Ten, Rank::Jack, Rank::Queen, Rank::King, Rank::Ace
    };

    std::string basePath = "playing-cards-assets-master/png/";

    for (Suit s : suits)
    {
        for (Rank r : ranks)
        {
            Card c(r, s);
            std::string filename = getCardFilename(c);
            sf::Texture tex;
            if (tex.loadFromFile(basePath + filename))
            {
                cardTextures[filename] = std::move(tex);
            }
        }
    }

    sf::Texture backTex;
    if (backTex.loadFromFile(basePath + "back.png"))
    {
        cardTextures["back.png"] = std::move(backTex);
    }
}

void BlackjackUI::drawCard(sf::RenderWindow& window, const Card& card, float x, float y) const
{
    std::string filename = getCardFilename(card);
    
    // We use .count() to safely check if our map has the key to avoid std::out_of_range
    if (cardTextures.count(filename) > 0)
    {
        sf::RectangleShape cardBg({ 100.f, 145.f });
        cardBg.setPosition({ x, y });
        cardBg.setFillColor(sf::Color::White);
        window.draw(cardBg);

        sf::Sprite sprite(cardTextures.at(filename));
        sprite.setPosition({ x, y });
        // Scale to a standard 100x145 pixel playing card
        sprite.setScale({ 100.f / sprite.getTextureRect().size.x, 145.f / sprite.getTextureRect().size.y });
        window.draw(sprite);
    }
    else
    {
         sf::RectangleShape cardRect({ 100.f, 145.f });
         cardRect.setPosition({ x, y });
         cardRect.setFillColor(sf::Color::White);
         window.draw(cardRect);
    }
}

void BlackjackUI::drawHiddenCard(sf::RenderWindow& window, float x, float y) const
{
    if (cardTextures.count("back.png") > 0)
    {
        sf::RectangleShape cardBg({ 100.f, 145.f });
        cardBg.setPosition({ x, y });
        cardBg.setFillColor(sf::Color::White);
        window.draw(cardBg);

        sf::Sprite sprite(cardTextures.at("back.png"));
        sprite.setPosition({ x, y });
        sprite.setScale({ 100.f / sprite.getTextureRect().size.x, 145.f / sprite.getTextureRect().size.y });
        window.draw(sprite);
    }
    else
    {
        sf::RectangleShape cardRect({ 100.f, 145.f });
        cardRect.setPosition({ x, y });
        cardRect.setFillColor(sf::Color(140, 20, 20));
        window.draw(cardRect);
    }
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

    messageText.setString(getStatusMessage());

    auto messageBounds = messageText.getLocalBounds();
    messageText.setPosition({
        500.f - messageBounds.size.x / 2.f - messageBounds.position.x,
        550.f
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

void BlackjackUI::handleScreenClick(sf::Vector2f mousePos, bool& backToMenu, bool& openStats)
{
    backToMenu = false;
    openStats = false;

    if (backButton.getGlobalBounds().contains(mousePos))
    {
        backToMenu = true;
        return;
    }

    if (statsButton.getGlobalBounds().contains(mousePos))
    {
        openStats = true;
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

    window.draw(titleText);
    window.draw(bankrollText);
    window.draw(betText);

    if (roundStarted)
    {
        const auto& dealerHand = game.getDealerHand();
        sf::Text dLabel(font, "Dealer", 26);
        dLabel.setFillColor(sf::Color::Yellow);
        dLabel.setPosition({ 100.f, 150.f });
        window.draw(dLabel);

        float dX = 100.f;
        float dY = 190.f;
        for (size_t i = 0; i < dealerHand.getCards().size(); ++i)
        {
            if (i == 1 && !game.isRoundOver())
                drawHiddenCard(window, dX, dY);
            else
                drawCard(window, dealerHand.getCards()[i], dX, dY);
            dX += 115.f;
        }

        const auto& playerHand = game.getPlayerHand();
        sf::Text pLabel(font, "Player", 26);
        pLabel.setFillColor(sf::Color::Green);
        pLabel.setPosition({ 100.f, 360.f });
        window.draw(pLabel);

        float pX = 100.f;
        float pY = 400.f;
        for (const auto& card : playerHand.getCards())
        {
            drawCard(window, card, pX, pY);
            pX += 115.f;
        }
    }

    window.draw(messageText);

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

    statsButton.setFillColor(sf::Color(60, 180, 60));
    backButton.setFillColor(sf::Color(180, 65, 85));

    window.draw(minus10Button);
    window.draw(betInputBox);
    window.draw(plus10Button);

    window.draw(hitButton);
    window.draw(standButton);
    window.draw(doubleButton);
    window.draw(newRoundButton);
    window.draw(statsButton);
    window.draw(backButton);

    window.draw(minus10Text);
    refreshBetInputText(); /* Make sure it's drawn correct */
    window.draw(betInputBoxText);
    window.draw(plus10Text);

    window.draw(hitText);
    window.draw(standText);
    window.draw(doubleText);
    window.draw(newRoundText);
    window.draw(statsText);
    window.draw(backText);
}

std::string BlackjackUI::shortenResultText(const std::string& text) const
{
    // Search for the "Results:\n\tHand 1: " or just return a brief message based on outcome
    auto pos = text.find("\nResults:\n\tHand 1: ");
    if (pos != std::string::npos)
    {
        size_t start = pos + 19;
        size_t end = text.find("\n", start);
        return text.substr(start, end - start);
    }
    return "Round Over";
}

std::string BlackjackUI::getPostRoundStats() const
{
    return "";
}