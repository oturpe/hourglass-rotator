// Cpu frequency for util/delay.h
#define F_CPU 1000000

// Half of delay in main execution loop, given in milliseconds.
#define LOOP_DELAY 50

// Half length of indicator led on-off sequence. In other words, the time
// the indicator is lit of darkened. Given in units of LOOP_DELAY
#define INDICATOR_HALF_PERIOD 10

// Amount of time to wait for the hourglass to empty. Given in units of
// LOOP_DELAY
#define HOURGLASS_PERIOD 20 /*one second*/ * 10

//Enables debug messaging
#define DEBUG
