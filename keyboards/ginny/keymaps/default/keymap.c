#include QMK_KEYBOARD_H

#include "sten.h"

// This is what dictionaries are currently loaded to your device
// You must add the dictionary to both parts of the keymap
// For configuring your device, got edit user.def :)

// ASETNIOP supports various languages, define the one you wish to include here
// da, de, en, en-colemak, en-dvorak, es, fn, fr, hu, it, nl, nw, pl, pt, ro, sv, tr

#define P P_KEYMAP			\
const dictEntry steno_keymap[] = {
	"gen/aset/layer-keymap.c"
	"gen/aset/cmd-keymap.c"
	"gen/aset/num-keymap.c"
	"gen/aset/en-keymap.c"
	"user.def"
}
#undef P

#define P P_ACTION
"gen/aset/layer-keymap.c"
"gen/aset/cmd-keymap.c"
"gen/aset/num-keymap.c"
"gen/aset/en-keymap.c"
"user.def"
#undef P

uint16_t processQwerty(bool lookup) {
	return 0;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[0] = LAYOUT_ginny(
		STN_S1, STN_S2, STN_TL, STN_KL, STN_PL, STN_WL, STN_HL, STN_RL, STN_ST1, STN_ST2
)};
