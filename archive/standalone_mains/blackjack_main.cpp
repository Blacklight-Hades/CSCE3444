/*
    Author:     Ramon Lopez | rjl0157 | ramonlopez2@my.unt.edu
    Team:       Group 11 - Team Galactic - Space Casino
    Course:     CSCE 3444.400 Software Engineering
    Instructor: Bahareh M. Dorri
*/

#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <cctype>
#include <fstream>
#include <iomanip>
#include "BlackjackGame.h"

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;
using std::vector;
using std::numeric_limits;
using std::streamsize;
using std::fixed;
using std::setprecision;

double promptStartingBankroll();
double promptBetAmount(const BlackjackGame& game);
string promptPlayerAction(const BlackjackGame& game);
void showTableState(const BlackjackGame& game);
string getDealerVisibleHand(const BlackjackGame& game);
void showSessionInfo(const BlackjackGame& game);
void showFinalSessionSummary(const BlackjackGame& game, double startingBankroll);
bool promptPlayAgain();
void writeRoundLogToFile(const BlackjackGame& game);

// Appends round log output to file.
void writeRoundLogToFile(const BlackjackGame& game)
{
    std::ofstream logFile("blackjack_log.txt", std::ios::app);

    std::vector<std::string> roundLog = game.getRoundLog();

    for (const std::string& entry : roundLog)
    {
        logFile << entry << "\n";
    }

    logFile << "\n";
    logFile.close();
}

// Main game loop.
int main()
{
    cout << "========================================" << endl;
    cout << "     Welcome to Terminal Blackjack" << endl;
    cout << "========================================" << endl;

    double startingBankroll = promptStartingBankroll();
    BlackjackGame game(startingBankroll);

    bool keepPlaying = true;

    while (keepPlaying && game.getTableBalance() > 0.0)
    {
        cout << endl;
        cout << "Current table balance: $" << game.getTableBalance() << endl;
        cout << "Cards remaining in shoe: " << game.getCardsRemaining() << endl;

        double betAmount = promptBetAmount(game);

        if (!game.startNewRound(betAmount))
        {
            cout << "Could not start round. Please try again." << endl;
            continue;
        }

        cout << endl;
        cout << "----------------------------------------" << endl;
        cout << "Round " << game.getRoundNumber() << endl;
        cout << "----------------------------------------" << endl;

        cout << "Initial Bet: $" << game.getInitialBet() << endl;
        cout << "Table balance after initial bet deduction: $" << game.getTableBalance() << endl;

        if (game.wasShoeReshuffledBeforeCurrentRound())
        {
            cout << "The shoe was reshuffled before this round." << endl;
        }

        if (game.isLastHandBeforeShuffle())
        {
            cout << "This is the last hand before the shoe is shuffled." << endl;
        }

        bool redrawTable = true;

        while (!game.isRoundOver())
        {
            if (redrawTable)
            {
                showTableState(game);
            }

            string action = promptPlayerAction(game);

            if (action == "H")
            {
                game.playerHit();
                redrawTable = true;
            }
            else if (action == "S")
            {
                game.playerStand();
                redrawTable = true;
            }
            else if (action == "D")
            {
                game.playerDoubleDown();
                redrawTable = true;
            }
            else if (action == "P")
            {
                game.playerSplit();
                redrawTable = true;
            }
            else if (action == "T")
            {
                game.requestHint();

                cout << endl;
                cout << game.getHintText() << endl;

                redrawTable = false;
            }
            else if (action == "I")
            {
                showSessionInfo(game);
                cout << endl;
                redrawTable = false;
            }
        }

        cout << endl;
        cout << "========== ROUND RESULT ==========" << endl << endl;
        cout << game.getRoundResultText() << endl;

        writeRoundLogToFile(game);

        if (game.isReshufflePending())
        {
            cout << endl;
            cout << "The shoe will be reshuffled before the next round." << endl;
        }

        if (game.getTableBalance() <= 0.0)
        {
            cout << endl;
            cout << "You are out of table funds. Session over." << endl;
            break;
        }

        keepPlaying = promptPlayAgain();
    }

    showFinalSessionSummary(game, startingBankroll);

    cout << endl;
    cout << "Thanks for playing Blackjack." << endl;
    return 0;
}

double promptStartingBankroll()
{
    double bankroll;

    while (true)
    {
        cout << "Enter starting bankroll: $";
        cin >> bankroll;

        if (cin.fail() || bankroll <= 0.0)
        {
            cout << "Please enter a valid amount greater than 0." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return bankroll;
        }
    }
}

double promptBetAmount(const BlackjackGame& game)
{
    double betAmount;

    while (true)
    {
        cout << "Enter your bet: $";
        cin >> betAmount;

        if (cin.fail())
        {
            cout << "Invalid input. Please enter a numeric bet." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (betAmount <= 0.0)
        {
            cout << "Bet must be greater than 0." << endl;
        }
        else if (betAmount > game.getTableBalance())
        {
            cout << "You cannot bet more than your current table balance." << endl;
        }
        else
        {
            return betAmount;
        }
    }
}

// Prompts only currently legal actions.
string promptPlayerAction(const BlackjackGame& game)
{
    string input;

    while (true)
    {
        cout << endl;
        cout << "Choose action for hand " << (game.getActiveHandIndex() + 1) << ": ";

        if (game.canHit())
        {
            cout << "[H]it ";
        }

        if (game.canStand())
        {
            cout << "[S]tand ";
        }

        if (game.canDoubleDown())
        {
            cout << "[D]ouble Down ";
        }

        if (game.canSplit())
        {
            cout << "S[P]lit ";
        }

        if (game.canRequestHint())
        {
            cout << "Hin[T] ";
        }

        cout << "[I]nfo ";

        cout << ": ";
        getline(cin, input);

        if (input.size() == 1)
        {
            char choice = toupper(input[0]);

            if (choice == 'H' && game.canHit())
            {
                return "H";
            }

            if (choice == 'S' && game.canStand())
            {
                return "S";
            }

            if (choice == 'D')
            {
                if (game.canDoubleDown())
                {
                    return "D";
                }

                cout << "Double down is not available right now." << endl;
                cout << "You must have enough table balance to match that hand's bet, and the hand must still be on its opening decision window." << endl;
                continue;
            }

            if (choice == 'P')
            {
                if (game.canSplit())
                {
                    return "P";
                }

                cout << "Split is not available right now." << endl;
                cout << "You need a splittable 2-card hand and enough table balance to fund the additional hand." << endl;
                continue;
            }

            if (choice == 'T')
            {
                if (game.canRequestHint())
                {
                    return "T";
                }

                cout << "Hint is not available right now." << endl;
                continue;
            }

            if (choice == 'I')
            {
                return "I";
            }
        }

        cout << "Invalid action. Please choose one of the available options." << endl;
    }
}

// Displays dealer and player hands for the current round.
void showTableState(const BlackjackGame& game)
{
    cout << endl;
    cout << "Dealer Hand: ";

    if (game.isDealerHoleCardRevealed())
    {
        cout << game.getDealerHand().toString();
        cout << " (Value: " << game.getDealerValue() << ")" << endl;
    }
    else
    {
        cout << getDealerVisibleHand(game) << endl;
    }

    for (int i = 0; i < game.getHandCount(); i++)
    {
        cout << "Player Hand " << (i + 1);

        if (!game.isRoundOver() && i == game.getActiveHandIndex())
        {
            cout << " [ACTIVE]";
        }

        cout << ": " << game.getPlayerHand(i).toString()
             << " (Value: " << game.getPlayerHand(i).getValue() << ")"
             << " | Bet: $" << game.getPlayerHandBet(i);

        if (game.didPlayerDoubleDown(i))
        {
            cout << " | Doubled";
        }

        cout << endl;
    }
}

// Hides dealer hole card until reveal.
string getDealerVisibleHand(const BlackjackGame& game)
{
    vector<Card> dealerCards = game.getDealerHand().getCards();

    if (dealerCards.empty())
    {
        return "";
    }

    if (dealerCards.size() == 1)
    {
        return dealerCards[0].toString();
    }

    return dealerCards[0].toString() + " ??";
}

// Shows current session stats.
void showSessionInfo(const BlackjackGame& game)
{
    BlackjackAccuracyStats accuracyStats = game.getSessionAccuracyStats();
    BlackjackLuckStats luckStats = game.getSessionLuckStats();

    cout << endl;
    cout << "========== SESSION INFO ==========" << endl;
    cout << "Current Table Balance: $" << fixed << setprecision(2) << game.getTableBalance() << endl;
    cout << "Strategy Accuracy: " << fixed << setprecision(2)
         << accuracyStats.getAccuracyPercent() << "%" << endl;
    cout << "Optimal Actions: " << accuracyStats.optimalActions << endl;
    cout << "Suboptimal Actions: " << accuracyStats.suboptimalActions << endl;
    cout << "Hints Used: " << accuracyStats.hintsUsed << endl;
    cout << "Luck Factor: " << fixed << setprecision(2)
         << luckStats.getLuckFactorPercent() << "%" << endl;
    cout << "Lucky Hands: " << luckStats.luckyHands << endl;
    cout << "Semi-Lucky Pushes: " << luckStats.semiLuckyPushes << endl;
    cout << "Unlucky Hands: " << luckStats.unluckyHands << endl;
    cout << "==================================" << endl;
}

// Shows final session totals.
void showFinalSessionSummary(const BlackjackGame& game, double startingBankroll)
{
    BlackjackAccuracyStats accuracyStats = game.getSessionAccuracyStats();
    BlackjackLuckStats luckStats = game.getSessionLuckStats();

    double finalTableBalance = game.getTableBalance();
    double netChange = finalTableBalance - startingBankroll;

    cout << endl;
    cout << "========== SESSION SUMMARY ==========" << endl;
    cout << "Starting Table Balance: $" << fixed << setprecision(2) << startingBankroll << endl;
    cout << "Final Table Balance:    $" << fixed << setprecision(2) << finalTableBalance << endl;

    if (netChange >= 0.0)
    {
        cout << "Net Change:             +$" << fixed << setprecision(2) << netChange << endl;
    }
    else
    {
        cout << "Net Change:             -$" << fixed << setprecision(2) << (-netChange) << endl;
    }

    cout << endl;
    cout << "Strategy Accuracy: " << fixed << setprecision(2)
         << accuracyStats.getAccuracyPercent() << "%" << endl;
    cout << "Optimal Actions:   " << accuracyStats.optimalActions << endl;
    cout << "Suboptimal Actions:" << accuracyStats.suboptimalActions << endl;
    cout << "Hints Used:        " << accuracyStats.hintsUsed << endl;

    cout << endl;
    cout << "Luck Factor:       " << fixed << setprecision(2)
         << luckStats.getLuckFactorPercent() << "%" << endl;
    cout << "Lucky Hands:       " << luckStats.luckyHands << endl;
    cout << "Semi-Lucky Pushes: " << luckStats.semiLuckyPushes << endl;
    cout << "Unlucky Hands:     " << luckStats.unluckyHands << endl;
    cout << "=====================================" << endl;
}

bool promptPlayAgain()
{
    string input;

    while (true)
    {
        cout << endl;
        cout << "Play another round? (Y/N): ";
        getline(cin, input);

        if (input.size() == 1)
        {
            char choice = toupper(input[0]);

            if (choice == 'Y')
            {
                return true;
            }

            if (choice == 'N')
            {
                return false;
            }
        }

        cout << "Please enter Y or N." << endl;
    }
}