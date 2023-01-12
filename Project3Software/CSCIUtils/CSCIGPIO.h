#ifndef INCLUDE_CSCI_GPIO
#define INCLUDE_CSCI_GPIO

// GPIO port pin class definitions.

#include <Arduino.h>

namespace csci
{

//********************** GPIO OUTPUT PIN ****************************/
// A GPIOOutputPin is associated with a HIGH/LOW GPIO output pin and can
// only be in one of two states: "active" or "inactive".

class GPIOOutputPin
{
  public:
  // activeState = HIGH means device is active if pin is HIGH
  // activeState = LOW  means device is active if pin is LOW
  // If default ctor is used, one MUST call "setup" method before using object.  
  GPIOOutputPin(int pinNumber = -1, int activeState = HIGH);
  
  void setup(int pinNumber, int ActiveState);
  
  public:
  void active();      // Set output pin to active state
  void inactive();    // Set output pin to inactive state
  
  // Set/Get which state (HIGH or LOW) is considered the active state
  void setActiveState(int state);
  int  getActiveState() const;
  
  private:
  int         m_pinNumber;          // GPIO pin number.
  int         m_activeState;        // Pin state (HIGH/LOW) which activates device.
};

// Passed to various constructors to activate/deactivate an input pin's internal pull up resistor.

static const bool InternalPullUp = true;
static const bool NoInternalPullUp = false;

/********************** GPIO INPUT PIN *****************************/
// A GPIOInputPin is associated with an input port and can only be
// active or inactive.

class GPIOInputPin
{
  public:
  // activeState = HIGH means input is active if port is HIGH
  // activeState = LOW  means input is active if port is LOW
  // activateInternalPullUp = "true" if internal pull-up resistor is to be activated
  GPIOInputPin(int pinNumber, int activeState, bool activateInternalPullUp);  
  
  public:
  bool active() const;    // Returns "true" if active, "false" if inactive
  bool inactive() const;  // Returns "true" if inactive, "false" if active
  
  // Set/Get which state (HIGH or LOW) is considered the active state
  void setActiveState(int state);
  int  getActiveState() const;
    
  private:
  int           m_pinNumber;        // Input port pin number
  int           m_activeState;      // Port state (HIGH/LOW) which means input is active
};

}   // End namespace

#endif    // INCLUDE_CSCI_GPIO
