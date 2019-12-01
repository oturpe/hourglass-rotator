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

// The limits of motor movement.
enum Limit {
  // Limit in clockwise direction
  L_CW,
  // Limit in counterclockwise direction
  L_CCW,
  // Neither limit
  L_NONE
};

/// \class SandTimer
///
/// A simple timer that counts calls to wait until specified amount of ticks
/// have been registered.
class SandTimer {
public:
  /// \brief Creates a new SandTimer in uninitialized state.
  SandTimer() : remainingTicks(-1) {}

public:
  /// \brief Initializes wait of given amount of ticks.
  ///
  /// \param ticks
  ///    The number of ticks to wait
  void beginWait(uint16_t ticks) {
    remainingTicks = ticks;
  }

  /// \brief Waits for one tick and announces completion if the wait completes.
  ///
  /// \return
  ///    If wait completed
  bool tick() {
    remainingTicks -= 1;
    return !remainingTicks;
  }

private:
  // Amount of ticks until completion of wait. Negative number means timer has
  // not been initialized and will not ever complete wait.
  uint16_t remainingTicks;
};

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

/// \brief Turns the motor on or off
///
/// \param running
///    If the motor is running. Otherwise it is not running.
void setMotor(bool running) {
  if (running)
    PORTD |= BV(PORTD4);
  else
    PORTD &= ~BV(PORTD4);
}

int main() {
  // Wait for powr supply capacitors to charge etc.
  //_delay_ms(1000);

  // Set pin D5 as output (indicator)
  DDRD |= BV(DDD5);
  // Set pin D4 as output (relay)
  DDRD |= BV(DDD4);
  // Set pins A0 and A1 as inputs with pullup enabled
  PORTA |= BV(PORTA0) | BV(PORTA1);

  uint16_t counter = 0;
  bool relayPulls = false;
  bool indicatorLit = 0;
  bool cwButtonDown;
  bool ccwButtonDown;
  Limit lastLimit = L_NONE;
  SandTimer sandTimer;

  // Starts with motor running
  setMotor(true);

  while(true) {
    counter += 1;
    _delay_ms(LOOP_DELAY);

    if(counter % INDICATOR_HALF_PERIOD == 0) {
      indicatorLit = !indicatorLit;
      setIndicator(indicatorLit);
    }

    cwButtonDown = PINA & BV(PINA0);
    ccwButtonDown = PINA & BV(PINA1);

    bool limitReached = false;
    if (cwButtonDown && lastLimit != L_CW) {
      limitReached = true;
      lastLimit = L_CW;
    }
    if (ccwButtonDown && lastLimit != L_CCW) {
      limitReached = true;
      lastLimit = L_CCW;
    }

    if (limitReached) {
      setMotor(false);
      sandTimer.beginWait(HOURGLASS_PERIOD);
    } else {
      bool timerDone = sandTimer.tick();
      if (timerDone) {
        setMotor(true);
      }
    }
  }
}