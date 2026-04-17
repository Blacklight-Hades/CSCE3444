///*
//	Elizabeth Stillwell
//	CSCE-3444-400, Software Engineering
//	University of North Texas
//	Professor Bahareh M. Dorri
//
//	Team Galactic's Space Casino casino simulator project
//	Slots Module
//	SlotTypes.h version 3
//		SlotTypes creates slot game states and the groundworks for the slot stats
//	last updated: 3/30/26
//*/

#ifndef SLOTTYPES_H
#define SLOTTYPES_H

#include "SlotWindow.h"

// CURRENTLY OUT OF COMMISSION - did not need but can implement
//enum class SlotState {		//Game states. WIP and currently not in use
//	WaitingForBet,
//	Bust,
//	Spinning
//};

struct SlotsSummary {		//The skeleton of slots stats for backend and session stats
	int spinNumber = 0;						// Number of spin in this session
	double startingBankroll = 0.0;
	double betMade = 0.0;
	double payoutAmount = 0.0;
	double endingBankroll = 0.0;
	double netChange = 0.0;

	// int reelPosit[3];		// Maybe come back to this later? for displaying what part of reel selected
										//	on the entire reel
	SlotWindow slotDisplay;		// Holds the display of the window
	char paytablCalc[7];		// Holds payCalc stack list

};


#endif