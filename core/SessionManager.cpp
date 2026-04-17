#include "SessionManager.h"
#include "../games/blackjack/BlackjackGame.h"

#include <iostream>
#include <limits>
#include <vector>
#include <iomanip>
#include <sstream>

using std::cin;
using std::cout;
using std::numeric_limits;
using std::streamsize;

SessionManager::SessionManager(double startingBankroll)
    : bankroll(startingBankroll),
      blackjackStats(startingBankroll),
      blackjackGame(nullptr),
      rouletteGame(nullptr),
      slotsGame(nullptr),
      sessionActive(false),
      activeModule(ActiveModule::None),
      slotsStats(startingBankroll),
      rouletteStats(startingBankroll)
{
}

SessionManager::~SessionManager() = default;

void SessionManager::startSession(double startingBankroll)
{
    bankroll = Bankroll(startingBankroll);
    blackjackStats = BlackjackStats(startingBankroll);
    blackjackGame.reset();
    rouletteGame.reset();
    slotsGame.reset();

    // Reset stats counters
    slotsStats = SlotsStats(startingBankroll);
    rouletteStats = RouletteStats(startingBankroll);

    sessionActive = true;
    activeModule = ActiveModule::None;
}

void SessionManager::endSession()
{
    cashOutBlackjackToSession();
    cashOutRouletteToSession();
    cashOutSlotsToSession();
    blackjackGame.reset();
    rouletteGame.reset();
    slotsGame.reset();

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
    else if (activeModule == ActiveModule::Roulette)
    {
        cashOutRouletteToSession();
    }
    else if (activeModule == ActiveModule::Slots)
    {
        cashOutSlotsToSession();
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
    if (!sessionActive) return false;
    if (activeModule == ActiveModule::Blackjack) cashOutBlackjackToSession();
    else if (activeModule == ActiveModule::Slots) cashOutSlotsToSession();

    if (!rouletteGame || activeModule != ActiveModule::Roulette)
    {
        rouletteGame = std::make_unique<RouletteGame>(static_cast<int>(bankroll.getBalance()));
    }

    activeModule = ActiveModule::Roulette;
    return true;
}

bool SessionManager::isRouletteAvailable() const { return true; }
RouletteGame* SessionManager::getRouletteGame() { return rouletteGame.get(); }
const RouletteGame* SessionManager::getRouletteGame() const { return rouletteGame.get(); }

bool SessionManager::enterSlots()
{
    if (!sessionActive) return false;
    if (activeModule == ActiveModule::Blackjack) cashOutBlackjackToSession();
    else if (activeModule == ActiveModule::Roulette) cashOutRouletteToSession();

    if (!slotsGame)
    {
        slotsGame = std::make_unique<Slots>(bankroll.getBalance());
    }
    else if (activeModule != ActiveModule::Slots)
    {
        slotsGame->setBankroll(bankroll.getBalance());
    }

    activeModule = ActiveModule::Slots;
    return true;
}

bool SessionManager::isSlotsAvailable() const { return true; }
Slots* SessionManager::getSlotsGame() { return slotsGame.get(); }
const Slots* SessionManager::getSlotsGame() const { return slotsGame.get(); }

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
                // --- Terminal-only: START --- Stats menu
                displayTerminalStatsMenu();
                // --- Terminal-only: END ---
                break;
            case 6:
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
    cout << "5. Check Stats\n";
    cout << "6. Exit\n";
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

        // --- Terminal-only: START --- Offer stats check before new hand
        char viewStats = 'n';
        cout << "\nView Blackjack stats before playing? (y/n): ";
        cin >> viewStats;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (viewStats == 'y' || viewStats == 'Y')
        {
            displayTerminalBlackjackStats();
        }
        // --- Terminal-only: END ---

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
    if (!enterRoulette())
    {
        cout << "\nUnable to enter Roulette.\n";
        return;
    }

    cout << "\n=== Roulette ===\n";

    bool keepPlaying = true;
    while (keepPlaying)
    {
        cout << "\nCurrent Table Balance: $" << getRouletteGame()->getBalance() << "\n";
        if (getRouletteGame()->getBalance() <= 0)
        {
            cout << "\nYou're out of money at the Roulette table. Returning to main menu...\n";
            break;
        }

        int betAmount = 0;
        cout << "Enter bet amount: $";
        cin >> betAmount;
        if (cin.fail() || betAmount <= 0 || betAmount > getRouletteGame()->getBalance())
        {
            cout << "Invalid bet.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string typeStr;
        cout << "Enter bet type (Straight, Red, Black, Even, High): ";
        cin >> typeStr;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (typeStr == "Straight") {
            int num = 0;
            cout << "Enter number (0-36): ";
            cin >> num;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            RouletteBet bet(BetType::StraightUp, num, betAmount);
            getRouletteGame()->placeBet(bet);
        } else if (typeStr == "Red") {
            getRouletteGame()->placeBet(RouletteBet(Color::Red, betAmount));
        } else if (typeStr == "Black") {
            getRouletteGame()->placeBet(RouletteBet(Color::Black, betAmount));
        } else if (typeStr == "Even") {
            getRouletteGame()->placeBet(RouletteBet(BetType::EvenOdd, 0, betAmount));
        } else if (typeStr == "High") {
            getRouletteGame()->placeBet(RouletteBet(BetType::HighLow, 1, betAmount));
        } else {
            cout << "Invalid bet type. Defaulting to StraightUp on 0.\n";
            getRouletteGame()->placeBet(RouletteBet(BetType::StraightUp, 0, betAmount));
        }

        // Track roulette stats
        int balanceBefore = getRouletteGame()->getBalance();
        bool wasStraightUp = (typeStr == "Straight");
        getRouletteGame()->spin();
        getRouletteGame()->resolve();
        auto result = getRouletteGame()->getLastResult();
        int balanceAfter = getRouletteGame()->getBalance();
        double netChange = static_cast<double>(balanceAfter - balanceBefore);
        double payoutAmt = (netChange > 0) ? netChange + betAmount : 0.0;
        RouletteRoundSummary rSummary;
        rSummary.betAmount     = betAmount;
        rSummary.payoutAmount  = payoutAmt;
        rSummary.netChange     = netChange;
        rSummary.wasStraightUp = wasStraightUp;
        rSummary.straightUpWon = (wasStraightUp && netChange > 0);
        rouletteStats.recordRound(rSummary);
        cout << "\n" << result.toString() << "\n";
        cout << "Updated Table Balance: $" << getRouletteGame()->getBalance() << "\n";

        if (getRouletteGame()->getBalance() <= 0)
        {
            cout << "\nYou're out of money! Returning to main menu...\n";
            break;
        }

        char again = 'n';
        // --- Terminal-only: START --- Offer stats check before next roulette round
        char viewStats = 'n';
        cout << "\nView Roulette stats? (y/n): ";
        cin >> viewStats;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (viewStats == 'y' || viewStats == 'Y')
        {
            displayTerminalRouletteStats();
        }
        // --- Terminal-only: END ---
        cout << "\nPlay another round? (y/n): ";
        cin >> again;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (again != 'y' && again != 'Y') keepPlaying = false;
    }

    returnToMainMenu();
}

void SessionManager::runTerminalSlotsPlaceholder()
{
    if (!enterSlots())
    {
        cout << "\nUnable to enter Slots.\n";
        return;
    }

    cout << "\n=== Slots ===\n";

    bool keepPlaying = true;
    while (keepPlaying)
    {
        cout << "\nCurrent Table Balance: $" << getSlotsGame()->getBankroll() << "\n";
        if (getSlotsGame()->getBankroll() <= 0)
        {
            cout << "\nYou're out of money at the Slots table. Returning to main menu...\n";
            break;
        }

        double bet = 0.0;
        cout << "Enter bet amount: $";
        cin >> bet;
        if (cin.fail() || bet <= 0.0 || bet > getSlotsGame()->getBankroll())
        {
            cout << "Invalid bet.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        SlotWindow slotw = getSlotsGame()->reelsSpin(bet);
        cout << "Results: \n"; 
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                cout << slotw.getDisplay(i,j) << "  ";
            }
            cout << "\n";
        }

        // Track slots stats
        double payout = getSlotsGame()->paytable();
        double slotsNetThisRound = payout - bet;
        // Check for special events
        bool wasThreeInARow = false;
        bool wasJackpot = false;
        if (payout > 0) {
            char m0 = slotw.getDisplay(0, 1);
            char m1 = slotw.getDisplay(1, 1);
            char m2 = slotw.getDisplay(2, 1);
            if (m0 == m1 && m1 == m2) {
                wasThreeInARow = true;
                if (m0 == 'B' || m0 == 'V') wasJackpot = true;
            }
        }
        SlotsRoundSummary sSummary;
        sSummary.betAmount      = bet;
        sSummary.payoutAmount   = payout;
        sSummary.netChange      = slotsNetThisRound;
        sSummary.wasThreeInARow = wasThreeInARow;
        sSummary.wasJackpot     = wasJackpot;
        slotsStats.recordRound(sSummary);
        cout << "Your payout is: $" << payout << "\n";
        cout << "Updated Table Balance: $" << getSlotsGame()->getBankroll() << "\n";

        if (getSlotsGame()->getBankroll() <= 0.0)
        {
            cout << "\nYou're out of money! Returning to main menu...\n";
            break;
        }

        char again = 'n';
        // --- Terminal-only: START --- Offer stats check before next slots round
        char viewStats = 'n';
        cout << "\nView Slots stats? (y/n): ";
        cin >> viewStats;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (viewStats == 'y' || viewStats == 'Y')
        {
            displayTerminalSlotsStats();
        }
        // --- Terminal-only: END ---
        cout << "\nPlay another round? (y/n): ";
        cin >> again;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (again != 'y' && again != 'Y') keepPlaying = false;
    }

    returnToMainMenu();
}

void SessionManager::cashOutRouletteToSession()
{
    if (!rouletteGame) return;
    double newBalance = static_cast<double>(rouletteGame->getBalance());
    double delta = newBalance - bankroll.getBalance();
    bankroll.applyNetChange(delta);
    rouletteGame.reset();
}

void SessionManager::cashOutSlotsToSession()
{
    if (!slotsGame) return;
    double newBalance = slotsGame->getBankroll();
    double delta = newBalance - bankroll.getBalance();
    bankroll.applyNetChange(delta);
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

// =====================================================
// --- Terminal-only: START --- Stats display methods
// =====================================================

void SessionManager::displayTerminalStatsMenu()
{
    cout << "\n====== STATS MENU ======\n";
    cout << "1. Blackjack Stats\n";
    cout << "2. Slots Stats\n";
    cout << "3. Roulette Stats\n";
    cout << "4. Overall Session Summary\n";
    cout << "5. Back to Main Menu\n";
    cout << "========================\n";
    cout << "Choice: ";

    int choice = 0;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (choice)
    {
        case 1: displayTerminalBlackjackStats(); break;
        case 2: displayTerminalSlotsStats(); break;
        case 3: displayTerminalRouletteStats(); break;
        case 4: displayTerminalSessionSummary(); break;
        case 5: break;
        default: cout << "Invalid choice.\n"; break;
    }
}

void SessionManager::displayTerminalBlackjackStats() const
{
    blackjackStats.displayStats();
}

void SessionManager::displayTerminalSlotsStats() const
{
    slotsStats.displayStats();
}

void SessionManager::displayTerminalRouletteStats() const
{
    rouletteStats.displayStats();
}

void SessionManager::displayTerminalSessionSummary() const
{
    const int W = 52;
    cout << "\n";
    cout << std::string(W, '=') << "\n";
    cout << "   * GALACTIC CASINO -- SESSION SUMMARY *\n";
    cout << std::string(W, '=') << "\n";

    auto printRow = [&](const std::string& label, const std::string& value) {
        int gap = W - 2 - (int)label.size() - (int)value.size();
        if (gap < 1) gap = 1;
        cout << "  " << label << std::string(gap, '.') << value << "\n";
    };

    auto fmtMoney = [](double amt) -> std::string {
        std::ostringstream out;
        if (amt >= 0) out << "+$" << std::fixed << std::setprecision(2) << amt;
        else out << "-$" << std::fixed << std::setprecision(2) << std::abs(amt);
        return out.str();
    };

    auto fmtPct = [](int num, int den) -> std::string {
        if (den <= 0) return "0.0%";
        std::ostringstream out;
        out << std::fixed << std::setprecision(1) << ((double)num / den * 100.0) << "%";
        return out.str();
    };

    cout << " BANKROLL\n";
    cout << std::string(W, '-') << "\n";
    printRow("Current Balance", fmtMoney(bankroll.getBalance()));

    cout << "\n GAMES PLAYED\n";
    cout << std::string(W, '-') << "\n";

    int totalRounds = blackjackStats.getTotalRounds()
                    + slotsStats.getTotalRounds()
                    + rouletteStats.getTotalRounds();
    printRow("Total Rounds (all games)", std::to_string(totalRounds));

    if (blackjackStats.getTotalRounds() > 0)
    {
        printRow("  Blackjack Rounds", std::to_string(blackjackStats.getTotalRounds()));
        printRow("  Blackjack Wins", std::to_string(blackjackStats.getTotalWins()));
        printRow("  Blackjack Losses", std::to_string(blackjackStats.getTotalLosses()));
        printRow("  Blackjack Win %", fmtPct(blackjackStats.getTotalWins(), blackjackStats.getTotalWins() + blackjackStats.getTotalLosses()));
        printRow("  Blackjack Blackjacks", std::to_string(blackjackStats.getTotalBlackjacks()));
        printRow("  Blackjack Biggest Win", fmtMoney(blackjackStats.getBiggestWin()));
        printRow("  Blackjack Biggest Loss", fmtMoney(blackjackStats.getBiggestLoss()));
        printRow("  Blackjack Net", fmtMoney(blackjackStats.getNetProfit()));
    }

    if (slotsStats.getTotalRounds() > 0)
    {
        printRow("  Slots Rounds", std::to_string(slotsStats.getTotalRounds()));
        printRow("  Slots Wins", std::to_string(slotsStats.getTotalWins()));
        printRow("  Slots Losses", std::to_string(slotsStats.getTotalLosses()));
        printRow("  Slots Win %", fmtPct(slotsStats.getTotalWins(), slotsStats.getTotalRounds()));
        printRow("  Slots Jackpots", std::to_string(slotsStats.getJackpotHits()));
        printRow("  Slots Biggest Win", fmtMoney(slotsStats.getBiggestWin()));
        printRow("  Slots Biggest Loss", fmtMoney(slotsStats.getBiggestLoss()));
        printRow("  Slots Net", fmtMoney(slotsStats.getNetProfit()));
    }

    if (rouletteStats.getTotalRounds() > 0)
    {
        printRow("  Roulette Rounds", std::to_string(rouletteStats.getTotalRounds()));
        printRow("  Roulette Wins", std::to_string(rouletteStats.getTotalWins()));
        printRow("  Roulette Losses", std::to_string(rouletteStats.getTotalLosses()));
        printRow("  Roulette Win %", fmtPct(rouletteStats.getTotalWins(), rouletteStats.getTotalRounds()));
        printRow("  Roulette Straight Hits", std::to_string(rouletteStats.getStraightUpHits()));
        printRow("  Roulette Biggest Win", fmtMoney(rouletteStats.getBiggestWin()));
        printRow("  Roulette Biggest Loss", fmtMoney(rouletteStats.getBiggestLoss()));
        printRow("  Roulette Net", fmtMoney(rouletteStats.getNetProfit()));
    }

    if (totalRounds == 0)
    {
        cout << "  No games played yet.\n";
    }

    cout << std::string(W, '=') << "\n\n";
}

// =====================================================
// --- Terminal-only: END --- Stats display methods
// =====================================================