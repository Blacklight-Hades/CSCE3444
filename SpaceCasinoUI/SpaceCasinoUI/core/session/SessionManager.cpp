#include "SessionManager.h"
#include "../blackjack/BlackjackGame.h"

#include <iostream>
#include <limits>
#include <vector>

using std::cin;
using std::cout;
using std::numeric_limits;
using std::streamsize;

SessionManager::SessionManager(double startingBankroll)
    : bankroll(startingBankroll),
      blackjackStats(startingBankroll),
      blackjackGame(nullptr),
      sessionActive(false),
      activeModule(ActiveModule::None)
{
}

SessionManager::~SessionManager() = default;

void SessionManager::startSession(double startingBankroll)
{
    bankroll = Bankroll(startingBankroll);
    blackjackStats = BlackjackStats(startingBankroll);
    blackjackGame.reset();

    sessionActive = true;
    activeModule = ActiveModule::None;
}

void SessionManager::endSession()
{
    cashOutBlackjackToSession();
    blackjackGame.reset();

    sessionActive = false;
    activeModule = ActiveModule::None;
}

bool SessionManager::isSessionActive() const
{
    return sessionActive;
}

double SessionManager::getCurrentBankroll() const
{
    return bankroll.getBalance();
}

SessionManager::ActiveModule SessionManager::getActiveModule() const
{
    return activeModule;
}

void SessionManager::setActiveModule(ActiveModule module)
{
    activeModule = module;
}

void SessionManager::returnToMainMenu()
{
    if (activeModule == ActiveModule::Blackjack)
    {
        cashOutBlackjackToSession();
    }

    activeModule = ActiveModule::None;
}

bool SessionManager::enterBlackjack()
{
    if (!sessionActive)
    {
        return false;
    }

    if (!blackjackGame)
    {
        blackjackGame = std::make_unique<BlackjackGame>(bankroll.getBalance(), 6);
    }
    else if (activeModule != ActiveModule::Blackjack)
    {
        blackjackGame->setTableBalance(bankroll.getBalance());
    }

    activeModule = ActiveModule::Blackjack;
    return true;
}

bool SessionManager::startBlackjackRound(double betAmount)
{
    if (!sessionActive)
    {
        return false;
    }

    if (activeModule != ActiveModule::Blackjack)
    {
        if (!enterBlackjack())
        {
            return false;
        }
    }

    if (!blackjackGame)
    {
        return false;
    }

    return blackjackGame->startNewRound(betAmount);
}

bool SessionManager::canBlackjackHit() const
{
    return blackjackGame && blackjackGame->canHit();
}

bool SessionManager::canBlackjackStand() const
{
    return blackjackGame && blackjackGame->canStand();
}

bool SessionManager::canBlackjackDoubleDown() const
{
    return blackjackGame && blackjackGame->canDoubleDown();
}

bool SessionManager::canBlackjackSplit() const
{
    return blackjackGame && blackjackGame->canSplit();
}

bool SessionManager::canBlackjackRequestHint() const
{
    return blackjackGame && blackjackGame->canRequestHint();
}

bool SessionManager::blackjackHit()
{
    if (!canBlackjackHit())
    {
        return false;
    }

    blackjackGame->playerHit();

    if (blackjackGame->isRoundOver())
    {
        finalizeBlackjackRound();
    }

    return true;
}

bool SessionManager::blackjackStand()
{
    if (!canBlackjackStand())
    {
        return false;
    }

    blackjackGame->playerStand();

    if (blackjackGame->isRoundOver())
    {
        finalizeBlackjackRound();
    }

    return true;
}

bool SessionManager::blackjackDoubleDown()
{
    if (!canBlackjackDoubleDown())
    {
        return false;
    }

    blackjackGame->playerDoubleDown();

    if (blackjackGame->isRoundOver())
    {
        finalizeBlackjackRound();
    }

    return true;
}

bool SessionManager::blackjackSplit()
{
    if (!canBlackjackSplit())
    {
        return false;
    }

    blackjackGame->playerSplit();

    if (blackjackGame->isRoundOver())
    {
        finalizeBlackjackRound();
    }

    return true;
}

bool SessionManager::blackjackRequestHint()
{
    if (!canBlackjackRequestHint())
    {
        return false;
    }

    blackjackGame->requestHint();
    return true;
}

bool SessionManager::isBlackjackLoaded() const
{
    return blackjackGame != nullptr;
}

bool SessionManager::isBlackjackRoundOver() const
{
    return blackjackGame && blackjackGame->isRoundOver();
}

BlackjackGame* SessionManager::getBlackjackGame()
{
    return blackjackGame.get();
}

const BlackjackGame* SessionManager::getBlackjackGame() const
{
    return blackjackGame.get();
}

BlackjackStats& SessionManager::getBlackjackStats()
{
    return blackjackStats;
}

const BlackjackStats& SessionManager::getBlackjackStats() const
{
    return blackjackStats;
}

void SessionManager::finalizeBlackjackRound()
{
    if (!blackjackGame || !blackjackGame->isRoundOver())
    {
        return;
    }

    blackjackStats.recordRound(blackjackGame->getRoundSummary());
}

bool SessionManager::prepareNextBlackjackRound()
{
    if (!blackjackGame || !blackjackGame->isRoundOver())
    {
        return false;
    }

    blackjackGame->resetForNextRound();
    return true;
}

bool SessionManager::enterRoulette()
{
    if (!sessionActive)
    {
        return false;
    }

    if (activeModule == ActiveModule::Blackjack)
    {
        cashOutBlackjackToSession();
    }

    activeModule = ActiveModule::Roulette;
    return true;
}

bool SessionManager::isRouletteAvailable() const
{
    return false;
}

bool SessionManager::enterSlots()
{
    if (!sessionActive)
    {
        return false;
    }

    if (activeModule == ActiveModule::Blackjack)
    {
        cashOutBlackjackToSession();
    }

    activeModule = ActiveModule::Slots;
    return true;
}

bool SessionManager::isSlotsAvailable() const
{
    return false;
}

void SessionManager::runTerminalSession()
{
    double startingBankroll = promptStartingBankroll();
    startSession(startingBankroll);

    while (sessionActive)
    {
        if (bankroll.isBroke())
        {
            cout << "\nYou're out of money! Session over.\n";
            break;
        }

        showTerminalMainMenu();
        int choice = promptMainMenuChoice();

        switch (choice)
        {
            case 1:
                runTerminalBlackjack();
                break;
            case 2:
                runTerminalSlotsPlaceholder();
                break;
            case 3:
                runTerminalRoulettePlaceholder();
                break;
            case 4:
                cout << "\nCurrent bankroll: $" << bankroll.getBalance() << "\n";
                break;
            case 5:
                endSession();
                break;
            default:
                cout << "Invalid choice.\n";
                break;
        }
    }
}

double SessionManager::promptStartingBankroll() const
{
    double amount = 0.0;

    while (true)
    {
        cout << "Enter your starting bankroll: $";
        cin >> amount;

        if (cin.fail() || amount <= 0.0)
        {
            cout << "Invalid amount. Try again.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return amount;
    }
}

int SessionManager::promptMainMenuChoice() const
{
    int choice = 0;

    while (true)
    {
        cout << "Choice: ";

        if (cin >> choice)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return choice;
        }

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter a number.\n";
    }
}

void SessionManager::showTerminalMainMenu() const
{
    cout << "\n============================\n";
    cout << "Balance: $" << bankroll.getBalance() << "\n";
    cout << "1. Blackjack\n";
    cout << "2. Slots\n";
    cout << "3. Roulette\n";
    cout << "4. Current Bankroll\n";
    cout << "5. Exit\n";
    cout << "============================\n";
}

void SessionManager::runTerminalBlackjack()
{
    if (!enterBlackjack())
    {
        cout << "\nUnable to enter Blackjack.\n";
        return;
    }

    cout << "\n=== Blackjack ===\n";

    bool keepPlaying = true;

    while (keepPlaying)
    {
        const BlackjackGame* game = getBlackjackGame();

        cout << "\nCurrent Table Balance: $" << game->getTableBalance() << "\n";

        if (game->getTableBalance() <= 0.0)
        {
            cout << "\nYou're out of money at the Blackjack table. Returning to main menu...\n";
            break;
        }

        double bet = 0.0;
        cout << "Enter bet amount: $";
        cin >> bet;

        if (cin.fail())
        {
            cout << "Invalid bet.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (!startBlackjackRound(bet))
        {
            cout << "Unable to start round.\n";
            continue;
        }

        while (!isBlackjackRoundOver())
        {
            game = getBlackjackGame();

            cout << "\nYour Hand: " << game->getPlayerHand().toString()
                 << " (" << game->getPlayerValue() << ")\n";

            cout << "Dealer Hand: ";
            if (game->isDealerHoleCardRevealed())
            {
                cout << game->getDealerHand().toString()
                    << " (" << game->getDealerValue() << ")\n";
            }
            else
            {
                vector<Card> dealerCards = game->getDealerHand().getCards();

                if (!dealerCards.empty())
                {
                    cout << dealerCards[0].toString() << " ??\n";
                }
                else
                {
                    cout << "\n";
                }
            }

            cout << "\nChoose action:\n";
            cout << "1. Hit\n";
            cout << "2. Stand\n";

            if (canBlackjackDoubleDown()) cout << "3. Double Down\n";
            if (canBlackjackSplit()) cout << "4. Split\n";
            if (canBlackjackRequestHint()) cout << "5. Hint\n";

            int choice = 0;
            cout << "Choice: ";
            cin >> choice;

            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid option.\n";
                continue;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (choice)
            {
                case 1:
                    if (!blackjackHit()) cout << "Hit is not available.\n";
                    break;
                case 2:
                    if (!blackjackStand()) cout << "Stand is not available.\n";
                    break;
                case 3:
                    if (!blackjackDoubleDown()) cout << "Double down is not available.\n";
                    break;
                case 4:
                    if (!blackjackSplit()) cout << "Split is not available.\n";
                    break;
                case 5:
                    if (blackjackRequestHint())
                    {
                        cout << getBlackjackGame()->getHintText() << "\n";
                    }
                    else
                    {
                        cout << "Hint is not available.\n";
                    }
                    break;
                default:
                    cout << "Invalid option.\n";
                    break;
            }
        }

        game = getBlackjackGame();

        cout << "\n" << game->getRoundResultText() << "\n";
        cout << "Updated Table Balance: $" << game->getTableBalance() << "\n";

        if (game->getTableBalance() <= 0.0)
        {
            cout << "\nYou're out of money! Returning to main menu...\n";
            break;
        }

        char again = 'n';
        cout << "\nPlay another round? (y/n): ";
        cin >> again;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (again != 'y' && again != 'Y')
        {
            keepPlaying = false;
        }
    }

    returnToMainMenu();
}

void SessionManager::runTerminalRoulettePlaceholder()
{
    if (enterRoulette())
    {
        cout << "\n[Roulette placeholder]\n";
    }

    returnToMainMenu();
}

void SessionManager::runTerminalSlotsPlaceholder()
{
    if (enterSlots())
    {
        cout << "\n[Slots placeholder]\n";
    }

    returnToMainMenu();
}

void SessionManager::cashOutBlackjackToSession()
{
    if (!blackjackGame)
    {
        return;
    }

    double newBalance = blackjackGame->cashOut();
    double delta = newBalance - bankroll.getBalance();
    bankroll.applyNetChange(delta);
}