#include "CasinoApp.h"
#include <optional>
#include <iostream>

CasinoApp::CasinoApp()
    : window(sf::VideoMode({ 1000, 760 }), "Space Casino"),
    currentState(AppState::MainMenu),
    sharedBankroll(1000.0),
    bankrollInitialized(false)
{
    window.setFramerateLimit(60);

    if (!font.openFromFile("ARIAL.TTF"))
    {
        std::cout << "Failed to load ARIAL.TTF\n";
    }

    mainMenu = std::make_unique<MainMenu>(font);
    gameSelect = std::make_unique<GameSelect>(font);
    blackjackUI = std::make_unique<BlackjackUI>(font);
    slotsUI = std::make_unique<SlotsUI>(font);
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
                bool exitGame = false;
                mainMenu->handleMouseClick(mousePos, startGame, exitGame);

                if (startGame)
                {
                    if (!bankrollInitialized)
                    {
                        sharedBankroll = mainMenu->getEnteredBankroll();
                        bankrollInitialized = true;
                    }

                    syncGameSelectBankroll();
                    syncBlackjackBankroll();
                    syncSlotsBankroll();

                    currentState = AppState::GameSelect;
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
                sharedBankroll = gameSelect->getBankroll();

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
                blackjackUI->handleScreenClick(mousePos, backToMenu);

                if (backToMenu)
                {
                    sharedBankroll = blackjackUI->getCurrentBankroll();
                    syncGameSelectBankroll();
                    syncSlotsBankroll();
                    currentState = AppState::GameSelect;
                }
            }
            else if (currentState == AppState::Slots)
            {
                bool backToMenu = false;
                slotsUI->handleScreenClick(mousePos, backToMenu);

                if (backToMenu)
                {
                    sharedBankroll = slotsUI->getCurrentBankroll();
                    syncGameSelectBankroll();
                    syncBlackjackBankroll();
                    currentState = AppState::GameSelect;
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

    window.display();
}

void CasinoApp::syncGameSelectBankroll()
{
    gameSelect->setBankroll(sharedBankroll);
}

void CasinoApp::syncBlackjackBankroll()
{
    blackjackUI->setStartingBankroll(sharedBankroll);
}

void CasinoApp::syncSlotsBankroll()
{
    slotsUI->setStartingBankroll(sharedBankroll);
}

void CasinoApp::resetSessionIfNeeded()
{
    if (sharedBankroll <= 0.0)
    {
        bankrollInitialized = false;
        mainMenu->resetSession();
    }
}