#include QMK_KEYBOARD_H

/* Note: Don't edit this file! 
 *
 * You can tweak what dictionaries/languages are loaded in dicts.def
 * Your personal keymap is over in user.def
 *
 * This is the most non-QMK powered device ever :)
 * Happy Hacking,
 *		-- Germ
 */

#include "sten.h"

#define P P_ACTION
	#include "dicts.def"
#undef P

#define P P_KEYMAP			
struct dictEntry steno_keymap[] = {
	#include "dicts.def"
}
#undef P

#define P P_KEYMAP			
struct dictEntry steno_keymap[] = {
	#include "dicts.def"
}
#undef P


uint16_t processQwerty(bool lookup) {
	return 0;
}
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[0] = LAYOUT_ginny(
		STN_S1, STN_S2, STN_TL, STN_KL, STN_PL, STN_WL, STN_HL, STN_RL, STN_ST1, STN_ST2
)};
