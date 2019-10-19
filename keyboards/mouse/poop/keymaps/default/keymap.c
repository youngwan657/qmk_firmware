/* Copyright 2019 Drashna Jael're (@drashna)
 * Copyright 2019 Sunjun Kim
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include "sten.h"
#include "keymap_steno.h"

/* Keymap Definintions
 * Order your chords from longest to shortest!
 * docs.gboards.ca
 * /--------------------\   /------|------\
 * |  LMB  |MMB|   RMB  |   | FN1  | FN2  |
 * \--------------------/   \------|------/ */
uint32_t processQwerty(bool lookup) {
    // Mouse Keys
    P( FN1 | FN2,			SEND_STRING("Hello QMK!"));
    P( LMB,  	 				CLICK_MOUSE(KC_MS_BTN1));
    P( RMB,    				CLICK_MOUSE(KC_MS_BTN2));
    P( MMB,    				CLICK_MOUSE(KC_MS_BTN3));
    P( FN1,    				CLICK_MOUSE(KC_MS_BTN4));
    P( FN2,    				CLICK_MOUSE(KC_MS_BTN5));

		return 0;
}

/* 0: TEMPLATE
 * /---------------------\   /------|------\
 * |       |    |        |   |      |      |
 * \---------------------/   \------|------/
 */

// This is the "proper" qmk keymap, don't edit this unless 
// you know what you're doing. You can define a chord to 
// jump to higher layers and then return to 0 with DF()
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(TB_LMB, TB_RMB, TB_MMB, TB_FN1, TB_FN2),
};

// Don't fuck with this, thanks. It's needed for the engine
size_t keymapsCount  = sizeof(keymaps)/sizeof(keymaps[0]);
