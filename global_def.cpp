#include "global_def.h"

int latch_pin =  7;
int clock_pin =  8;
int data_pin  =  9;

int iteration =  1;

bool matrix		[MATRIX_HEIGHT][MATRIX_WIDTH+MATRIX_BUFFER];
int	matrix_slice[MATRIX_HEIGHT][1];
