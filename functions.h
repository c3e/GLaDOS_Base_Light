#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Arduino.h>
#include "global_def.h"
#include <TimerOne.h>

#define BLANK 0xffe0

void		shift_Matrix_left	(int delay_iteration, int ammount);
void		displayMatrix		();
void		shift				(uint16_t input);
int			insertLetter		(int letter, int delay_ms);
void		insertSpace			();
void		displayMessage		(char* message, int delay_iteration);
void		matrix_init			();
void		configure_pinning	(uint8_t new_latch_pin, uint8_t new_clock_pin, uint8_t new_data_pin);
void		updateDisplay		();

#endif