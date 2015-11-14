#ifndef GLOBAL_DEF_H
#define GLOBAL_DEF_H
#include <Arduino.h>

// Letters are stored as uint32_t, so make sure
// Letter_Height*Letter_Width <= 32
#define LETTER_WIDTH	5
#define LETTER_HEIGHT	5
#define MATRIX_HEIGHT	5
#define MATRIX_WIDTH	11
#define MATRIX_BUFFER	LETTER_WIDTH

extern bool matrix		[MATRIX_HEIGHT][MATRIX_WIDTH+MATRIX_BUFFER];
extern int	matrix_slice[MATRIX_HEIGHT][1];

extern int latch_pin;
extern int clock_pin;
extern int data_pin;

extern int iteration;

const uint32_t ascii[128] = {
	// Font can be calculated using the "LetterTool"
	#include "font.h"
};

#endif


