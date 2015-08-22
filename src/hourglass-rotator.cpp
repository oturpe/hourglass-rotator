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

/// \brief Turns the indicator led on of off
///
/// \param lit
///    If led is turned on. Otherwise it is turned off.
void setIndicator(bool lit) {
  if (lit)
    PORTD |= BV(PORTD5);
  else
    PORTD &= ~BV(PORTD5);
}

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

  // Set pin D5 as output (debug indicator)
  DDRD |= BV(DDD5);
// Set pin D4 as output (relay)
  DDRD |= BV(DDD4);
// Set pins A0 and A1 as inputs with pullup enabled
  PORTA |= BV(PORTA0) | BV(PORTA1);

  uint8_t counter = 0;
  bool relayPulls = false;
  bool cwButtonDown;
  bool ccwButtonDown;

  while(true) {
    _delay_ms(500);
    setIndicator(true);
    _delay_ms(500);
    setIndicator(false);

    if (!(counter % 4)) {
      if(relayPulls)
        PORTD &= ~BV(PORT4);
      else
        PORTD |= BV(PORT4);

      relayPulls = !relayPulls;
    }

    counter += 1;

    // No negation here becauee the buttons are of "normally on" type.
    cwButtonDown = PINA & BV(PINA0);
    ccwButtonDown = PINA & BV(PINA1);
    if (cwButtonDown || ccwButtonDown)
      setIndicator(true);
  }
}