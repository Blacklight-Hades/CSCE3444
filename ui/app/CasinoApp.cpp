#include "CasinoApp.h"
#include <optional>
#include <iostream>

CasinoApp::CasinoApp()
    : window(sf::VideoMode({ 1000, 760 }), "Space Casino"),
    currentState(AppState::MainMenu),
    bankrollInitialized(false)
{
    window.setFramerateLimit(60);

    if (!font.openFromFile("assets/Roboto.ttf"))
    {
        std::cout << "Failed to load assets/Roboto.ttf\n";
    }

    mainMenu = std::make_unique<MainMenu>(font);
    gameSelect = std::make_unique<GameSelect>(font);
    blackjackUI = std::make_unique<BlackjackUI>(font);
    slotsUI = std::make_unique<SlotsUI>(font);
    statsUI = std::make_unique<StatsUI>(font, nullptr);
}

void CasinoApp::run()
{
    while (window.isOpen())
    {
        processEvents();
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
            continue;
        }

        if (const auto* textEntered = event->getIf<sf::Event::TextEntered>())
        {
            if (currentState == AppState::MainMenu)
            {
                if (textEntered->unicode == 8)
                {
                    mainMenu->handleBackspace();
                }
                else
                {
                    mainMenu->handleTextEntered(textEntered->unicode);
                }
            }
            else if (currentState == AppState::GameSelect)
            {
                if (textEntered->unicode == 8)
                {
                    gameSelect->handleBackspace();
                }
                else
                {
                    gameSelect->handleTextEntered(textEntered->unicode);
                }
            }

            else if (currentState == AppState::Blackjack)
            {
                if (textEntered->unicode == 8)
                {
                    blackjackUI->handleBackspace();
                }
                else
                {
                    blackjackUI->handleTextEntered(textEntered->unicode);
                }
            }
            else if (currentState == AppState::Slots)
            {
                if (textEntered->unicode == 8)
                {
                    slotsUI->handleBackspace();
                }
                else
                {
                    slotsUI->handleTextEntered(textEntered->unicode);
                }
            }

            continue;
        }

        if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mousePressed->button != sf::Mouse::Button::Left)
            {
                continue;
            }

            sf::Vector2f mousePos = window.mapPixelToCoords(
                { mousePressed->position.x, mousePressed->position.y }
            );

            if (currentState == AppState::MainMenu)
            {
                bool startGame = false;
                bool openStats = false;
                bool exitGame = false;
                mainMenu->handleMouseClick(mousePos, startGame, openStats, exitGame);

                if (startGame)
                {
                    if (!bankrollInitialized)
                    {
                        sessionStats = std::make_unique<SessionStats>(mainMenu->getEnteredBankroll());
                        statsUI->setSessionStats(sessionStats.get());
                        blackjackUI->setSessionStats(sessionStats.get());
                        slotsUI->setSessionStats(sessionStats.get());
                        bankrollInitialized = true;
                    }

                    syncGameSelectBankroll();
                    syncBlackjackBankroll();
                    syncSlotsBankroll();

                    currentState = AppState::GameSelect;
                }
                else if (openStats)
                {
                    if (!bankrollInitialized)
                    {
                        // Open stats even if bankroll not set, just default $1000 and don't seal it as initialized
                        if (!sessionStats) {
                            sessionStats = std::make_unique<SessionStats>(1000.0);
                            statsUI->setSessionStats(sessionStats.get());
                            blackjackUI->setSessionStats(sessionStats.get());
                            slotsUI->setSessionStats(sessionStats.get());
                        }
                    }
                    statsUI->setReturnState(AppState::MainMenu);
                    statsUI->updateStats();
                    currentState = AppState::Stats;
                }
                else if (exitGame)
                {
                    window.close();
                }
            }
            else if (currentState == AppState::GameSelect)
            {
                bool openBlackjack = false;
                bool openSlots = false;
                bool backToMain = false;

                gameSelect->handleMouseClick(mousePos, openBlackjack, openSlots, backToMain);
                if (sessionStats) {
                    sessionStats->getBankroll().setBalance(gameSelect->getBankroll());
                }

                if (openBlackjack)
                {
                    syncBlackjackBankroll();
                    currentState = AppState::Blackjack;
                }
                else if (openSlots)
                {
                    syncSlotsBankroll();
                    currentState = AppState::Slots;
                }
                else if (backToMain)
                {
                    resetSessionIfNeeded();
                    currentState = AppState::MainMenu;
                }
            }
            else if (currentState == AppState::Blackjack)
            {
                bool backToMenu = false;
                bool openStats = false;
                blackjackUI->handleScreenClick(mousePos, backToMenu, openStats);

                if (backToMenu)
                {
                    if (sessionStats) {
                        sessionStats->getBankroll().setBalance(blackjackUI->getCurrentBankroll());
                    }
                    syncGameSelectBankroll();
                    syncSlotsBankroll();
                    currentState = AppState::GameSelect;
                }
                else if (openStats)
                {
                    statsUI->setReturnState(AppState::Blackjack);
                    statsUI->updateStats();
                    currentState = AppState::Stats;
                }
            }
            else if (currentState == AppState::Slots)
            {
                bool backToMenu = false;
                bool openStats = false;
                slotsUI->handleScreenClick(mousePos, backToMenu, openStats);

                if (backToMenu)
                {
                    if (sessionStats) {
                        sessionStats->getBankroll().setBalance(slotsUI->getCurrentBankroll());
                    }
                    syncGameSelectBankroll();
                    syncBlackjackBankroll();
                    currentState = AppState::GameSelect;
                }
                else if (openStats)
                {
                    statsUI->setReturnState(AppState::Slots);
                    statsUI->updateStats();
                    currentState = AppState::Stats;
                }
            }
            else if (currentState == AppState::Stats)
            {
                bool backToMenu = false;
                statsUI->handleScreenClick(mousePos, backToMenu);

                if (backToMenu)
                {
                    currentState = statsUI->returnState;
                }
            }
        }
    }
}

void CasinoApp::render()
{
    window.clear();

    if (currentState == AppState::MainMenu)
    {
        mainMenu->draw(window);
    }
    else if (currentState == AppState::GameSelect)
    {
        gameSelect->draw(window);
    }
    else if (currentState == AppState::Blackjack)
    {
        blackjackUI->draw(window);
    }
    else if (currentState == AppState::Slots)
    {
        slotsUI->draw(window);
    }
    else if (currentState == AppState::Stats)
    {
        statsUI->draw(window);
    }

    window.display();
}

void CasinoApp::syncGameSelectBankroll()
{
    if (sessionStats) gameSelect->setBankroll(sessionStats->getCurrentBalance());
}

void CasinoApp::syncBlackjackBankroll()
{
    if (sessionStats) blackjackUI->setStartingBankroll(sessionStats->getCurrentBalance());
}

void CasinoApp::syncSlotsBankroll()
{
    if (sessionStats) slotsUI->setStartingBankroll(sessionStats->getCurrentBalance());
}

void CasinoApp::resetSessionIfNeeded()
{
    if (sessionStats && sessionStats->getCurrentBalance() <= 0.0)
    {
        bankrollInitialized = false;
        mainMenu->resetSession();
    }
}
