/*	This is a stripped down version of the Georgi engine meant for use with
 *	Ginni. As such serial-Steno features are disabled, chords are 16bits and
 *	crap is removed where possible
 *
 *	Do not use this on anything other then Ginny if you want to be sane
 */
#include "engine.h"

// Chord state
C_SIZE		cChord				= 0;			// Current Chord
int			 	chordIndex		= 0;			// Keys in previousachord
int			 	pressed				= 0;			// number of held keys
C_SIZE		chordState[32];					// Full Chord history
#define  	QWERBUF				24				// Size of chords to buffer for output

bool	 		repeatFlag		= false;	// Should we repeat?
C_SIZE		pChord				= 0;			// Previous Chord
C_SIZE		stickyBits 		= 0;			// Or'd with every incoming press
int		 		pChordIndex		= 0;			// Keys in previousachord
C_SIZE		pChordState[32];				// Previous chord sate 

// Key Dicts
extern const struct keyEntry		keyDict[];
extern size_t keyLen;
extern const struct comboEntry	cmbDict[];
extern size_t comboLen; 
extern const struct funcEntry 	funDict[];
extern size_t funcsLen;
extern const struct stringEntry	strDict[];
extern size_t stringLen; 
extern const struct specialEntry spcDict[];
extern size_t specialLen; 

// Mode state
enum MODE { STENO = 0, QWERTY, COMMAND };
enum MODE pMode;
enum MODE cMode = QWERTY;

// Command State
#define MAX_CMD_BUF   20
uint8_t	 CMDLEN				= 0;
uint8_t	 CMDBUF[MAX_CMD_BUF];

// Key Repeat state
bool     inChord			= false;
bool	 	 repEngaged		= false;
uint16_t repTimer			= 0;
#define  REP_INIT_DELAY 750
#define  REP_DELAY		25

// Mousekeys state
bool		inMouse			= false;
int8_t	mousePress;


// All processing done at chordUp goes through here
void processKeysUp() { 
	// Check for mousekeys, this is release
#ifdef MOUSEKEY_ENABLE
	if (inMouse) {
		inMouse = false;
		mousekey_off(mousePress);
		mousekey_send();
	}
#endif

	// handle command mode
	if (cChord == COMMAND_MODE) {
#ifndef NO_DEBUG
		uprintf("COMMAND Toggle\n");
#endif
		if (cMode != COMMAND) {   // Entering Command Mode
			CMDLEN = 0;
			pMode = cMode;
			cMode = COMMAND;
		} else {                  // Exiting Command Mode
			cMode = pMode;

			// Press all and release all
			for (int i = 0; i < CMDLEN; i++) {
				register_code(CMDBUF[i]);
			}
			clear_keyboard();
		}
	}

	// Process and reset state
	processChord();
	cChord = 0;
	inChord = false;
	chordIndex = 0;
	clear_keyboard();
	repEngaged  = false;
	for (int i = 0; i < 32; i++)
		chordState[i] = 0xFFFF;
}

// Update Chord State 
bool process_record_kb(uint16_t keycode, keyrecord_t *record) { 
	// Everything happens in here when steno keys come in.
	// Bail on keyup
	bool pr = record->event.pressed;
	if (record->event.pressed) {
		pressed++;
	} else {
		pressed--;
	}

	// All keys up, send it!
	if (pressed == 0) {
		processKeysUp();
		repEngaged = false;
		return false;
	}

	// Update key repeat timers
	repTimer = timer_read();
	inChord  = true;

	// Switch on the press adding to chord
	switch (keycode) {
		ENGINE_CONFIG
		default: return true;
	}

	// Store previous state for fastQWER
	if (pr) {
		chordState[chordIndex] = cChord; 
		chordIndex++;
	}

#ifndef NO_DEBUG
	uprintf("Chord: %u\n", cChord);
#endif
	return false; 
}
void matrix_scan_user(void) {
	// We abuse this for early sending of key
	// Key repeat only on QWER/SYMB layers
	if (cMode != QWERTY || !inChord) return;

	// Check timers
#ifndef NO_REPEAT
	if (repEngaged && timer_elapsed(repTimer) > REP_DELAY) {
		// Process Key for report
		processChord();

		// Send report to host
		send_keyboard_report();
		clear_keyboard();
		repTimer = timer_read();
	}

	if (!repEngaged && timer_elapsed(repTimer) > REP_INIT_DELAY) {
		repEngaged = true;
	}
#endif
};

// Try and match cChord
C_SIZE mapKeys(bool lookup) {
#ifndef NO_DEBUG
	if (!lookup) uprint("SENT!");
#endif
	// Single key chords
	for (int i = 0; i < keyLen; i++) {
		if (keyDict[i].chord == cChord) {
			if (!lookup) SEND(keyDict[i].key);
			return cChord;
		}
	}

	// strings
	for (int i = 0; i < stringLen; i++) {
		struct stringEntry fromPgm;
		memcpy_P(&fromPgm, &strDict[i], sizeof(stringEntry_t));
		if (fromPgm.chord == cChord) {
			if (!lookup) send_string_P((PGM_P)(fromPgm.str));
			return cChord;
		}
	}

	// combos
	for (int i = 0; i < comboLen; i++) {
		struct comboEntry fromPgm;
		memcpy_P(&fromPgm, &cmbDict[i], sizeof(comboEntry_t));
		if (fromPgm.chord == cChord) {
#ifndef NO_DEBUG
			uprintf("%d found combo\n", i);
#endif

			if (!lookup) {
				uint8_t comboKeys[COMBO_MAX];
				memcpy_P(&comboKeys, fromPgm.keys, sizeof(uint8_t)*COMBO_MAX);
				for (int j = 0; j < COMBO_MAX; j++) 
#ifndef NO_DEBUG
					uprintf("Combo [%u]: %u\n", j, comboKeys[j]);
#endif

				for (int j = 0; (j < COMBO_MAX) && (comboKeys[j] != COMBO_END); j++) {
#ifndef NO_DEBUG
					uprintf("Combo [%u]: %u\n", j, comboKeys[j]);
#endif
					SEND(comboKeys[j]);
				}
			}
			return cChord;
		}
	}

	// functions
	for (int i = 0; i < funcsLen; i++) {
		if (funDict[i].chord == cChord) {
			if (!lookup) funDict[i].act();
			return cChord;
		}
	}

	// Special handling
	for (int i = 0; i < specialLen; i++) {
		if (spcDict[i].chord == cChord) {
			if (!lookup) {
				uint16_t arg = spcDict[i].arg;
				switch (spcDict[i].action) {
					case SPEC_STICKY:
						SET_STICKY(arg);
						break;
					case SPEC_REPEAT:
						REPEAT();
						break;
					case SPEC_CLICK:
						CLICK_MOUSE((uint8_t)arg);
						break;
					case SPEC_SWITCH:
						SWITCH_LAYER(arg);
						break;
					default:
						SEND_STRING("Invalid Special in Keymap");
				}
			}
			return cChord;
		}
	}

#ifndef NO_DEBUG
	uprintf("Reached end\n");
#endif
	return 0;
}
// Traverse the chord history to a given point
// Returns the mask to use
void processChord(void) {
	// Save the clean chord state
	C_SIZE savedChord = cChord;

	// Apply Stick Bits if needed
	if (stickyBits != 0) {
		cChord |= stickyBits;
		for (int i = 0; i <= chordIndex; i++)
			chordState[i] |= stickyBits;
	}

	// First we test if a whole chord was passsed
	// If so we just run it handling repeat logic
	if (mapKeys(true) == cChord) {
		mapKeys(false);
		// Repeat logic
		if (repeatFlag) {
#ifndef NO_DEBUG
			uprintf("repeating?\n");
#endif
			restoreState();
			repeatFlag = false;
			processChord();
		} else {
			saveState(cChord);
		}
		return;
	}
#ifndef NO_DEBUG
	uprintf("made it past the maw");
#endif

	// Iterate through chord picking out the individual 
	// and longest chords
	C_SIZE bufChords[QWERBUF];
	int		 bufLen		= 0;
	C_SIZE mask		= 0;

	// We iterate over it multiple times to catch the longest
	// chord. Then that gets addded to the mask and re run.
	while (savedChord != mask) {
		C_SIZE		test				= 0;
		C_SIZE		longestChord	= 0;

		for (int i = 0; i <= chordIndex; i++) {
			cChord = chordState[i] & ~mask;
			if (cChord == 0)
				continue;

			test = mapKeys(true);
			if (test != 0) {
				longestChord = test;
			}
		}
		
		mask |= longestChord;
		bufChords[bufLen] = longestChord;
		bufLen++;

		// That's a loop of sorts, halt processing
		if (bufLen >= QWERBUF) {
#ifndef NO_DEBUG
			uprintf("looped. exiting");
#endif
			return;
		}
	}
	
	// Now that the buffer is populated, we run it
	for (int i = 0; i < bufLen ; i++) {
		cChord = bufChords[i];
#ifndef NO_DEBUG
		uprintf("sending: %x\n", cChord);
#endif
		mapKeys(false);
	}

	// Save state in case of repeat
	if (!repeatFlag) {			
		saveState(savedChord);
	}

	// Restore cChord for held repeat
	cChord = savedChord;
	return;
}
void saveState(C_SIZE cleanChord) {
	pChord = cleanChord;
	pChordIndex = chordIndex;
	for (int i = 0; i < 32; i++) 
		pChordState[i] = chordState[i];
}
void restoreState() {
	cChord = pChord;
	chordIndex = pChordIndex;
	for (int i = 0; i < 32; i++) 
		chordState[i] = pChordState[i];
}

// Macros for calling from keymap.c
void SEND(uint8_t kc) {
	// Send Keycode, Does not work for Quantum Codes
	if (cMode == COMMAND && CMDLEN < MAX_CMD_BUF) {
#ifndef NO_DEBUG
		uprintf("CMD LEN: %d BUF: %d\n", CMDLEN, MAX_CMD_BUF);
#endif
		CMDBUF[CMDLEN] = kc;
		CMDLEN++;
	} 

	if (cMode != COMMAND) register_code(kc);
	return;
}
void REPEAT(void) {
	if (cMode != QWERTY)
		return;

	repeatFlag = true;
	return;
}
void SET_STICKY(C_SIZE stick) {
	stickyBits ^= stick;
	return;
}
void CLICK_MOUSE(uint8_t kc) {
#ifdef MOUSEKEY_ENABLE
	mousekey_on(kc);
	mousekey_send();

	// Store state for later use
	inMouse = true;
	mousePress = kc;
#endif
}
void SWITCH_LAYER(int layer) {
#ifndef NO_ACTION_LAYER
	if (keymapsCount >= layer) 
		layer_on(layer);
#endif
}
