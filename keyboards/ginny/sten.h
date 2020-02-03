/* 	2019, g Heavy Industries
	 	Blessed mother of Christ, please keep this readable
 		and protect us from segfaults. For thine is the clock,
 		the slave and the master. Until we return from main.

 		Amen.

		This is a stripped down version of the Georgi engine meant for use with
 		Ginni. As such serial-Steno features are disabled, chords are 16bits and
 		crap is removed where possible
 
 		Do not use this on anything other then Ginny if you want to be sane. Modify
		the Georgi engine instead!
*/

#include QMK_KEYBOARD_H
#include "keymap_steno.h"
#include "mousekey.h"
#include "keymap.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "wait.h"

// In memory chord datatypes
struct funcEntry {
	uint16_t	chord;
	void 			(*act)(void);
} dictEntry_t;

struct stringEntry {
	uint16_t	chord;
	char			str[];
} stringEntry_t;

struct keyEntry {
	uint16_t	chord;
	uint8_t		keys[];
} keyEntry_t;

extern struct 	dictEntry test[];
extern uint16_t cChord;				// Current Chord

// Function defs
void 			processChord(void);
uint16_t	processQwerty(bool lookup);
uint16_t 	processFakeSteno(bool lookup);
void 			saveState(uint16_t cChord);
void 			restoreState(void);

// Macros for use in keymap.c
void 			SEND(uint8_t kc);
void 			REPEAT(void);
void 			SET_STICKY(uint16_t);
void 			SWITCH_LAYER(int);
void 			CLICK_MOUSE(uint8_t);

// Needed for hashing
/*#define H1(s,i,x)   (x*65599u+(uint8_t)s[(i)<strlen(s)?strlen(s)-1-(i):strlen(s)])
#define H4(s,i,x)   H1(s,i,H1(s,i+1,H1(s,i+2,H1(s,i+3,x))))
#define H16(s,i,x)  H4(s,i,H4(s,i+4,H4(s,i+8,H4(s,i+12,x))))
#define H64(s,i,x)  H16(s,i,H16(s,i+16,H16(s,i+32,H16(s,i+48,x))))
#define H256(s,i,x) H64(s,i,H64(s,i+64,H64(s,i+128,H64(s,i+192,x))))
#define HASH(s)    ((uint32_t)(H256(s,0,0)^(H256(s,0,0)>>16)))*/

#define PASTER(x,y) x ## _ ## y
#define EVALUATOR(x,y)  PASTER(x,y)
#define NAME(arg) EVALUATOR(fn, arg)

// X Macros for keymap definition
#define P_ACTION(chord, act) 	void NAME(chord) (void) { act; }			
#define P_KEYMAP(chord, act)	{chord, NAME(chord)}

// Keymap helper
//#define P(chord, act) if (cChord == (chord)) { if (!lookup) {act;} return chord;}*/
#define REBOOT() wdt_enable(WDTO_250MS); for (;;) ;

// New Approach, multiple structures
P_KEYMAP(chord, keycode) 
M_KEYMAP(chord, keycodes) 
S_KEYMAP(chord, string)
F_KEYMAP(chord, name, func)
F_ACTION(chord, name, func)


// Shift to internal representation
// i.e) S(teno)R(ight)F
#define STN(n) (1u<<n)
enum ORDER { 
		SLSU= 0, SLSD, SLT, SLK, SLP, SLW, SLH, SLR, SST1, SST2,
		SRES1, SRES2, SST3, SST4
};


// Ginny Define
#define GLP STN(SLSU)
#define GLR STN(SLSD)
#define GLM STN(SLT)
#define GLI STN(SLK)
#define GLT STN(SLP)

#define GRT STN(SLW)
#define GRI STN(SLH)
#define GRM STN(SLR)
#define GRR STN(SST1)
#define GRP STN(SST2)
#define RES1 STN(SRES1) // Use reserved for sticky state
#define RES2 STN(SRES2)
#define RES3 STN(SRES3)

// Original 32bit version
#define LSU STN(SLSU)
#define LSD STN(SLSD)
#define LFT STN(SLT)  	
#define LK  STN(SLK)
#define LP  STN(SLP)
#define LW  STN(SLW)
#define LH  STN(SLH)
#define LR  STN(SLR)
#define ST1 STN(SST1)
#define ST2 STN(SST2)
#define ST3 STN(SST3)
#define ST4 STN(SST4)

// Asetniop aliases, to Ginny Fingers
#define AA		GLP
#define AS		GLR
#define AE		GLM
#define AT		GLI
#define AN		GRI
#define AI		GRM
#define AO		GRR
#define AP		GRP

#define AL		GLT							// Left/Right thumbs
#define AR		GRT

#define NUM		RES1						// Sticky Layer 1
#define USR   RES2						// Sticky Layer 2
#define CMD		RES2 | RES1			// Sticky Layer 3
