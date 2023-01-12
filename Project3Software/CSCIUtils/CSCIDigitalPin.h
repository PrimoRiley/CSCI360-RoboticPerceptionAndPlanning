#ifndef INCLUDE_CSCI_DIGITAL_PIN
#define INCLUDE_CSCI_DIGITAL_PIN

// Utility Library DigitalPin classes header file.

#include "CSCICore.h"
#include "CSCIGPIO.h"

namespace csci
{

//********************** DIGITAL OUTPUT *****************************/
// A DigitalOutput is associated with a HIGH/LOW output pin and can
// only be in one of two states: "active" or "inactive".

class DigitalOutput
{
  protected:
  // activeState = HIGH means device is active if port is HIGH
  // activeState = LOW  means device is active if port is LOW  
  DigitalOutput(int pinNumber, int activeState);
  
  public:
  void active()   { m_gpioPin.active(); }   // Set output port to active state
  void inactive() { m_gpioPin.inactive(); } // Set output port to inactive state
  
  // Set/get which state (HIGH or LOW) is considered the active state
  void setActiveState(int state) { m_gpioPin.setActiveState(state); }
  int  getActiveState() const { return m_gpioPin.getActiveState(); }
  
  // Pulse output state for the specified duration.
  
  void pulseActive(uint32_t microSeconds);
  void pulseInactive(uint32_t microSeconds);
  
  // Cycle output state for the specified durations.

  void cycleActive(uint32_t microsActive, uint32_t microsInactive);
  void cycleInactive(uint32_t microsInactive, uint32_t microsActive);
  
  private:
  GPIOOutputPin m_gpioPin;  // GPIO output pin object is connected to.
};

/********************** DIGITAL INPUT ******************************/
// A DigitalInput is associated with a HIGH/LOW input pin and can only
// be active or inactive.

class DigitalInput
{
  protected:
  // activeState = HIGH means input is active if port is HIGH
  // activeState = LOW  means input is active if port is LOW
  // activateInternalPullUp = "true" if internal pull-up resistor is to be activated
  DigitalInput(int pinNumber, int activeState, bool activateInternalPullUp);  
  
  public:
  bool active() const   { return m_gpioPin.active(); }    // Returns "true" if active, "false" if inactive
  bool inactive() const { return m_gpioPin.inactive(); }  // Returns "true" if inactive, "false" if active
  
  // Set/get which state (HIGH or LOW) is considered the active state
  void setActiveState(int state) { m_gpioPin.setActiveState(state); }  
  int  getActiveState() const { return m_gpioPin.getActiveState(); }
  
  private:
  GPIOInputPin  m_gpioPin;  // GPIO input pin port is connected to.
};

}   // End namespace

#endif    // INCLUDE_CSCI_DIGITAL_PIN
