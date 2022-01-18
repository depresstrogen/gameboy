#include <gb/gb.h>
#include <gb/drawing.h>
#include "ricktiles.c"
#include "rickmap.c"

// So you can just steal notes from the internet
enum notes {
  C0, Cd0, D0, Dd0, E0, F0, Fd0, G0, Gd0, A0, Ad0, B0,
  C1, Cd1, D1, Dd1, E1, F1, Fd1, G1, Gd1, A1, Ad1, B1,
  C2, Cd2, D2, Dd2, E2, F2, Fd2, G2, Gd2, A2, Ad2, B2,
  C3, Cd3, D3, Dd3, E3, F3, Fd3, G3, Gd3, A3, Ad3, B3,
  C4, Cd4, D4, Dd4, E4, F4, Fd4, G4, Gd4, A4, Ad4, B4,
  C5, Cd5, D5, Dd5, E5, F5, Fd5, G5, Gd5, A5, Ad5, B5,
  SILENCE, END
};

// Frequencies Corresponding to Notes
const uint16_t frequencies[] = {
  44, 156, 262, 363, 457, 547, 631, 710, 786, 854, 923, 986,
  1046, 1102, 1155, 1205, 1253, 1297, 1339, 1379, 1417, 1452, 1486, 1517,
  1546, 1575, 1602, 1627, 1650, 1673, 1694, 1714, 1732, 1750, 1767, 1783,
  1798, 1812, 1825, 1837, 1849, 1860, 1871, 1881, 1890, 1899, 1907, 1915,
  1923, 1930, 1936, 1943, 1949, 1954, 1959, 1964, 1969, 1974, 1978, 1982,
  1985, 1988, 1992, 1995, 1998, 2001, 2004, 2006, 2009, 2011, 2013, 2015
};

//The notes to play upon button press
const uint8_t music[] = {
  A2, B2/* Never*/, D3, B2/*Gonna*/, Fd3 /*Give*/, Fd3/*You*/, E3/*Up*/,
  A2, B2/* Never*/, D3, B2/*Gonna*/, E3 /*Let*/, E3/*You*/, D3, Cd3, B2/*Downnn*/,
  A2, B2/* Never*/, D3, B2/*Gonna*/, D3/*Run*/, E3, Cd3 /*Around*/, A2/*And*/, A2, E3/*Desert*/, D3/*You*/,
  END
};

//The length of each note
const long length[] = {
	250, 250, 250, 250, 500, 500, 700,
	250, 250, 250, 250, 500, 500, 500, 200, 500,
	250, 250, 250, 250, 500, 300, 500, 500, 300, 500, 300
};




void main() {

	// 1000 0000 Turns on sound
	NR52_REG = 0x80;
	// 0111 1111 Sets volume for both channels
    NR50_REG = 0x77;
	// 1111 1111 Set which channels are on (all of them)
    NR51_REG = 0xFF;

    

	// Turn on the display
	DISPLAY_ON;						
	//Load tiles into memory		
	set_bkg_data(0, 128, RICKTILES);
	
	// Print the map at X: 4 Y: 8 in a 12x12 area
	set_bkg_tiles(4,8,12,12,RICKMAP);
	//Make BKG Visible
	SHOW_BKG;

	//Counter Variable
	int note = 0;
    while(1) {

        UBYTE joypad_state = joypad();
		// If a DPAD button is pressed
        if(joypad_state) {
			//If the current note is not the end
			while (music[note] != END) {
				//Get frequency of note
				uint16_t value = frequencies[music[note]];

				/*
				NR10 Register - Frequency Sweep
				Bit 7   - Unused
				Bit 6-4 - Sweep Time (0 If Off)
				Bit 3   - Sweep Direction (1 = Decrease 0 = Increase)
				Bit 2-0 - Sweep Shift Amount (0 If Off)
				7 654 3 210
				0 000 0 000 (0x00 = Clean Note)
				*/
				NR10_REG = 0x00;
				/*
				NR11 Register  - Wave Pattern Cycle & Sound Length
				Applies to Channels 1, 2, 4
				Bit 7-6 - Wave Pattern Cycle 0-3 (12.5%, 25%, 50%, 75%), How long wave is on / off, 50% for clean tone
				Bit 5-0 - Sound Length (Higher = Shorter)
				76 543210
				10 111000 (0xB8 = Clean Note)
				*/
				NR11_REG = 0xB8;

				/*
				NR12 Register - Volume Envelope (Makes volume louder or quiter over time)
				Applies to Channels 1, 2, 4
				Bit 7-4 - Channel Volume (Initial)
				Bit 3   - Envelope Directoin (0 = Down 1 = Up)
				Bit 2-0 - Length of Step Between Volume Changes (n/64 Seconds) 
				7654 3 210
				1111 0 000 (0xF0 = Clean Note)
				*/
				NR12_REG = 0xF6;

				/*
				NR13 Register - Frequency Low (Least Significant)
				Applies to Channels 1, 2, 3
				Bit 7-0 - Least Significant Byte of Frequency (Out of 11 Bits)
				Ex) Frequency = 1546 (C2) = 101 11110110
				76543210
				11110110 (Value Casted to Byte)
				*/
				NR13_REG = (uint8_t) value;
				/*
				NR14 Register - Playback & Frequency High (Most Significant)
				Applies to Channels 1, 2, 3, 4
				Bit 7   - Initialize Channel (Write Only)
				Bit 6   - "Consecutive Select" (When 0 Ignores Length Data on NR11)
				Bit 5-3 - Unused
				Bit 2-0 - 3 Most Significant Bits of Frequency (Out of 11 Bits)
				Ex) Frequency = 1546 (C2) = 101 11110110
				7 6 543 210
				1 0 000 101 (0x80 + Value Shifted 8 Bits Right)
				*/
				NR14_REG = 0x80 + (value >> 8);

				// Wait n length to play next note, USES ALL CPU TIME VERY INEFFICIENT
				delay(length[note]);

				//Increment note count
				note ++;
       		}
			//Set Note to 0 to Play Again
			if(music[note] == END) {
					note = 0;
			}
		}
	}
}