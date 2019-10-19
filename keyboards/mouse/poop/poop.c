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


// TODO: Implement libinput profiles
// https://wayland.freedesktop.org/libinput/doc/latest/pointer-acceleration.html
// Compile time accel selection
// Valid options are ACC_NONE, ACC_LINEAR, ACC_CUSTOM, ACC_QUADRATIC
#define PROFILE_NONE

// Debug Options
#define DEBUGMOUSE					false 	// Slows down scan rate!

// Trackball State
bool 					BurstState 	= false; 	// init burst state for Trackball module
bool 					DragLock 		= false;  // Are we scrolling?
uint16_t 			MotionStart = 0;			// Timer for accel, 0 is resting state

// Transform Selection
static void process_mouse(bool bMotion, bool* bBurst) {
	// Read state
	PMWState 	d 		= point_burst_read(bMotion, bBurst);
	bool isMoving = (d.X != 0) || (d.Y != 0);
	// Reset timer if stopped moving
	if (!isMoving) {
		if (MotionStart != 0) MotionStart = 0;
		return;
	} 

	// Set timer if new motion
	if ((MotionStart == 0) && isMoving) {
		if (DEBUGMOUSE) uprintf("Starting motion.\n");
		MotionStart = timer_read();
	}

	if (DEBUGMOUSE) uprintf("Delt] d: %d t: %u\n", abs(d.X) + abs(d.Y), MotionStart);
	if (DEBUGMOUSE) uprintf("Pre ] X: %d, Y: %d\n", d.X, d.Y);

	// Wrap to HID size
	int16_t x = constrain(d.X, -127, 127);
	int16_t y = constrain(d.Y, -127, 127);
	if (DEBUGMOUSE) uprintf("Cons] X: %d, Y: %d\n", x, y);
 	//uprintf("Elapsed:%u, X: %f Y: %\n", i, pgm_read_byte(firmware_data+i));

  report_mouse_t currentReport = pointing_device_get_report();
  if (bMotion) {
    currentReport.x = (int)x;
    currentReport.y = (int)y;
  } else {
    currentReport.v = (int)x;
    currentReport.h = (int)y;
  }
  pointing_device_set_report(currentReport);
}

// Hardware Setup
void keyboard_pre_init_kb(void) {
  debug_enable = false;
  debug_matrix = false;
  debug_mouse  = false;

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
