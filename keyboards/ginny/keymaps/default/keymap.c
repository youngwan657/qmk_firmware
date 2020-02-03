#include QMK_KEYBOARD_H
#include "sten.h"
#include "keymap_steno.h"

/* Note: Don't edit this file! 
 *
 * You can tweak what dictionaries/languages are loaded in dicts.def
 * Your personal keymap is over in user.def
 *
 * This is the most non-QMK powered device ever :)
 * Happy Hacking,
 *		-- Germ
 */

/*
#define P BLANK
#define K BLANK
#define S BLANK
#define F F_ACTION
	#include "dicts.def"
#undef P */

// Set up Macros for QMK Keys
#define PRES P_KEYMAP			
#define KEYS K_KEYMAP
#define SUBS BLANK
#define EXEC BLANK
struct keyEntry keyDict[] = {
	#include "dicts.def"
	{}

#undef  P
#undef  K
#define P BLANK
#define K BLANK

// Set up Macros for SEND_STRINGS
// Set up Macros for function calls
//
//struct funcEntry		funDict[] = {}
//struct stringEntry	strDict[] = {}

// Stub to be removed
uint16_t processQwerty(bool lookup) {
	return 0;
}
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[0] = LAYOUT_ginny(
		STN_S1, STN_S2, STN_TL, STN_KL, STN_PL, STN_WL, STN_HL, STN_RL, STN_ST1, STN_ST2
)};
