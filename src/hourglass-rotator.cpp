// Hourglass rotator
//
// Firmware for attiny2313 based ac motor controller. Detects the upright
// position of the motor by rotating shaft contacting a button and afterwards
// waits for the sand in hourglass to move, then rotates the hourglass 180
// degrees again.
//
// Author: Otto Urpelainen
// Email: oturpe@iki.fi

#include "Attiny2313Utils.h"

#include "config.h"

#include <avr/io.h>
#include <util/delay.h>

#ifdef DEBUG
#include "Debug.h"
#endif

/// Limits given value by given minimum and maximum values.
///
/// \param value
///    Value to limit
///
/// \param min
///    Minimum value
///
/// \param max
///    Maximum value
///
/// \return
///    Limited value
int16_t limit(int16_t value, int16_t min, int16_t max) {
  if(value < min)
    return min;
  if(value > max)
    return max;

  return value;
}

int main() {
  // Wait for powr supply capacitors to charge etc.
  //_delay_ms(1000);

  #ifdef DEBUG
    /* TODO: implement debugging
    Debug debug(DEBUG_FREQ);
    */
  #endif

  // Set pin D5 as output
  DDRD |= BV(DDD5);

  uint8_t counter = 0x80;
  while(true) {
    _delay_ms(500);
    PORTD |= BV(PORTD5);
    _delay_ms(500);
    PORTD &= !BV(PORTD5);

    }
}