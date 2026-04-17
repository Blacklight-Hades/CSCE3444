#include "CasinoApp.h"
#include <optional>
#include <sstream>
#include <iomanip>
#include <iostream>

CasinoApp::CasinoApp()
    : window(sf::VideoMode({1000, 700}), "Space Casino"),
      session(1000.0),
      currentState(AppState::Welcome),
      pendingBankroll(0.0),
      startButton(nullptr),
      exitButton(nullptr),
      add5(nullptr),
      add25(nullptr),
      add100(nullptr),
      add500(nullptr),
      clearButton(nullptr),
      confirmButton(nullptr),
      blackjackButton(nullptr),
      slotsButton(nullptr),
      rouletteButton(nullptr),
      statsButton(nullptr),
      backButton(nullptr),
      blackjackScreen(nullptr)
{
    window.setFramerateLimit(60);

    if (!font.openFromFile("assets/Roboto.ttf"))
    {
        std::cout << "Failed to load font.\n";
    }

    setupButtons();
    blackjackScreen = new BlackjackScreen(font, session);
}

CasinoApp::~CasinoApp()
{
    delete startButton;
    delete exitButton;

    delete add5;
    delete add25;
    delete add100;
    delete add500;
    delete clearButton;
    delete confirmButton;

    delete blackjackButton;
    delete slotsButton;
    delete rouletteButton;
    delete statsButton;
    delete backButton;

    delete blackjackScreen;
}

void CasinoApp::setupButtons()
{
    startButton = new Button({220.f, 60.f}, {390.f, 300.f}, "Start Game", font);
    exitButton = new Button({220.f, 60.f}, {390.f, 380.f}, "Exit", font);

    add5 = new Button({120.f, 50.f}, {200.f, 300.f}, "+$5", font);
    add25 = new Button({120.f, 50.f}, {350.f, 300.f}, "+$25", font);
    add100 = new Button({120.f, 50.f}, {500.f, 300.f}, "+$100", font);
    add500 = new Button({120.f, 50.f}, {650.f, 300.f}, "+$500", font);

    clearButton = new Button({150.f, 50.f}, {300.f, 400.f}, "Clear", font);
    confirmButton = new Button({150.f, 50.f}, {550.f, 400.f}, "Confirm", font);

    blackjackButton = new Button({220.f, 60.f}, {390.f, 250.f}, "Blackjack", font);
    slotsButton = new Button({220.f, 60.f}, {390.f, 330.f}, "Slots", font);
    rouletteButton = new Button({220.f, 60.f}, {390.f, 410.f}, "Roulette", font);
    statsButton = new Button({220.f, 60.f}, {390.f, 490.f}, "Stats", font);
    backButton = new Button({150.f, 50.f}, {20.f, 620.f}, "Back", font);
}

void CasinoApp::run()
{
    while (window.isOpen())
    {
        processEvents();
        update();
        render();
    }
}

void CasinoApp::processEvents()
{
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }

        if (const auto* resized = event->getIf<sf::Event::Resized>())
        {
            window.setView(sf::View(sf::FloatRect(
                {0.f, 0.f},
                {
                    static_cast<float>(resized->size.x),
                    static_cast<float>(resized->size.y)
                }
            )));
        }

        switch (currentState)
        {
        case AppState::Welcome:
            if (startButton->isClicked(window, *event))
            {
                changeState(AppState::BankrollSetup);
            }

            if (exitButton->isClicked(window, *event))
            {
                window.close();
            }
            break;

        case AppState::BankrollSetup:
            if (add5->isClicked(window, *event))   pendingBankroll += 5.0;
            if (add25->isClicked(window, *event))  pendingBankroll += 25.0;
            if (add100->isClicked(window, *event)) pendingBankroll += 100.0;
            if (add500->isClicked(window, *event)) pendingBankroll += 500.0;

            if (clearButton->isClicked(window, *event))
            {
                pendingBankroll = 0.0;
            }

            if (confirmButton->isClicked(window, *event) && pendingBankroll > 0.0)
            {
                session.startSession(pendingBankroll);
                changeState(AppState::MainHub);
            }

            if (backButton->isClicked(window, *event))
            {
                changeState(AppState::Welcome);
            }
            break;

        case AppState::MainHub:
            if (blackjackButton->isClicked(window, *event))
            {
                if (session.enterBlackjack())
                {
                    blackjackScreen->resetForTableEntry();
                    changeState(AppState::Blackjack);
                }
            }

            if (slotsButton->isClicked(window, *event))
            {
                changeState(AppState::Slots);
            }

            if (rouletteButton->isClicked(window, *event))
            {
                changeState(AppState::Roulette);
            }

            if (statsButton->isClicked(window, *event))
            {
                changeState(AppState::Stats);
            }

            if (backButton->isClicked(window, *event))
            {
                changeState(AppState::Welcome);
            }
            break;

        case AppState::Blackjack:
        {
            BlackjackScreen::Action action = blackjackScreen->handleEvent(window, *event);

            if (action == BlackjackScreen::Action::ReturnToHub)
            {
                session.returnToMainMenu();
                changeState(AppState::MainHub);
            }
            break;
        }

        case AppState::Slots:
        case AppState::Roulette:
        case AppState::Stats:
            if (backButton->isClicked(window, *event))
            {
                changeState(AppState::MainHub);
            }
            break;

        default:
            break;
        }
    }
}

void CasinoApp::update()
{
    switch (currentState)
    {
    case AppState::Welcome:
        startButton->update(window);
        exitButton->update(window);
        break;

    case AppState::BankrollSetup:
        add5->update(window);
        add25->update(window);
        add100->update(window);
        add500->update(window);
        clearButton->update(window);
        confirmButton->update(window);
        backButton->update(window);
        break;

    case AppState::MainHub:
        blackjackButton->update(window);
        slotsButton->update(window);
        rouletteButton->update(window);
        statsButton->update(window);
        backButton->update(window);
        break;

    case AppState::Slots:
    case AppState::Roulette:
    case AppState::Stats:
        backButton->update(window);
        break;

    case AppState::Blackjack:
        blackjackScreen->update(window);
        break;

    default:
        break;
    }
}

void CasinoApp::render()
{
    window.clear(sf::Color(10, 10, 30));

    switch (currentState)
    {
    case AppState::Welcome:
        renderWelcome();
        break;

    case AppState::BankrollSetup:
        renderBankrollSetup();
        break;

    case AppState::MainHub:
        renderMainHub();
        break;

    case AppState::Blackjack:
        blackjackScreen->render(window);
        break;

    case AppState::Slots:
        renderCenteredText("Slots: GUI Under Construction", 300.f, 36);
        backButton->render(window);
        break;

    case AppState::Roulette:
        renderCenteredText("Roulette: GUI Under Construction", 300.f, 36);
        backButton->render(window);
        break;

    case AppState::Stats:
    {
        renderCenteredText("Session Stats", 50.f, 36);

        // --- Blackjack Stats ---
        const auto& bj = session.getBlackjackStats();
        if (bj.getTotalRounds() > 0) {
            int bjDecided = bj.getTotalWins() + bj.getTotalLosses();
            double bjWinPct = (bjDecided > 0) ? (double)bj.getTotalWins() / bjDecided * 100.0 : 0.0;
            std::ostringstream bjLine1, bjLine2, bjLine3;
            bjLine1 << "Blackjack: " << bj.getTotalRounds() << " rounds | "
                     << bj.getTotalWins() << "W / " << bj.getTotalLosses() << "L | Win: "
                     << std::fixed << std::setprecision(1) << bjWinPct << "%";
            bjLine2 << "Blackjacks: " << bj.getTotalBlackjacks()
                     << " | Best Win: $" << std::fixed << std::setprecision(2) << bj.getBiggestWin()
                     << " | Worst Loss: $" << std::fixed << std::setprecision(2) << bj.getBiggestLoss();
            bjLine3 << "Net: $" << std::fixed << std::setprecision(2) << bj.getNetProfit();
            renderCenteredText(bjLine1.str(), 120.f, 18);
            renderCenteredText(bjLine2.str(), 145.f, 16);
            renderCenteredText(bjLine3.str(), 168.f, 16);
        } else {
            renderCenteredText("Blackjack: No rounds played yet", 140.f, 18);
        }

        // --- Slots Stats ---
        if (session.getSlotsGame() || true) { // always show section
            std::ostringstream slLine1, slLine2, slLine3;
            // Access stats via session manager (they are tracked internally)
            // For now display what we can from the session
            renderCenteredText("Slots & Roulette: Full stats in Terminal mode (option 2)", 220.f, 16);
        }

        // --- Bankroll ---
        std::ostringstream bankLine;
        bankLine << "Current Bankroll: $" << std::fixed << std::setprecision(2)
                 << session.getCurrentBankroll();
        renderCenteredText(bankLine.str(), 300.f, 26);

        backButton->render(window);
        break;
    }

    default:
        break;
    }

    window.display();
}

void CasinoApp::renderCenteredText(const std::string& str, float y, unsigned int size)
{
    sf::Text text(font, str);
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

void CasinoApp::renderWelcome()
{
    renderCenteredText("Space Casino", 150.f, 40);
    startButton->render(window);
    exitButton->render(window);
}

void CasinoApp::renderBankrollSetup()
{
    renderCenteredText("Set Your Bankroll", 120.f, 36);

    std::ostringstream ss;
    ss << "$" << std::fixed << std::setprecision(2) << pendingBankroll;

    renderCenteredText("Selected: " + ss.str(), 200.f, 28);

    add5->render(window);
    add25->render(window);
    add100->render(window);
    add500->render(window);

    clearButton->render(window);
    confirmButton->render(window);
    backButton->render(window);
}

void CasinoApp::renderMainHub()
{
    renderCenteredText("Main Hub", 100.f, 36);

    std::ostringstream ss;
    ss << "$" << std::fixed << std::setprecision(2)
       << session.getCurrentBankroll();

    renderCenteredText("Bankroll: " + ss.str(), 160.f, 26);

    blackjackButton->render(window);
    slotsButton->render(window);
    rouletteButton->render(window);
    statsButton->render(window);
    backButton->render(window);
}

void CasinoApp::changeState(AppState newState)
{
    currentState = newState;
}