// Utility Library sound classes implementation file.

#include "CSCISound.h"
#include "CSCITimer.h"

namespace csci
{

/*********************** ACTIVE BUZZER *****************************/

ActiveBuzzer::ActiveBuzzer(int pinNumber, int onState)
  : DigitalOutput(pinNumber, onState) { }

void ActiveBuzzer::alert()
{
  cycleActive(200000, 200000);
}
  
void ActiveBuzzer::beep(uint32_t durationMilliSeconds)
{
  uint32_t microsDuration = durationMilliSeconds * MICROS_PER_MILLIS;
  pulseActive(microsDuration);
}

}   // End namespace
