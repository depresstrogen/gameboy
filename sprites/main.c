
#include "alpha.c"
#include "tiles.c"
#include "sprites.c"
#include "dungeon.c"
#include <gb/gb.h>

void init();
void checkInput();
void updateSwitches();
int collisionCheck(int, int, int, int, int, int, int, int);


int player[2]; //Player X and Y
int enemy[2];

unsigned char noWalk[2];

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
	set_bkg_data(0, 23, tiles);		// Load 47 tiles into background memory
	
	// Use the 'helloWorld' array to write background tiles starting at 0,6 (tile positions)
	//  and write for 10 tiles in width and 2 tiles in height
	set_bkg_tiles(0,0,20,18,dungeon);			

	//Load sprites from sprites.c into sprite memory
	set_sprite_data(0, 1, sprites);

	//sets first movable sprite (0) to be first tile in sprite mem (0)
	set_sprite_tile(0,0);
	set_sprite_tile(1,1);


	player[0] = 64;
	player[1] = 64;

	enemy[0] = 128;
	enemy[1] = 128;
}

void updateSwitches() {
	
	HIDE_WIN;
	SHOW_SPRITES;
	SHOW_BKG;
	
}

void checkInput() {

    if (joypad() & J_B) {
    }

	// UP
	if (joypad() & J_UP) {
			
		player[1] = player[1] - 1;
		
	}

	// DOWN
	if (joypad() & J_DOWN) {
			
		player[1]++;
		
	}

	// LEFT
	if (joypad() & J_LEFT) {
		
		player[0] = player[0] - 1;
		
	}	
	
	// RIGHT
	if (joypad() & J_RIGHT) {
		
		player[0]++;
		
	}
	
		// Move the sprite in the first movable sprite list (0)
	//  to the position of X (player[0]) and y (player[1])
	move_sprite(0, player[0], player[1]);
	
	// Move the sprite in the second movable sprite list (1)
	//  to the position of X (enemy[0]) and y (enemy[1])
	move_sprite(1, enemy[0], enemy[1]);
	
	// Is the player colliding with the enemy?
	if(collisionCheck(player[0], player[1], 8, 8, enemy[0], enemy[1], 8, 8) == 1) {
		set_sprite_tile(0,1);
	} else {
		set_sprite_tile(0,0);
	}
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