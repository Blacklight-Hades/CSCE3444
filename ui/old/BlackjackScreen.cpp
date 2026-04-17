#include "BlackjackScreen.h"
#include "../games/blackjack/BlackjackGame.h"

#include <sstream>
#include <iomanip>
#include <vector>

BlackjackScreen::BlackjackScreen(sf::Font& appFont, SessionManager& sessionManager)
    : font(appFont),
      session(sessionManager),
      pendingBet(0.0),
      roundText(""),
      add5Button(nullptr),
      add25Button(nullptr),
      add100Button(nullptr),
      clearBetButton(nullptr),
      dealButton(nullptr),
      hitButton(nullptr),
      standButton(nullptr),
      doubleButton(nullptr),
      splitButton(nullptr),
      hintButton(nullptr),
      nextHandButton(nullptr),
      backToHubButton(nullptr)
{
    setupButtons();
}

BlackjackScreen::~BlackjackScreen()
{
    delete add5Button;
    delete add25Button;
    delete add100Button;
    delete clearBetButton;
    delete dealButton;
    delete hitButton;
    delete standButton;
    delete doubleButton;
    delete splitButton;
    delete hintButton;
    delete nextHandButton;
    delete backToHubButton;
}

void BlackjackScreen::setupButtons()
{
    add5Button = new Button({110.f, 48.f}, {80.f, 140.f}, "+$5", font);
    add25Button = new Button({110.f, 48.f}, {210.f, 140.f}, "+$25", font);
    add100Button = new Button({110.f, 48.f}, {340.f, 140.f}, "+$100", font);

    clearBetButton = new Button({130.f, 48.f}, {490.f, 140.f}, "Clear Bet", font);
    dealButton = new Button({130.f, 48.f}, {640.f, 140.f}, "Deal", font);

    hitButton = new Button({130.f, 52.f}, {760.f, 350.f}, "Hit", font);
    standButton = new Button({130.f, 52.f}, {760.f, 420.f}, "Stand", font);
    doubleButton = new Button({130.f, 52.f}, {760.f, 490.f}, "Double", font);
    splitButton = new Button({130.f, 52.f}, {760.f, 560.f}, "Split", font);
    hintButton = new Button({130.f, 52.f}, {760.f, 630.f}, "Hint", font);

    nextHandButton = new Button({180.f, 50.f}, {730.f, 560.f}, "Next Hand", font);
    backToHubButton = new Button({170.f, 50.f}, {20.f, 620.f}, "Back to Hub", font);
}

void BlackjackScreen::resetForTableEntry()
{
    pendingBet = 0.0;
    roundText.clear();
    hintText.clear();
}

void BlackjackScreen::update(const sf::RenderWindow& window)
{
    BlackjackGame* game = session.getBlackjackGame();

    bool roundActive = game && !game->isRoundOver() &&
                       game->getRoundState() == RoundState::PlayerTurn;

    backToHubButton->update(window);

    if (roundActive)
    {
        hitButton->update(window);
        standButton->update(window);
        doubleButton->update(window);
        splitButton->update(window);
        hintButton->update(window);
    }
    else if (game && game->isRoundOver())
    {
        nextHandButton->update(window);
    }
    else
    {
        add5Button->update(window);
        add25Button->update(window);
        add100Button->update(window);
        clearBetButton->update(window);
        dealButton->update(window);
    }
}

BlackjackScreen::Action BlackjackScreen::handleEvent(const sf::RenderWindow& window, const sf::Event& event)
{
    if (backToHubButton->isClicked(window, event))
    {
        return Action::ReturnToHub;
    }

    BlackjackGame* game = session.getBlackjackGame();

    bool roundActive = game && !game->isRoundOver() &&
                       game->getRoundState() == RoundState::PlayerTurn;

    // Handle player actions during an active round
    if (roundActive)
    {
        if (hitButton->isClicked(window, event))
        {
            session.blackjackHit();
            updateRoundTextFromGame();
        }

        if (standButton->isClicked(window, event))
        {
            session.blackjackStand();
            updateRoundTextFromGame();
        }

        if (doubleButton->isClicked(window, event) && session.canBlackjackDoubleDown())
        {
            session.blackjackDoubleDown();
            updateRoundTextFromGame();
        }

        if (splitButton->isClicked(window, event) && session.canBlackjackSplit())
        {
            session.blackjackSplit();
            updateRoundTextFromGame();
        }
        
        if (hintButton->isClicked(window, event))
        {
            if (session.blackjackRequestHint())
            {
                hintText = session.getBlackjackGame()->getHintText();
            }
            else
            {
                hintText = "Hint unavailable.";
            }
        }
    }
    // Handle post-round actions
    else if (game && game->isRoundOver())
    {
        if (nextHandButton->isClicked(window, event))
        {
            if (session.prepareNextBlackjackRound())
            {
                resetForTableEntry();
                hintText.clear();
            }
        }
    }
    // Handle betting phase
    else
    {
        if (add5Button->isClicked(window, event))
        {
            pendingBet += 5.0;
        }

        if (add25Button->isClicked(window, event))
        {
            pendingBet += 25.0;
        }

        if (add100Button->isClicked(window, event))
        {
            pendingBet += 100.0;
        }

        if (clearBetButton->isClicked(window, event))
        {
            pendingBet = 0.0;
        }

        if (dealButton->isClicked(window, event))
        {
            if (pendingBet > 0.0 && session.startBlackjackRound(pendingBet))
            {
                pendingBet = 0.0;
                updateRoundTextFromGame();
            }
        }
    }

    return Action::None;
}

void BlackjackScreen::render(sf::RenderWindow& window)
{
    renderCenteredText(window, "Blackjack", 45.f, 36);

    std::ostringstream balanceStream;
    balanceStream << "$" << std::fixed << std::setprecision(2) << getDisplayedTableBalance();
    renderLeftText(window, "Table Balance: " + balanceStream.str(), 40.f, 85.f, 24);

    std::ostringstream betStream;
    betStream << "$" << std::fixed << std::setprecision(2) << pendingBet;
    renderLeftText(window, "Pending Bet: " + betStream.str(), 40.f, 115.f, 22);

    BlackjackGame* game = session.getBlackjackGame();
    bool roundActive = game && !game->isRoundOver() &&
                       game->getRoundState() == RoundState::PlayerTurn;

    if (roundActive)
    {
        hitButton->render(window);
        standButton->render(window);

        if (session.canBlackjackDoubleDown())
        {
            doubleButton->render(window);
        }

        if (session.canBlackjackSplit())
        {
            splitButton->render(window);
        }
        
        hintButton->render(window);
    }
    else if (game && game->isRoundOver())
    {
        nextHandButton->render(window);
    }
    else
    {
        add5Button->render(window);
        add25Button->render(window);
        add100Button->render(window);
        clearBetButton->render(window);
        dealButton->render(window);
    }

    backToHubButton->render(window);

    float y = 200.f;

    renderLeftText(window, buildDealerDisplay(), 40.f, y, 22);
    y += 80.f;

    renderLeftText(window, buildPlayerHandsDisplay(), 40.f, y, 22);
    y += 120.f;

    if (!roundText.empty())
    {
        renderLeftText(window, "Results", 40.f, y, 20);
        y += 35.f;

        renderLeftText(window, roundText, 40.f, y, 18);
    }

    if (!hintText.empty() && roundActive)
    {
        renderLeftText(window, "Hint", 40.f, 500.f, 20);
        renderLeftText(window, hintText, 40.f, 530.f, 18);
    }
}

void BlackjackScreen::renderCenteredText(sf::RenderWindow& window, const std::string& textString, float y, unsigned int size)
{
    sf::Text text(font, textString);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color::White);

    sf::FloatRect bounds = text.getLocalBounds();

    text.setOrigin({
        bounds.position.x + bounds.size.x / 2.0f,
        bounds.position.y + bounds.size.y / 2.0f
    });

    text.setPosition({
        window.getSize().x / 2.0f,
        y
    });

    window.draw(text);
}

void BlackjackScreen::renderLeftText(sf::RenderWindow& window, const std::string& textString, float x, float y, unsigned int size)
{
    sf::Text text(font, textString);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color::White);
    text.setPosition({x, y});
    window.draw(text);
}

void BlackjackScreen::updateRoundTextFromGame()
{
    BlackjackGame* game = session.getBlackjackGame();

    if (!game)
    {
        roundText.clear();
        return;
    }

    if (!game->isRoundOver())
    {
        roundText.clear();
        return;
    }

    std::string fullText = game->getRoundResultText();

    size_t resultsPos = fullText.find("Results:");
    size_t insightPos = fullText.find("Round Insight:");
    size_t dividerPos = fullText.find("----------------------------------------");

    std::string cleaned;

    if (resultsPos != std::string::npos)
    {
        size_t resultsContentStart = resultsPos + std::string("Results:").length();

        size_t resultsContentEnd = std::string::npos;
        if (insightPos != std::string::npos && insightPos > resultsContentStart)
        {
            resultsContentEnd = insightPos;
        }
        else if (dividerPos != std::string::npos && dividerPos > resultsContentStart)
        {
            resultsContentEnd = dividerPos;
        }

        cleaned += fullText.substr(resultsContentStart, resultsContentEnd - resultsContentStart);
    }

    if (insightPos != std::string::npos)
    {
        size_t insightContentStart = insightPos;
        size_t insightContentEnd = dividerPos;

        if (insightContentEnd != std::string::npos && insightContentEnd > insightContentStart)
        {
            cleaned += "\n";
            cleaned += fullText.substr(insightContentStart, insightContentEnd - insightContentStart);
        }
        else
        {
            cleaned += "\n";
            cleaned += fullText.substr(insightContentStart);
        }
    }

    for (char& ch : cleaned)
    {
        if (ch == '\t')
        {
            ch = ' ';
        }
    }

    roundText = cleaned;
}

std::string BlackjackScreen::buildDealerDisplay() const
{
    const BlackjackGame* game = session.getBlackjackGame();

    if (!game)
    {
        return "Dealer: No round started yet.";
    }

    Hand dealerHand = game->getDealerHand();
    std::vector<Card> cards = dealerHand.getCards();

    if (cards.empty())
    {
        return "Dealer: No cards yet.";
    }

    std::string result = "Dealer:\n    ";

    if (game->isDealerHoleCardRevealed())
    {
        result += dealerHand.toString();
        result += " (" + std::to_string(dealerHand.getValue()) + ")";
    }
    else
    {
        result += cards[0].toString();

        if (cards.size() > 1)
        {
            result += " ??";
        }
    }

    return result;
}

std::string BlackjackScreen::buildPlayerHandsDisplay() const
{
    const BlackjackGame* game = session.getBlackjackGame();

    if (!game)
    {
        return "Player: No round started yet.";
    }

    std::string result = "Player Hands:\n";

    for (int i = 0; i < game->getHandCount(); ++i)
    {
        Hand hand = game->getPlayerHand(i);

        result += "    Hand " + std::to_string(i + 1) + ": ";
        result += hand.toString();
        result += " (" + std::to_string(hand.getValue()) + ")";

        std::ostringstream betStream;
        betStream << std::fixed << std::setprecision(2) << game->getPlayerHandBet(i);
        result += "  Bet: $" + betStream.str();

        if (i == game->getActiveHandIndex() && !game->isRoundOver())
        {
            result += " <-- Active";
        }

        if (i < game->getHandCount() - 1)
        {
            result += "\n";
        }
    }

    return result;
}

double BlackjackScreen::getDisplayedTableBalance() const
{
    const BlackjackGame* game = session.getBlackjackGame();

    if (game)
    {
        return game->getTableBalance();
    }

    return session.getCurrentBankroll();
}