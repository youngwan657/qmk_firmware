/* Good on you for modifying your layout! if you don't have
 * time to read the QMK docs, a list of keycodes can be found at
 *
 * https://github.com/qmk/qmk_firmware/blob/master/docs/keycodes.md
 *
 * There's also a template for adding new layers at the bottom of this file!
 */

#include QMK_KEYBOARD_H
#include "g/keymap_combo.h"

#define BASE 0 // default layer
#define NUMS 1
#define SYMS 2
#define MOUSE 3

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
    CMD_B = SAFE_RANGE,
    CMD_Z,
    CMD_X,
    CMD_C,
    CMD_V,
    CMD_SPACE,
    CTRL_SPACE,
    CTRL_GRAVE
};


/* Combomap
 *
 * ,-----------------------------.       ,--------------------------------.
 * |      |    ESC    |     |     |      |     |    ESC    |    BSLH      |
 * |-----+-----+-----+-----+------|      |--------------------------------|
 * |      |   BSPC   ENT    |     |      |    LES   COLN  GRT    |        | 
 * |-----+-----+-----+--RMB+-LMB--+      |--------------------------------|
 * |      |   MINS    |     |     |      |    QUO   UNDR   |     |        |
 * `------+-----+-----+------+----'	     `--------------------------------'
 *  .-------------------------.           .-----------------.   
 *  |        |       |        |           |        |    |   |
 *  '-------------------------'           '-----------------' 
 */

// Blank template at the bottom
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
// Keymap 0: Basic layer
[BASE] = LAYOUT_gergoplex(
    KC_Q, KC_W, KC_F, KC_P, KC_B,  	
	KC_J, KC_L, KC_U, KC_Y, KC_SCLN, 

    MT(MOD_LCTL, KC_A), MT(MOD_LALT, KC_R), MT(MOD_LGUI, KC_S), MT(MOD_LSFT, KC_T), KC_G,    
    KC_M, MT(MOD_RSFT, KC_N), MT(MOD_RGUI, KC_E), MT(MOD_RALT, KC_I), MT(MOD_LCTL, KC_O),    

    KC_Z, KC_X, KC_C, KC_D, KC_V,  	
	KC_K, KC_H, KC_COMM, KC_DOT, KC_SLSH, 

	MT(KC_HYPR, KC_TAB), LT(NUMS, KC_SPC), CTRL_GRAVE,
	CMD_SPACE, LT(SYMS, KC_ENT), KC_BSPC
),

// Keymap 1: Number layer
[NUMS] = LAYOUT_gergoplex(
    KC_1, KC_2, KC_3, KC_4, KC_5,
	KC_6, KC_7,	KC_8, KC_9, KC_0,

    MT(MOD_LCTL, KC_F1), MT(MOD_LALT, KC_F2), MT(MOD_LGUI, KC_F3), MT(MOD_LSFT, KC_F4), KC_F5,
	KC_LEFT, MT(MOD_RSFT, KC_DOWN), MT(MOD_RGUI, KC_UP), MT(MOD_RALT, KC_RGHT), MO(MOD_LCTL),

    KC_F6, KC_F7, KC_F8, KC_F9, KC_F10,
	KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,

	KC_TRNS,KC_TRNS,KC_TRNS,
  	MO(SYMS),KC_TRNS,KC_TRNS
),

// Keymap 2: Symbol layer
[SYMS] = LAYOUT_gergoplex(
    KC_AMPR, KC_PIPE, KC_LPRN, KC_RPRN, KC_PLUS,
    KC_TRNS, KC_EXLM, KC_AT, KC_TILD, KC_BSLS,

	MT(MOD_LCTL, KC_CIRC), MT(MOD_LALT, KC_DLR), MT(MOD_LGUI, KC_LCBR), MT(MOD_LSFT, KC_RCBR), KC_MINS,
	KC_PERC, MT(MOD_LSFT, KC_ASTR), MT(MOD_LGUI, KC_HASH), MT(MOD_LALT, KC_UNDERSCORE), MO(MOD_LCTL),

	KC_QUOT, KC_DQUO, KC_LBRC, KC_RBRC, KC_EQL,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,

	KC_TRNS, MO(MOUSE), KC_TRNS,   
	KC_TRNS, KC_TRNS, KC_TRNS
),

// Keymap 3: Mouse
[MOUSE] = LAYOUT_gergoplex(
    KC_BRID, KC_BRIU, KC_VOLD, KC_VOLU, KC_MUTE,
	KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,

	KC_TRNS, KC_TRNS, KC_ACL0, KC_ACL1, KC_ACL2,
	KC_WH_L, KC_WH_D, KC_WH_U, KC_WH_R, KC_BTN1,

	KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
	KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, KC_BTN2,

	KC_TRNS, KC_TRNS, KC_TRNS,
	KC_TRNS, KC_TRNS, KC_TRNS
)

};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {

    case CMD_B:
        if (record->event.pressed) {
            SEND_STRING(SS_LCMD("b"));
        }
        break;
    case CMD_Z:
        if (record->event.pressed) {
            SEND_STRING(SS_LCMD("z"));
        }
        break;
    case CMD_X:
        if (record->event.pressed) {
            SEND_STRING(SS_LCMD("x"));
        }
        break;
    case CMD_C:
        if (record->event.pressed) {
            SEND_STRING(SS_LCMD("c"));
        }
        break;
    case CMD_V:
        if (record->event.pressed) {
            SEND_STRING(SS_LCMD("v"));
        }
        break;
    case CMD_SPACE:
        if (record->event.pressed) {
            SEND_STRING(SS_LCMD(" "));
        }
        break;
    case CTRL_SPACE:
        if (record->event.pressed) {
            SEND_STRING(SS_LCTRL(" "));
        }
        break;
    case CTRL_GRAVE:
        if (record->event.pressed) {
            SEND_STRING(SS_LCTL("`"));
        }
        break;
	}
    return true;
};
