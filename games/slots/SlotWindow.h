///*
//	Elizabeth Stillwell
//	CSCE-3444-400, Software Engineering
//	University of North Texas
//	Professor Bahareh M. Dorri
//
//	Team Galactic's Space Casino casino simulator project
//	Slots Module
//	SlotWindow.h version 3
//		This class supports the display of the slot reels "in the slot machine window"
//		to ensure the proper symbols and reel positions are shown.
//	last updated: 3/30/26
//*/

#ifndef SLOTWINDOW_H
#define SLOTWINDOW_H

class SlotWindow {
private:
	char display[3][3];			// Window character display, 3x3
public:
	void setDisplay(char a, int i, int j) {		// Initializes the characters in the window.
		display[i][j] = a;
	};
	char getDisplay(int i, int j) {				// Returns the characters in the window.
		return display[i][j];					//	needs to be iterated through
	};
};

#endif
