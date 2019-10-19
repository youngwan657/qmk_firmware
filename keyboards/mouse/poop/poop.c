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
#include <LUFA/Drivers/Peripheral/SPI.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include <stdbool.h>
#include <string.h>
#include <print.h>
#include <stdio.h>
#include "wait.h"
#include "util.h"
#include "matrix.h"
#include "quantum.h"
#include "debug.h"
#include "debounce.h"
#include "poop.h"
#include "spi.h"
#include "print.h"
#include "spi.c"
#include "math.h"

#ifdef ENCODER_ENABLE
#  include "encoder.h"
#endif

// TODO: Move these options to keymap.c and import here, default if unset
// User Config Options
#define BALLANGLE 			degToRad(-20)  // Angle in radians to rotate orientation
#define BALLSENS				1;					 // Factor to scale raw read input by

// Compile time accel selection
// Valid options are ACC_NONE, ACC_LINEAR, ACC_CUSTOM, ACC_QUADRATIC
#define PROFILE_NONE

// TODO: Implement libinput profiles
// https://wayland.freedesktop.org/libinput/doc/latest/pointer-acceleration.html

// Trackball State
bool 				BurstState 	= false; 	// init burst state for Trackball module
bool 				DragLock 		= false;  // Are we scrolling?
uint16_t 		MotionStart = 0;			// Timer for accel, 0 is resting state

// Transform Selection
static void process_mouse(bool bMotion, bool* bBurst) {
	// Read state
	PMWState 	d 		= point_burst_read(bMotion, bBurst);
	int				delta	= (int)(abs((double)d.X) + abs((double)d.Y));

	// Reset timer if stopped moving
	if (delta == 0) {
		MotionStart = 0;
		return;
	} 

	// Set timer if new motion
	if ((MotionStart == 0) && (delta != 0)) {
		uprintf("Starting motion.\n");
		MotionStart = timer_read();
	}

	uprintf("Delt] d: %d t: %u\n", delta, MotionStart);
	uprintf("Pre ] X: %d, Y: %d\n", d.X, d.Y);

	// Use integer math for speed
	// Rotate around origin
	//double x = ((double)d.X)*cos(BALLANGLE) - ((double)d.Y)*sin(BALLANGLE);
	//double y = ((double)d.X)*sin(BALLANGLE) + ((double)d.Y)*cos(BALLANGLE);
	int16_t x = (d.X)*(1000*cos(BALLANGLE)) - (d.Y)*(1000*sin(BALLANGLE));
	int16_t y = (d.X)*(1000*sin(BALLANGLE)) + (d.Y)*(1000*cos(BALLANGLE));
	uprintf("Rot ] X: %d, Y: %d\n", x, y);

	// Scale
	x = x*BALLSENS;
	y = y*BALLSENS;

	uprintf("Scal] X: %d, Y: %d\n", x, y);

	// Apply transform
	//uint16_t elapsed = timer_elapsed(MotionStart);
	#ifdef PROFILE_NONE
	#endif
	#ifdef PROFILE_LINEAR
	#endif
	#ifdef PROFILE_INVERSE
	#endif
	#ifdef PROFILE_CUSTOM
		// TODO: Function call to keymap.c
	#endif

	// Return to normal int space
	x = x/1000;
	y = y/1000;

	// Wrap to HID size
	x = constrain(x, -127, 127);
	y = constrain(y, -127, 127);
	uprintf("Cons] X: %d, Y: %d\n", x, y);
 	//uprintf("Elapsed:%u, X: %f Y: %\n", i, pgm_read_byte(firmware_data+i));

  report_mouse_t currentReport = pointing_device_get_report();
  if (bMotion) {
    currentReport.x = x;
    currentReport.y = y;
  } else {
    currentReport.v = x;
    currentReport.h = y;
  }
  pointing_device_set_report(currentReport);
}

// This is the firmware that is uploaded to the PMW3360 each time the mouse starts up.
// Don't mess with this. Just...just trust me. Don't do it.
// "We fuckin' with it. " -- Germ
//
// Hardware Setup
void keyboard_pre_init_kb(void) {
  debug_enable = true;
  debug_matrix = true;
  debug_mouse  = true;

	// Set up all the hardware
	setPinOutput(SENSOR_CS);

	// These should probably be moved into the matrix itself
	// using DIRECT_PINS, and then custom keycodes added for the
	// mouse buttons.
	setPinInputHigh(MOUSE_LEFT_PIN);
	setPinInputHigh(MOUSE_RIGHT_PIN);
	setPinInputHigh(MOUSE_MIDDLE_PIN);
	setPinInputHigh(MOUSE_BACK_PIN);
	setPinInputHigh(MOUSE_FORWARD_PIN);

	// This can probably be replaced with rotary encoder config,
	setPinInput(OPT_ENC_PIN1);
	setPinInput(OPT_ENC_PIN2);

	// This is the debug LED.
	setPinOutput(F7);
	if (debug_enable) {
			writePinHigh(F7);
	} else {
			writePinLow(F7);
	}

	/* Ground all output pins connected to ground. This provides additional
	 * pathways to ground. If you're messing with this, know this: driving ANY
	 * of these pins high will cause a short. On the MCU. Ka-blooey.
	 */
	setPinOutput(D3);
	writePinLow(D3);
	setPinOutput(D1);
	writePinLow(D1);
	setPinOutput(B4);
	writePinLow(B4);
	setPinOutput(B6);
	writePinLow(B6);
	setPinOutput(B7);
	writePinLow(B7);
	setPinOutput(D6);
	writePinLow(D6);
	setPinOutput(C7);
	writePinLow(C7);
	setPinOutput(F6);
	writePinLow(F6);
	setPinOutput(F5);
	writePinLow(F5);
	setPinOutput(F3);
	writePinLow(F3);


  // Initialize SPI for MCU
  SPI_Init(SPI_OPTION);
  DDRB |= (1 << SS_TB);
  PORTB |= (1 << SS_TB);  // pull up to diable all comm
	point_init(SS_TB);
}
void matrix_scan_kb(void) {
  process_mouse(Motion, &BurstState);
}
