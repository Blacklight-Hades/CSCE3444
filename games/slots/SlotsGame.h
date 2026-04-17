///*
//	Elizabeth Stillwell
//	CSCE-3444-400, Software Engineering
//	University of North Texas
//	Professor Bahareh M. Dorri
//
//	Team Galactic's Space Casino casino simulator project
//	Slots Module
//	SlotsGame.h version 3
//		Creates the SlotsGame class Slots
//	last updated: 3/30/26
//*/

#ifndef SLOTSGAME_H
#define SLOTSGAME_H

#include <string>
#include "SlotWindow.h"
#include <stack>
#include "SlotTypes.h"

class Slots {
private:
	//Symbols: B = bar, V = seven
	//		low-paying: J = jack, K = king, Q = queen, T = ten, 
	//		high-paying (themed): S = star, A = alien, M = moon, R = rocket
	//		multipliers: W = 2x, F = 5x
	char reels[3][30] = { {'B', 'J', 'S', 'T', 'J', 'K', 'A', 'M', 'M', 'W',
							'R', 'B', 'S', 'Q', 'K', 'T', 'A', 'J', 'S', 'Q',
							'M', 'K', 'T', 'R', 'V', 'J', 'T', 'R', 'S', 'A'}, //reel 1

							{'J', 'K', 'A', 'T', 'Q', 'V', 'B', 'S', 'K', 'T',
							'B', 'A', 'M', 'Q', 'K', 'R', 'R', 'S', 'T', 'B',
							'M', 'S', 'V', 'A', 'B', 'T', 'J', 'Q', 'F', 'R'} , //reel 2

							{'T', 'B', 'Q', 'R', 'B', 'J', 'M', 'A', 'B', 'K',
							'J', 'Q', 'W', 'V', 'B', 'M', 'T', 'K', 'R', 'J',
							'B', 'S', 'A', 'M', 'Q', 'Q', 'K', 'J', 'V', 'T'}  //reel 3
						};

//	SlotState gamestate;	//Stores current game state

	double bankroll;		//Stores bankroll for stats purposes
	double initbank;		//What the bank originally was at the beginning of the session(?)
	double currentbet;		//Stores user bet amount
	double payout;			//Stores payout
	bool won;				//true if win, false if no win

	///*int r1pos;			//possibly redundant
	//int r2pos;
	//int r3pos;*/
	int rpos[3];			//stores middle row positions for each reel

	SlotWindow slotw;		//A 3x3 slot machine window holding the symbols of the reels

	std::stack<char> payCalc;		//Stack to store the payout calculation method
	void placeBet(double bet);		//function that intializes the user bet, called by reelsSpin

public:

	Slots(double startingBankroll);		// Initializes slot starting bankroll for stats
	SlotWindow reelsSpin(double b);		// Spins and "starts game." Takes bet, returns Slotwindow
	double paytable();					// Calculates paytable and returns payout
	double getBankroll() const;         // Returns the current bankroll of the slot game
	void setBankroll(double amount);    // Sets the bankroll from SessionManager

};

#endif