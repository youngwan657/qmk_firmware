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
#include "print.h"
#include "spi.c"

#ifdef ENCODER_ENABLE
#  include "encoder.h"
#endif

// This is the firmware that is uploaded to the PMW3360 each time the mouse starts up.
// Don't mess with this. Just...just trust me. Don't do it.
// "We fuckin' with it. " -- Germ

#define SPI_OPTION (SPI_SPEED_FCPU_DIV_2 | SPI_SCK_LEAD_FALLING | SPI_SAMPLE_TRAILING | SPI_ORDER_MSB_FIRST | SPI_MODE_MASTER)
#define MOUSE_LEFT_PIN    D4
#define MOUSE_RIGHT_PIN   E6
#define MOUSE_MIDDLE_PIN  D2
#define MOUSE_BACK_PIN    B5
#define MOUSE_FORWARD_PIN D7

// For info, see https://forum.arduino.cc/index.php?topic=241369.0
#define SENSOR_CS B0
#define OPT_ENC_PIN1 F4
#define OPT_ENC_PIN2 F2

// For Trackball
bool BurstState = false;  // init burst state for Trackball module
bool BoolMotion_TB1 = true;

/*
// Max recommended is 4,000,000, since clock speed is 8MHz.
// If lower, it should be by factors of 1/2.
#define SPIMAXIMUMSPEED 2000000
#define CPI 1200
#define SCROLL_DEBOUNCE 50
#define SCROLL_BUTT_DEBOUNCE 100
#define ROTATIONAL_TRANSFORM_ANGLE 20
#define OPT_LOW_THRESHOLD 150

static const uint8_t MOTION = 0x02;
static const uint8_t DELTA_X_L = 0x03;
static const uint8_t DELTA_X_H = 0x04;
static const uint8_t DELTA_Y_L = 0x05;
static const uint8_t DELTA_Y_H = 0x06;
static const uint8_t CONFIG1 = 0x0F;
static const uint8_t CONFIG2 = 0x10;
static const uint8_t SROM_ENABLE = 0x13;
static const uint8_t SROM_ID = 0x2A;
static const uint8_t POWER_UP_RESET = 0x3A;
static const uint8_t SHUTDOWN = 0x3B;
static const uint8_t MOTION_BURST = 0x50;
static const uint8_t SROM_LOAD_BURST = 0x62;
const uint16_t firmware_length = 4094;

void adnsComBegin(void) { writePinLow	(SENSOR_CS); }
void adnsComEnd(void) 	{ writePinHigh(SENSOR_CS); }
*/

void nopekeyboard_pre_init_kb(void) {
    // Not sure the purpose
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
    //
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

    // replace with ACTUAL SPI code
    // just in case make sure that it's AVR and SPI compatible
    // keyboards/crkbd/keymaps/vlukash_trackpad_right has good references for this

    //SPI_Init(SPI_SPEED_FCPU_DIV_2|SPI_ORDER_MSB_FIRST|SPI_SCK_LEAD_FALLING);

    //initialisePMW3360();

    keyboard_pre_init_user();
}
void keyboard_post_init_user(void) {
	debug_enable=true;
  debug_matrix=true;
  debug_keyboard=true;
  debug_mouse=true;
}

// Probably going to have to refactor
uint8_t adnsReadReg(uint8_t reg_addr) {
    adnsComBegin();

    // send adress of the register, with MSBit = 0 to indicate it's a read
    SPI_TransferByte(reg_addr & 0x7f);
    wait_us(35);  // tSRAD
    uint8_t data = SPI_TransferByte(0);

    // tSCLK-SENSOR_CS for read operation is 120ns
    wait_us(1);
    adnsComEnd();
    // tSRW/tSRR (=20us) minus tSCLK-SENSOR_CS
    wait_us(19);

    return data;
}
void initialisePMW3360(void) {
    if (debug_enable) {
        dprintf("Initialising PMW3360");
    }

    // Hard reset. Start by ensuring that the serial port is reset.
    adnsComEnd();
    adnsComBegin();
    adnsComEnd();

    // SHUTDOWN the PMW3360, in case the microcontroller reset but the sensor didn't.
    adnsWriteReg(SHUTDOWN, 0xb6);
    wait_ms(300);

    // Drop and raise SENSOR_CS to reset the spi port
    adnsComBegin();
    wait_us(40);
    adnsComEnd();
    wait_us(40);

    // Force a reset of the PMW3360.
    adnsWriteReg(POWER_UP_RESET, 0x5a);
    wait_ms(50);

    // Read registers 0x02 through 0x06 (and discard the data)
    adnsReadReg(MOTION);
    adnsReadReg(DELTA_X_L);
    adnsReadReg(DELTA_X_H);
    adnsReadReg(DELTA_Y_L);
    adnsReadReg(DELTA_Y_H);

    if (debug_enable) {
        dprintf("Uploading firmware to PMW3360");
    }

    // Write 0 to Rest_En bit of CONFIG2 register to disable Rest mode.
    adnsWriteReg(CONFIG2, 0x00);

    // write 0x1d in SROM_enable reg for initializing
    adnsWriteReg(SROM_ENABLE, 0x1d);

    // wait for more than one frame period
    // assume that the frame rate is as low as 100fps, even if it should never be that low
    wait_ms(10);

    // write 0x18 to SROM_enable to start SROM download
    adnsWriteReg(SROM_ENABLE, 0x18);

    // write burst destination adress
    adnsComBegin();
    SPI_TransferByte(SROM_LOAD_BURST | 0x80);
    wait_us(15);

    unsigned char c;
    for (uint8_t i = 0; i < (sizeof(firmware_data) / sizeof(char)); i++) {
        c = (unsigned char)pgm_read_byte(firmware_data + i);
        SPI_TransferByte(c);
        wait_us(15);
    }

    // Read the SROM_ID register to verify the ID before any other register reads or writes.
    adnsReadReg(SROM_ID);

    // Write 0x00 (rest disable) to CONFIG2 register for wired mouse.
    // (If this were a wireless design, it'd be 0x20.)
    adnsWriteReg(CONFIG2, 0x00);

    adnsComEnd();

    if (debug_enable) {
        dprintf("Firmware successfully written to PMW3360");
    }

    wait_ms(10);

    setCPI(CPI);

    // start burst mode
    adnsWriteReg(MOTION_BURST, 0x00);

    if (debug_enable) {
        dprintf("PMW3360 successfully initialized");
    }
}
void adnsWriteReg(uint8_t reg_addr, uint8_t data) {
    adnsComBegin();

    // send adress of the register, with MSBit = 1 to indicate it's a write
    SPI_TransferByte(reg_addr | 0x80);
    SPI_TransferByte(data);

    // tSCLK-SENSOR_CS for write operation
    wait_us(20);
    adnsComEnd();
    // tSWW/tSWR (=120us) minus tSCLK-SENSOR_CS.
    // Could be shortened, but this looks like a safe lower-bound.
    wait_us(100);
}
void setCPI(int cpi) {
    int cpival = constrain((cpi / 100) - 1, 0, 0x77);  // limits to 0--119

    adnsComBegin();
    adnsWriteReg(CONFIG1, cpival);
    adnsComEnd();

    // if (debug_enable) {
    //     Serial.print(F("Got "));
    //     Serial.println(cpi);
    //     Serial.print(F("Set cpi to "));
    //     Serial.println((cpival + 1) * 100);
    // }
}



void matrix_init(void) {
  debug_enable = true;
  debug_matrix = true;
  debug_mouse  = true;

  // Initialize SPI for MCU
  SPI_Init(SPI_OPTION);
  DDRB |= (1 << SS_KBS) | (1 << SS_TB1) | (1 << SS_TB2);
  PORTB |= (1 << SS_KBS) | (1 << SS_TB1) | (1 << SS_TB2);  // pull up to diable all comm

  // Initialize Slaves
  point_init(SS_TB1);
  matrix_init_quantum();
}
uint8_t matrix_scan(void) {
  matrix_scan_quantum();
  //point_update_kb();
  mouse_report(SS_TB1, BoolMotion_TB1, &BurstState);

  return 1;
}
