/* 
 * Good on you for modifying your layout, this is the most nonQMK layout you will come across
 * There are three modes, Steno (the default), QWERTY (Toggleable) and a Momentary symbol layer
 *
 * Don't modify the steno layer directly, instead add chords using the keycodes and macros
 * from sten.h to the layout you want to modify.
 *
 * Observe the comment above processQWERTY!
 */

#include QMK_KEYBOARD_H
#include "keymap_steno.h"
#include "sten.h"
#define IGNORE_MOD_TAP_INTERRUPT

// "Layers"
#define FUNCT   (RR | RG | RB | RS)
#define MEDIA   (LS | LK | LW | LR)
#define MOVE    (ST1 | ST2)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Steno layer
 * FN1:			Qwerty Toggle (processQwerty)
 * PWR:			Symbol Momentary (Combo with press, processSymbol)
 * FN1+PWR:		Command Mode
 *
 * ,-------------------------.    ,-----------------------.
 * | FN1 | S | T | P | H |ST1|    |ST3| F | P | L | T | D |
 * |-----+---+---+---+---|---|    |---|---+---+---+---+---|
 * | PWR | S | K | W | R |ST2|    |ST4| R | G | B | S | Z |
 * `-------------------------'    `-----------------------'
 *               ,-----------,    .-----------.
 *               | # | A | O |    | E | U | # |
 *               `-----------'    `-----------'
 */
[0] = LAYOUT_GEORGI(  
STN_FN,  STN_S1, STN_TL, STN_PL, STN_HL, STN_ST1,       STN_ST3, STN_FR, STN_PR, STN_LR, STN_TR, STN_DR,
STN_PWR, STN_S2, STN_KL, STN_WL, STN_RL, STN_ST2,       STN_ST4, STN_RR, STN_BR, STN_GR, STN_SR, STN_ZR,
						 STN_N1, STN_A,  STN_O,			STN_E,   STN_U,  STN_N1)
};

// YOU MUST ORDER THIS!
// P   Will return from processing on the first match it finds. Therefore
// PJ  Will run the requested action, remove the matched chord and continue 
// First any mods should be checked using PJ !
bool processQwerty(void) {
	// P that PJ interferes with, Chords placed here will never
	// be shifted, alt'd, etc
	P( LA | LO,							SEND(KC_LALT));

	// Mods 
	PJ( RT | RD | RS | RZ,				SEND(KC_LGUI));
	PJ( RT | RD,						SEND(KC_LCTL));
	PJ( RS | RZ,						SEND(KC_LALT));
	PJ( LA | NUM,						SEND(KC_LCTRL));
	PJ( LO,								SEND(KC_LSFT));

	// Function Layer 
	P( FUNCT | RF | RR,					SEND(KC_F5));
	P( FUNCT | RP | RG,					SEND(KC_F6));
	P( FUNCT | RL | RB,					SEND(KC_F7));
	P( FUNCT | RT | RS,					SEND(KC_F8));
	P( FUNCT | RF,						SEND(KC_F1));
	P( FUNCT | RP,						SEND(KC_F2));
	P( FUNCT | RL,						SEND(KC_F3));
	P( FUNCT | RT,						SEND(KC_F4));
	P( FUNCT | RR,						SEND(KC_F9));
	P( FUNCT | RG,						SEND(KC_F10));
	P( FUNCT | RB,						SEND(KC_F11));
	P( FUNCT | RS,						SEND(KC_F12));

	// Movement Layer
	P( MOVE | RF,						SEND(KC_LEFT));
	P( MOVE | RF,						SEND(KC_LEFT));
	P( MOVE | RP,						SEND(KC_DOWN));
	P( MOVE | RL,						SEND(KC_UP));
	P( MOVE | RT,						SEND(KC_RIGHT));
	P( MOVE | ST3,						SEND(KC_PGUP));
	P( MOVE | ST4,						SEND(KC_PGDN));

	// Mouse Keys
	P( LP | LH,							clickMouse(KC_MS_BTN1));
	P( LW | LR,							clickMouse(KC_MS_BTN2));
	
	// Number Row
	P( NUM | LSU,						SEND(KC_1));
	P( NUM | LFT,						SEND(KC_2));
	P( NUM | LP,						SEND(KC_3));
	P( NUM | LH,						SEND(KC_4));
	P( NUM | ST1,						SEND(KC_5));
	P( NUM | ST3,						SEND(KC_6));
	P( NUM | RF,						SEND(KC_7));
	P( NUM | RP,						SEND(KC_8));
	P( NUM | RL,						SEND(KC_9));
	P( NUM | RT,						SEND(KC_0));
	P( NUM | LA,						SEND(KC_5));
	P( NUM | RT,						SEND(KC_0));
	
	// Specials
	P( LA | NUM,						SEND(KC_ESC));
	P( RU | NUM,						SEND(KC_TAB));
	P( RE | RU,							SEND(KC_BSPC));
	P( RD | RZ,							SEND(KC_ENT));
	P( RE,								SEND(KC_ENT));
	P( RD,								SEND(KC_BSPC));	
	P( NUM,								SEND(KC_BSPC));
	P( LA,								SEND(KC_SPC));
	P( RU,								SEND(KC_SPC));
	P( RZ,								SEND(KC_ESC));

	// Letters
	P( LSU | LSD,						SEND(KC_A));
	P( LFT | LK,						SEND(KC_S));
	P( LP  | LW,						SEND(KC_D));
	P( LH  | LR,						SEND(KC_F));
	P( ST1 | ST2,						SEND(KC_G));
	P( ST3 | ST4,						SEND(KC_H));
	P( RF  | RR,						SEND(KC_J));
	P( RT  | RS,						SEND(KC_SCLN))
	P( RG  | RL,						SEND(KC_L));
	P( RP  | RB,						SEND(KC_K));
	P( LSU,								SEND(KC_Q));
	P( LSD,								SEND(KC_Z));
	P( LFT,								SEND(KC_W));
	P( LK,								SEND(KC_X));
	P( LP,								SEND(KC_E));
	P( LW,								SEND(KC_C));
	P( LH,								SEND(KC_R));
	P( LR,								SEND(KC_V));
	P( ST1,								SEND(KC_T));
	P( ST2,								SEND(KC_B));
	P( ST3,								SEND(KC_Y));
	P( ST4,								SEND(KC_N));
	P( RF,								SEND(KC_U));
	P( RR,								SEND(KC_M));
	P( RP,								SEND(KC_I));
	P( RB,								SEND(KC_COMM));
	P( RL,								SEND(KC_O));
	P( RG,								SEND(KC_DOT));
	P( RT,								SEND(KC_P));
	P( RS,								SEND(KC_SLSH));

	// Symbols and Numbers
	P( PWR | RE | RU,					SEND(KC_ENT));
	P( PWR | LA | LO,					SEND(KC_SPC));
	P( PWR | LP | LW,					SEND(KC_LSFT); SEND(KC_9));			// (
	P( PWR | LH | LR,					SEND(KC_LSFT); SEND(KC_0));			// )
	P( PWR | ST1 | ST2,					SEND(KC_GRV));						// `
	P( PWR | RD | RZ,					SEND(KC_ESC));							
	P( PWR | LSU | LSD,					SEND(KC_LSFT); SEND(KC_3));			// #
	P( PWR | LFT | LK,					SEND(KC_LSFT); SEND(KC_4));			// $
	P( PWR | LSU,						SEND(KC_LSFT); SEND(KC_1));			// !
	P( PWR | LSD,						SEND(KC_LSFT); SEND(KC_5));			// %
	P( PWR | LFT,						SEND(KC_LSFT); SEND(KC_2));			// @
	P( PWR | LK,						SEND(KC_LSFT); SEND(KC_6));			// ^
	P( PWR | LP,						SEND(KC_LSFT); SEND(KC_LBRC));		// {
	P( PWR | LW,						SEND(KC_LBRC));
	P( PWR | LH,						SEND(KC_LSFT); SEND(KC_RBRC));		// }
	P( PWR | LR,						SEND(KC_RBRC));
	P( PWR | ST1,						SEND(KC_LSFT); SEND(KC_BSLS));		// |
	P( PWR | ST2,						SEND(KC_LSFT); SEND(KC_GRV));		// ~
	P( PWR | ST3,						SEND(KC_QUOT));
	P( PWR | ST4,						SEND(KC_LSFT); SEND(KC_QUOT));		// "
	P( PWR | RF,						SEND(KC_KP_PLUS));
	P( PWR | RR,						SEND(KC_LSFT); SEND(KC_7));			// &
	P( PWR | RP,						SEND(KC_MINS));
	P( PWR | RB,						SEND(KC_EQL));
	P( PWR | RL,						SEND(KC_SLSH));
	P( PWR | RG,						SEND(KC_COMM));
	P( PWR | RT,						SEND(KC_PAST));
	P( PWR | RS,						SEND(KC_DOT));
	P( PWR | RD,						SEND(KC_TAB));
	P( PWR | LA,						SEND(KC_SCLN));
	P( PWR | LO,						SEND(KC_SLSH));
	P( PWR | RE,						SEND(KC_SCLN));
	P( PWR | RU,						SEND(KC_SLSH));

	// Version Debug / MISC
	P( RT  | RS  | RT  | RZ | NUM,		SEND_STRING(VERSION); SEND_STRING(__DATE__));
	P( NUM | LA  | LO  | RE | RU,		SEND(KC_MPLY));
	P( ST1 | ST2 | ST3 | ST4,			SEND(KC_BSPC));

	// If we make here, send as a steno chord
	// If plover is running we can hook that host side
	return false;
}

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
  steno_set_mode(STENO_MODE_GEMINI); // or STENO_MODE_BOLT
};
