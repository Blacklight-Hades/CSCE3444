/*
	Elizabeth Stillwell
	CSCE-3444-400, Software Engineering
	University of North Texas
	Professor Bahareh M. Dorri

	Team Galactic's Space Casino casino simulator project
	Slots Module
	main.cpp version 3
		Runs the slots module in terminal for testing purposes
	last updated: 3/30/26
*/

#include <iostream>
#include <string>
#include "SlotsGame.h"
#include "SlotWindow.h"


using namespace std;

//
//		CURRENT TO-DO: (3/26/26)
//		-> create game state (check how Ramon did it and emulate that) 
//				and use that to pull stuff
//		-> clear, communicative comments for Kaden and Prayush
//		-> stats session class that can be created, have stats set, and sent to
//				backend overall game session
//		-> continued error catching/testing/handling
//		


int main() {
	double bank;
	double money;
	double payOut = 0.0;
	char cont;
	SlotWindow slotwindow;		//Slotwindow creates a class in SlotWindow.h to pass the
								//	display from the slot reels with

	cout << "Welcome to Slots. Please tell us how much money is in your bank account:   $";
	cin >> bank;
	Slots game(bank);
	do {
		cout << "Great! Now how much would you like to bet today?   $";
		cin >> money;
		
		if (money > bank) {
			cout << "Sorry, you don't have enough money!" << endl;
		}
		else {
			bank -= money;
			cout << endl << "Alright, time to spin the wheel." << endl;
			slotwindow = game.reelsSpin(money);				// Spins the slot machine and 
															//assigns the results of the spin 
															// to the slotwindow

			cout << "Results: \n"; 
			for (int i = 0; i < 3; i++) {		//Displays the slots table by iterating through 
				for (int j = 0; j < 3; j++) {	//	the slot window struct's array
					cout << (slotwindow.getDisplay(i,j)) << "  ";
				}
				cout << endl;
			}
			payOut = game.paytable();
			cout << "Your payout is: " << payOut << endl;		//returns the payout
			bank += payOut;				//returns updated bankroll
													//	possibly just add payout to bankroll
													//	after taking out bet.
			cout << "Your new bank balance is: " << bank;
			payOut = 0.0;
		}
		cout << endl << "Would you like to play again? Y/N:   ";
		cin >> cont;
	} while (cont == 'Y' || cont == 'y');

	cout << "Thanks for playing!" << endl;
	return 0;
}