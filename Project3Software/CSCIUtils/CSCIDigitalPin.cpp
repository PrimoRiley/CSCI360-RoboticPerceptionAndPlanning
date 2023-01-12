// Utility Library DigitalPin classes implementation file.

#include "CSCIDigitalPin.h"
#include "CSCITimer.h"

namespace csci
{

/********************** DIGITAL OUTPUT *****************************/

DigitalOutput::DigitalOutput(int pinNumber, int activeState)
  : m_gpioPin(pinNumber, activeState)
{
  // Nothing more to do here.
}
  
void DigitalOutput::pulseActive(uint32_t microSeconds)
{
  active();
  WaitMicros(microSeconds);
  inactive();
}

void DigitalOutput::pulseInactive(uint32_t microSeconds)
{
  inactive();
  WaitMicros(microSeconds);
  active();
}

void DigitalOutput::cycleActive
	(uint32_t microsActive, uint32_t microsInactive)
{
  pulseActive(microsActive);
  pulseInactive(microsInactive);
}  

void DigitalOutput::cycleInactive
	(uint32_t microsInactive, uint32_t microsActive)
{
  pulseInactive(microsInactive);
  pulseActive(microsActive);
}

/********************** DIGITAL INPUT ******************************/

DigitalInput::DigitalInput(int pinNumber, int activeState, bool activateInternalPullUp)
  : m_gpioPin(pinNumber, activeState, activateInternalPullUp)
{
  // Nothing more to do here.
}

}   // End namespace
