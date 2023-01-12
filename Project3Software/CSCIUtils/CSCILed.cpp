// LED classes implementation.

#include <CSCILED.h>
#include <CSCITimer.h>

namespace csci
{

/************************ DIGITAL LED ******************************/

void DigitalLed::blink(uint32_t onTimeMillis, uint32_t offTimeMillis)
{
  cycleActive(onTimeMillis * MICROS_PER_MILLIS,
              offTimeMillis * MICROS_PER_MILLIS);
}

/************************* ANALOG LED ******************************/

AnalogLed::AnalogLed(AnalogOutput& analogOutput)
  : m_analogOutput(analogOutput) { }
  
void AnalogLed::blink(uint32_t onTimeMillis, uint32_t offTimeMillis)
{
  m_analogOutput.cycleActiveInactive(onTimeMillis * MICROS_PER_MILLIS,
                                     offTimeMillis * MICROS_PER_MILLIS);
}

}   // End namespace
