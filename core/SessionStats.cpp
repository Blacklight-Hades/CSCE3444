/*
    Name:       Prayush Panta
    UID:        PP1008
    Team:       Group 11 - Team Galactic - Space Casino
    Course:     CSCE 3444 Software Engineering
    Instructor: Bahareh M. Dorri
*/

#include "SessionStats.h"
#include "../games/blackjack/BlackjackGame.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>
#include <cctype>
#include <algorithm>
#include <cmath>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::ostringstream;
using std::fixed;
using std::setprecision;
using std::setw;
using std::left;
using std::right;
using std::numeric_limits;
using std::streamsize;


//  Constructor

SessionStats::SessionStats(double startBalance)
    : bankroll(startBalance),
      totalRoundsAllGames(0),
      gamesPlayed(0),
      playedBlackjack(false),
      playedRoulette(false),
      blackjackStats(startBalance),
      slotsStats(startBalance),
      rouletteStats(startBalance),
      sessionStarted(false)
{
}


//  Session lifecycle

void SessionStats::startSession()
{
    sessionStart   = steady_clock::now();
    sessionStarted = true;
    cout << "\n Session started. Good luck!\n";
}

void SessionStats::endSession()
{
    sessionStarted = false;
}

double SessionStats::getSessionDuration() const
{
    if (!sessionStarted) return 0.0;
    auto now     = steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - sessionStart);
    return static_cast<double>(elapsed.count());
}


//  Blackjack game runner
//  - Passes Bankroll& into BlackjackGame (tighter integration)
//  - BlackjackGame calls withdraw() on bet, deposit() on payout
//  - After each round: validateBalance(), record stats
//  - Stats display only at session summary (not after each round)

void SessionStats::playBlackjack()
{
    if (!playedBlackjack)
    {
        playedBlackjack = true;
        gamesPlayed++;
    }

    cout << "\n========================================\n";
    cout << "           BLACKJACK\n";
    cout << "========================================\n";

    // Pass the shared Bankroll by reference — BlackjackGame owns
    // all withdraw/deposit calls during the round.
    BlackjackGame game(bankroll.getBalance(), 6);

    bool keepPlaying = true;

    while (keepPlaying && !bankroll.isBroke())
    {
        cout << "\nCurrent bankroll: $" << fixed << setprecision(2)
             << bankroll.getBalance() << "\n";
        cout << "Cards remaining : " << game.getCardsRemaining() << "\n";

        //  Prompt bet 
        double betAmount = 0.0;
        while (true)
        {
            cout << "Enter your bet (0 to return to menu): $";
            cin >> betAmount;

            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (betAmount == 0.0) { keepPlaying = false; break; }
            if (betAmount <  0.0) { cout << "Bet must be positive.\n"; continue; }
            if (betAmount > bankroll.getBalance())
            {
                cout << "Bet exceeds bankroll.\n";
                continue;
            }
            break;
        }

        if (!keepPlaying) break;

        // startNewRound: internally calls bankroll.withdraw(bet)
        if (!game.startNewRound(betAmount))
        {
            cout << "Could not start round.\n";
            continue;
        }

        cout << "\n--- Round " << game.getRoundNumber() << " ---\n";
        if (game.wasShoeReshuffledBeforeCurrentRound())
            cout << "Shoe reshuffled before this round.\n";
        if (game.isLastHandBeforeShuffle())
            cout << "Last hand before shuffle.\n";

        //  Player action loop 
        while (!game.isRoundOver())
        {
            // Show table state
            cout << "\nDealer: ";
            if (game.isDealerHoleCardRevealed())
            {
                cout << game.getDealerHand().toString()
                     << " (Value: " << game.getDealerValue() << ")\n";
            }
            else
            {
                auto dealerCards = game.getDealerHand().getCards();
                if (!dealerCards.empty())
                    cout << dealerCards[0].toString() << " ??\n";
            }

            for (int i = 0; i < game.getHandCount(); i++)
            {
                cout << "Player Hand " << (i + 1);
                if (!game.isRoundOver() && i == game.getActiveHandIndex())
                    cout << " [ACTIVE]";
                cout << ": " << game.getPlayerHand(i).toString()
                     << " (Value: " << game.getPlayerHand(i).getValue() << ")"
                     << " | Bet: $" << game.getPlayerHandBet(i);
                if (game.didPlayerDoubleDown(i)) cout << " | Doubled";
                cout << "\n";
            }

            // Prompt action
            string input;
            while (true)
            {
                cout << "\nAction: ";
                if (game.canHit())        cout << "[H]it ";
                if (game.canStand())      cout << "[S]tand ";
                if (game.canDoubleDown()) cout << "[D]ouble ";
                if (game.canSplit())      cout << "S[P]lit ";
                if (game.canRequestHint())cout << "Hin[T] ";
                cout << ": ";

                std::getline(cin, input);
                if (input.empty()) continue;

                char c = toupper(input[0]);

                if (c == 'H' && game.canHit())        { game.playerHit();        break; }
                if (c == 'S' && game.canStand())       { game.playerStand();      break; }
                if (c == 'D' && game.canDoubleDown())  { game.playerDoubleDown(); break; }
                if (c == 'P' && game.canSplit())       { game.playerSplit();      break; }
                if (c == 'T' && game.canRequestHint())
                {
                    game.requestHint();
                    cout << game.getHintText() << "\n";
                    // hint doesn't consume the action — re-prompt
                    continue;
                }

                cout << "Invalid action.\n";
            }
        }

        //  Round result 
        // resolveRound inside BlackjackGame calls bankroll.deposit(payout)
        cout << "\n========== ROUND RESULT ==========\n";
        cout << "Dealer: " << game.getDealerHand().toString()
             << " (Value: " << game.getDealerValue() << ")\n";

        for (int i = 0; i < game.getHandCount(); i++)
        {
            cout << "Player Hand " << (i + 1) << ": "
                 << game.getPlayerHand(i).toString()
                 << " (Value: " << game.getPlayerHand(i).getValue() << ")\n";
        }

        cout << game.getRoundResultText() << "\n";
        cout << "Payout : $" << fixed << setprecision(2) << game.getPayoutAmount() << "\n";
        cout << "Balance: $" << fixed << setprecision(2) << bankroll.getBalance()  << "\n";

        //  Post-round: validate balance, record stats 
        bankroll.validateBalance();

        BlackjackRoundSummary summary = game.getRoundSummary();
        blackjackStats.recordRound(summary);
        totalRoundsAllGames++;

        // Stats are NOT displayed here — only on session summary screen.

        if (bankroll.isBroke())
        {
            cout << "\nOut of money!\n";
            break;
        }

        //  Play again prompt 
        string again;
        while (true)
        {
            cout << "\nAnother hand? (Y/N): ";
            std::getline(cin, again);
            if (again.empty()) continue;
            char c = toupper(again[0]);
            if (c == 'Y') { keepPlaying = true;  break; }
            if (c == 'N') { keepPlaying = false; break; }
            cout << "Please enter Y or N.\n";
        }
    }

    cout << "\nReturning to casino menu. Balance: $"
         << fixed << setprecision(2) << bankroll.getBalance() << "\n";
}


//  playSlots — Sprint 2 placeholder
//  When ready: SlotsGame game(bankroll); game.run();

void SessionStats::playSlots()
{
    if (!playedSlots)
    {
        playedSlots = true;
        gamesPlayed++;
    }

    cout << "\n[Slots] Balance: $" << fixed << setprecision(2)
         << bankroll.getBalance() << "\n";
    cout << "[Slots] Coming in Sprint 2 — plug SlotsGame(bankroll) here.\n";

    // Sprint 2 integration point:
    // SlotsGame game(bankroll);
    // game.run();
    // SlotsRoundSummary summary = game.getRoundSummary();
    // slotsStats.recordRound(summary);
    // bankroll.validateBalance();
    // totalRoundsAllGames += game.getRoundsPlayed();
}


//  playRoulette — Sprint 2 placeholder
//  When ready: RouletteGame game(bankroll); game.run();

void SessionStats::playRoulette()
{
    if (!playedRoulette)
    {
        playedRoulette = true;
        gamesPlayed++;
    }

    cout << "\n[Roulette] Balance: $" << fixed << setprecision(2)
         << bankroll.getBalance() << "\n";
    cout << "[Roulette] Coming in Sprint 2 — plug RouletteGame(bankroll) here.\n";

    // Sprint 2 integration point:
    // RouletteGame game(bankroll);
    // game.run();
    // RouletteRoundSummary summary = game.getRoundSummary();
    // rouletteStats.recordRound(summary);
    // bankroll.validateBalance();
    // totalRoundsAllGames += game.getRoundsPlayed();
}


//  Session summary display

void SessionStats::displaySessionSummary() const
{
    const int W = 52;

    double net = bankroll.getNetGainLoss();
    double dur = const_cast<SessionStats*>(this)->getSessionDuration();

    cout << endl;
    printDivider('=', W);
    cout << setw((W + 26) / 2) << right
         << "* GALACTIC CASINO  --  SESSION SUMMARY *\n";
    printDivider('=', W);

    //  Bankroll overview 
    cout << " BANKROLL\n";
    printDivider('-', W);
    printRow("Starting Balance", formatMoney(bankroll.getStartingBalance()), W);
    printRow("Final Balance",    formatMoney(bankroll.getBalance()),         W);
    printRow("Net Gain / Loss",  formatMoney(net),                          W);
    printRow("Peak Balance",     formatMoney(bankroll.getPeakBalance()),     W);
    printRow("Lowest Balance",   formatMoney(bankroll.getLowestBalance()),   W);

    //  Session overview 
    cout << "\n SESSION\n";
    printDivider('-', W);
    printRow("Session Duration", formatDuration(dur),                    W);
    printRow("Total Rounds",     std::to_string(totalRoundsAllGames),    W);
    printRow("Games Played",     std::to_string(gamesPlayed),            W);

    //  Per-game breakdown 
    cout << "\n PER-GAME BREAKDOWN\n";
    printDivider('-', W);

    if (playedBlackjack)
    {
        cout << "  Blackjack\n";
        printRow("    Hands Played",  std::to_string(blackjackStats.getTotalRounds()), W);
        printRow("    Wins",          std::to_string(blackjackStats.getTotalWins()),   W);
        printRow("    Losses",        std::to_string(blackjackStats.getTotalLosses()), W);
        printRow("    Pushes",        std::to_string(blackjackStats.getTotalPushes()), W);
        printRow("    Blackjacks",    std::to_string(blackjackStats.getTotalBlackjacks()), W);
        printRow("    Player Busts",  std::to_string(blackjackStats.getTotalPlayerBusts()), W);
        printRow("    Double Downs",  std::to_string(blackjackStats.getTotalDoubleDowns()), W);
        printRow("    Win Streak",    std::to_string(blackjackStats.getLongestWinStreak()), W);
        printRow("    Loss Streak",   std::to_string(blackjackStats.getLongestLossStreak()), W);

        ostringstream wr;
        wr << fixed << setprecision(1) << blackjackStats.getWinRate() * 100.0 << "%";
        printRow("    Win Rate", wr.str(), W);

        printRow("    Net",      formatMoney(blackjackStats.getNetProfit()), W);
    }

    if (playedSlots)
    {
        cout << "  Slots\n";
        printRow("    Spins Played", std::to_string(slotsStats.getTotalRounds()), W);
        printRow("    Wins", std::to_string(slotsStats.getTotalWins()), W);
        printRow("    Losses", std::to_string(slotsStats.getTotalLosses()), W);
        printRow("    Three in a Row", std::to_string(slotsStats.getThreeInARowHits()), W);
        printRow("    Jackpots", std::to_string(slotsStats.getJackpotHits()), W);
        
        ostringstream wr;
        wr << fixed << setprecision(1) << slotsStats.getWinRate() * 100.0 << "%";
        printRow("    Win Rate", wr.str(), W);

        printRow("    Net", formatMoney(slotsStats.getNetProfit()), W);
    }
    
    if (playedRoulette)
    {
        cout << "  Roulette\n";
        printRow("    Spins Played", std::to_string(rouletteStats.getTotalRounds()), W);
        printRow("    Wins", std::to_string(rouletteStats.getTotalWins()), W);
        printRow("    Losses", std::to_string(rouletteStats.getTotalLosses()), W);
        printRow("    Straight Up Hits", std::to_string(rouletteStats.getStraightUpHits()), W);
        
        ostringstream wr;
        wr << fixed << setprecision(1) << rouletteStats.getWinRate() * 100.0 << "%";
        printRow("    Win Rate", wr.str(), W);

        printRow("    Net", formatMoney(rouletteStats.getNetProfit()), W);
    }

    printDivider('=', W);
    cout << endl;

    //  Full Blackjack stats panel 
    if (playedBlackjack)
        blackjackStats.displayStats();
}


//  Helpers

string SessionStats::formatMoney(double amount) const
{
    ostringstream out;
    if (amount >= 0)
        out << "+$" << fixed << setprecision(2) << amount;
    else
        out << "-$" << fixed << setprecision(2) << std::abs(amount);
    return out.str();
}

string SessionStats::formatDuration(double seconds) const
{
    int mins = static_cast<int>(seconds) / 60;
    int secs = static_cast<int>(seconds) % 60;
    ostringstream out;
    out << mins << "m " << secs << "s";
    return out.str();
}

void SessionStats::printDivider(char c, int width) const
{
    cout << string(width, c) << endl;
}

void SessionStats::printRow(const string& label, const string& value, int width) const
{
    int gap = width - 2 - (int)label.size() - (int)value.size();
    if (gap < 1) gap = 1;
    cout << "  " << label << string(gap, '.') << value << endl;
}
