
#include "alpha.c"
#include "helloWorld.c"
#include "blankScreen.c"
#include <gb/gb.h>

void init();
void checkInput();
void updateSwitches();

void main() {

	init();
	
	while(1) {
		
		checkInput();				// Check for user input (and act on it)
		updateSwitches();			// Make sure the SHOW_SPRITES and SHOW_BKG switches are on each loop
		wait_vbl_done();			// Wait until VBLANK to avoid corrupting memory
	}
	
}

void init() {
	
	DISPLAY_ON;						// Turn on the display
	set_bkg_data(0, 47, alpha);		// Load 47 tiles into background memory
	
	// Use the 'helloWorld' array to write background tiles starting at 0,6 (tile positions)
	//  and write for 10 tiles in width and 2 tiles in height

	set_bkg_tiles(0,6,10,2,helloWorld);			

}

void updateSwitches() {
	
	HIDE_WIN;
	SHOW_SPRITES;
	SHOW_BKG;
	
}

void checkInput() {

    if (joypad() & J_B) {
		// The B button was pressed!
        set_bkg_tiles(0,0,20,18,blankScreen);
    }

}