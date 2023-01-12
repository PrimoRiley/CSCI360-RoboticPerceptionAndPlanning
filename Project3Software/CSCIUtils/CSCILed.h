#ifndef INCLUDE_CSCI_LED
#define INCLUDE_CSCI_LED

// Utility Library LED class definitions.

#include <CSCIDigitalPin.h>
#include <CSCIAnalogPin.h>

namespace csci
{

/************************ DIGITAL LED ******************************/
// A DigtialLed is associated with a HIGH/LOW output pin and is either
// on (active) or off (inactive).

class DigitalLed : public DigitalOutput
{
  public:
  DigitalLed(int pinNumber, int onState = HIGH)
  : DigitalOutput(pinNumber, onState) { }
  
  void on()   { active(); }
  void off()  { inactive(); }
  
  // Blink LED on and off for specified duration (in milliseconds).
  
  void blink(uint32_t onTimeMillis, uint32_t offTimeMillis);
};

/************************* ANALOG LED ******************************/
// An AnalogLed is associated with an analog output port.

class AnalogLed
{
  public:
  AnalogLed(AnalogOutput& analogOutput);

  void intensity(double fraction) { m_analogOutput.output(fraction); }
  
  void outputPortValue(int value) { m_analogOutput.outputPortValue(value); }
  
  int getMinOutputValue() const { return m_analogOutput.getMinOutputValue(); }
  int getMaxOutputValue() const { return m_analogOutput.getMaxOutputValue(); }
  
  // Blink the LED for the specified time (in milliseconds).
  
  void blink(uint32_t onTimeMillis, uint32_t offTimeMillis);
  
  private:
  AnalogOutput& m_analogOutput; // Analog output object associated with the LED.
};


}   // End namespace

#endif    // INCLUDE_CSCI_LED
