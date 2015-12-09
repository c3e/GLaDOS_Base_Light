#include "functions.h"

void matrix_init() {

//#if !defined FASTLED_VERSION
//	Timer1.initialize(5000);					// initialize timer1, and set a 3 milli second period
//#endif
	Timer1.attachInterrupt(updateDisplay);		// attaches callback() as a timer overflow interrupt

	pinMode(latch_pin, OUTPUT);
	pinMode(clock_pin, OUTPUT);
	pinMode(data_pin, OUTPUT);

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 11; j++) {
			matrix[i][j] = false;
		}
	}
}

void updateDisplay() {
	iteration++;
	displayMatrix();
}

void configure_pinning(uint8_t new_latch_pin, uint8_t new_clock_pin, uint8_t new_data_pin) {
	latch_pin	= new_latch_pin;
	clock_pin	= new_clock_pin;
	data_pin	= new_data_pin;
}

void shift_Matrix_left(int delay_iteration, int ammount) {

	for (int counter = 0; counter < ammount; counter++) {
		// Copy the first element into a buffer
		for (int i = 0; i < MATRIX_HEIGHT; i++) {
			matrix_slice[i][0] = matrix[i][0];
		}
		// Shift all elements left
		for (int i = 0; i < MATRIX_HEIGHT; i++) {
			for (int j = 0; j < MATRIX_WIDTH+MATRIX_BUFFER-1; j++) {
				matrix[i][j] = matrix[i][j + 1];
			}
		}
		// Copy the last element from the buffer
		for (int i = 0; i < MATRIX_HEIGHT; i++) {
			matrix[i][MATRIX_WIDTH+MATRIX_BUFFER-1] = matrix_slice[i][0];
		}
		delay(delay_iteration);
	}
}

void displayMatrix() {
	int i = 0, j = 0;
	uint16_t shift_out = BLANK;

	for (i = 0; i < MATRIX_HEIGHT; i++) {
		for (j = 0; j < MATRIX_WIDTH; j++) {
			if (matrix[i][j]) {
				// Both offsets derivate from the used hardware and
				// wiring. Only change these if you change the former!
				shift_out |=	1 << (4 - i);	// Map the appropriate Row
				shift_out &= ~( 1 << (15 - j));	// Map the appropriate Column
			}
		}
		shift(shift_out);		// Shift out one row
		shift_out = BLANK;		// Initialise next row
	}
	shift(shift_out);
}

void shift(uint16_t input) {
	// My own implementation of shiftOut(), tailored to the particular needs of this application only
	for (uint16_t bit_mask = 32768; bit_mask > 0; bit_mask = bit_mask / 2)
	{
		if (bit_mask & input) {				// Begin with MSB and approach LSB with each iteration
			digitalWrite(data_pin, HIGH);   // Set the output for shift-register HIGH
		}
		else {
			digitalWrite(data_pin, LOW);	// Set the output for shift-register LOW
		}

		digitalWrite(clock_pin, LOW);		// create a rising edge on the shift-registers SCKL Pin
		digitalWrite(clock_pin, HIGH);		// -> shift 1 bit into the registers storage
	}

	digitalWrite(latch_pin, HIGH);			// Clock the collected data into the storage register to make it visible	
	digitalWrite(latch_pin, LOW);			// (falling edge is needed)
	digitalWrite(data_pin, LOW);			// Take Data Pin low just to be sure.
}

int insertLetter(int letter, int delay_ms) {

	uint32_t current_letter = ascii[letter];
	uint8_t current_nibble = 0;
	// Start with 1, otherwise SPACE will not be displayed as a space.
	uint8_t max_width = 1;
	uint8_t offset = (LETTER_HEIGHT-1) * LETTER_WIDTH;

	// Calculate max_width
	for (int i = 0; i < LETTER_HEIGHT; i++) {
		current_nibble = (current_letter >> (offset - (LETTER_WIDTH * i))) & 0x1f;
		for (int j = 0; j < LETTER_WIDTH; j++) {
			if ((current_nibble & (1 << j)) && max_width <= j) {
				// j starts with 0, max_width starts with 1
				max_width = j+1;
			}
		}
	}
	shift_Matrix_left(delay_ms, max_width);

	// Write letter into Matrix
	for (int i = 0; i < LETTER_HEIGHT; i++){
		current_nibble = (current_letter >> (offset - (LETTER_WIDTH * i))) & 0x1f;

		for (int j = 0; j < LETTER_WIDTH; j++)
		{	
			if (j < LETTER_WIDTH - max_width) {
				// empty space in front of the current letter, don't write anything into the matrix
			}
			else {
				// Bits representing the pixels of each letter, make sure to write this into the matrix
				matrix[i][MATRIX_WIDTH - 1 + j] = (current_nibble >> (LETTER_WIDTH - 1 - j)) & 0x01;
			}
		}
	}
	
	return max_width;
}

void insertSpace() {
	for (int i = 0; i < MATRIX_HEIGHT; i++)
	{
		matrix[i][MATRIX_WIDTH+MATRIX_BUFFER-1] = 0;
	}
}

void displayMessage(char* message, int delay_iteration) {
	int i = 0;
	uint8_t letter_width = 0;

		//Serial.println("Trying to print Message.");
		while (message[i] != 0)
		{

			insertLetter(message[i], delay_iteration);
			insertLetter(36, delay_iteration);
			i++;
		}
}