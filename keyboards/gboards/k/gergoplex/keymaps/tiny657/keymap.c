/* Good on you for modifying your layout! if you don't have
 * time to read the QMK docs, a list of keycodes can be found at
 *
 * https://github.com/qmk/qmk_firmware/blob/master/docs/keycodes.md
 *
 * There's also a template for adding new layers at the bottom of this file!
 */

#include QMK_KEYBOARD_H
#include "g/keymap_combo.h"

#define _BASE 0
#define _NUMBER 1
#define _SYMBOL 2
#define _MOUSE 3

// Blank template at the bottom
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

// Keymap 0: Basic layer
[_BASE] = LAYOUT_gergoplex(
	MT(MOD_LCTL, KC_Q), MT(MOD_LALT, KC_W), MT(MOD_LGUI, KC_E), MT(MOD_LSFT, KC_R), MT(MOD_LSFT, KC_T),  	
	MT(MOD_RSFT, KC_Y), MT(MOD_RSFT, KC_U), MT(MOD_RGUI, KC_I), MT(MOD_RALT, KC_O), MT(MOD_LCTL, KC_P), 

	MT(MOD_LCTL, KC_A), MT(MOD_LALT, KC_S), MT(MOD_LGUI, KC_D), MT(MOD_LSFT, KC_F), MT(MOD_LSFT, KC_G),    
	MT(MOD_RSFT, KC_H), MT(MOD_RSFT, KC_J), MT(MOD_RGUI, KC_K), MT(MOD_RALT, KC_L), MT(MOD_LCTL, KC_SCLN),    

	MT(MOD_LCTL, KC_Z), MT(MOD_LALT, KC_X), MT(MOD_LGUI, KC_C), MT(MOD_LSFT, KC_V), MT(MOD_LSFT, KC_B),  	
	MT(MOD_RSFT, KC_N), MT(MOD_RSFT, KC_M), MT(MOD_RGUI, KC_COMM), MT(MOD_RALT, KC_DOT), MT(MOD_LCTL, KC_SLSH), 

	KC_ESC, LT(_NUMBER, KC_SPC), KC_TAB,
	KC_ENT, LT(_SYMBOL, KC_BSPC), LGUI(KC_SPC)
),

// Keymap 1: Number layer
[_NUMBER] = LAYOUT_gergoplex(
	KC_1, KC_2, KC_3, KC_4, KC_5,
	KC_6, KC_7, KC_8, KC_9, KC_0,

	MT(MOD_LCTL, KC_F1), MT(MOD_LALT, KC_F2), MT(MOD_LGUI, KC_F3), MT(MOD_LSFT, KC_F4), KC_F5,
	KC_LEFT, MT(MOD_RSFT, KC_DOWN), MT(MOD_RGUI, KC_UP), MT(MOD_RALT, KC_RGHT), MO(MOD_LCTL),

	KC_F6, KC_F7, KC_F8, KC_F9, KC_F10,
	KC_HOME, KC_PGDN, KC_PGUP, KC_END, _______,

	_______, _______, _______,
	_______, MO(_MOUSE), _______
),

// Keymap 2: Symbol layer
[_SYMBOL] = LAYOUT_gergoplex(
	KC_EXLM, KC_AT, KC_LPRN, KC_RPRN, KC_PLUS,
	KC_GRAVE, LSFT(KC_GRAVE), KC_BSLS, LSFT(KC_BSLS), LSFT(KC_MINUS),

	KC_AMPR, KC_PIPE, KC_LCBR, KC_RCBR, KC_EQL,
	KC_CIRC, KC_ASTR, KC_HASH, KC_DLR, KC_PERC,

	KC_QUOT, KC_DQUO, KC_LBRC, KC_RBRC, KC_MINUS,
	_______, _______, _______, _______, _______,

	_______, MO(_MOUSE), _______,   
	_______, _______, _______
),

// Keymap 3: Mouse layer
[_MOUSE] = LAYOUT_gergoplex(
    KC_BRID, KC_BRIU, KC_VOLD, KC_VOLU, KC_MUTE,
	_______, _______, _______, _______, _______,

	_______, _______, KC_ACL0, KC_ACL1, KC_ACL2,
	KC_WH_L, KC_WH_U, KC_WH_D, KC_WH_R, KC_BTN1,

	_______, _______, _______, _______, _______,
	KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, KC_BTN2,

	_______, _______, _______,
	_______, _______, _______
),

};
