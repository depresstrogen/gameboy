#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "alpha.c"
#include "tiles.c"
#include "cursorspr.c"
#include "dungeon.c"
#include "keyboard.c"
#include <gb/gb.h>

void init();
void checkInput();
void updateSwitches();
int collisionCheck(int, int, int, int, int, int, int, int);
void printString(char string[], int len);
void updateCursor();
void updateString();

int cursor[] = {0, 0};
int cursorCoords[2]; //Player X and Y

unsigned char noWalk[2];

char message[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

int messageLen = 0;
int messageLine = 2;
char keebArray[4][10] = {
	{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x00},
	{0x21, 0x27, 0x15, 0x22, 0x24, 0x29, 0x25, 0x19, 0x1F, 0x20},
	{0x11, 0x23, 0x14, 0x16, 0x17, 0x18, 0x1A, 0x1B, 0x1C, 0x00},
	{0x2A, 0x28, 0x13, 0x26, 0x12, 0x1E, 0x1D, 0xFF, 0x00, 0x00}
	};

bool buttons[8];

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
	set_bkg_data(0, 49, alpha);		// Load 47 tiles into background memory
	
	// Use the 'helloWorld' array to write background tiles starting at 0,6 (tile positions)
	//  and write for 10 tiles in width and 2 tiles in height
	set_bkg_tiles(0,0,20,18,keyboard);			

	printString(message, messageLen);

	//Load sprites from sprites.c into sprite memory
	set_sprite_data(0, 1, cursorSPR);

	//sets first movable sprite (0) to be first tile in sprite mem (0)
	set_sprite_tile(0,0);

	cursor[0] = 0;
	cursor[1] = 0;
}

void updateSwitches() {
	
	HIDE_WIN;
	SHOW_SPRITES;
	SHOW_BKG;
	
}

void checkInput() {

    if (joypad() & J_B) {
		if (buttons[5] == false) {
			buttons[5] = true;
			for(int i = 0; i < messageLen; i++) {
				message[i] = 0xFF;
			}
			messageLen = 0;
			messageLine ++;
			
		}
    } else {
		buttons[5] = false;
	}


	if (joypad() & J_A) {
		if (buttons[4] == false) {
			buttons[4] = true;
			updateString();
		}
		

    } else {
		buttons[4] = false;
	}
	// UP
	if (joypad() & J_UP) {
		if (buttons[1] == false) {
			buttons[1] = true;
			cursor[1] = cursor[1] - 1;
		} 
		
		
	} else {
			buttons[1] = false;
		}

	// DOWN
	if (joypad() & J_DOWN) {
		if (buttons[2] == false) {
			buttons[2] = true;
			cursor[1]++;
		}
		
		
	}
	else {
			buttons[2] = false;
		}

	// LEFT
	if (joypad() & J_LEFT) {
		if (buttons[3] == false) {
			buttons[3] = true;
			cursor[0] = cursor[0] - 1;
		} 
		
		
	}	
	else {
			buttons[3] = false;
		}
	
	// RIGHT
	if (joypad() & J_RIGHT) {
		if (buttons[0] == false) {
			buttons[0] = true;
			cursor[0]++;
		}
		
		
	} else {
			buttons[0] = false;
		}
	
	// Move the sprite in the first movable sprite list (0)
	//  to the position of X (player[0]) and y (player[1])
	updateCursor();
	move_sprite(0, cursorCoords[0], cursorCoords[1]);
}

// Check if two rectangles from x1,y1, and extending out h1, h2, 
//  overlap with another, x2,y2, and extending out w2, h2
int collisionCheck(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {

	if ((x1 < (x2+w2)) && ((x1+w1) > x2) && (y1 < (h2+y2)) && ((y1+h1) > y2)) {
		
		return 1;
	
	} else {
		
		return 0;
	
	}

}

void printString(char string[], int len) {

		set_bkg_tiles(0,messageLine, len + 1 , 1, string);	
}

void updateCursor() {
	int basex = 8;
	int basey = 96;
	cursorCoords[0] = (cursor[0] * 16) + basex;
	if(cursor[1] > 1) {
		cursorCoords[0] = cursorCoords[0]  + ((cursor[1] - 1) * 8);
	}
	cursorCoords[1] = (cursor[1] * 16) + basey;
}

void updateString() {
	message[messageLen] = keebArray[cursor[1]][cursor[0]];
	messageLen ++;
	printString(message, messageLen);
}