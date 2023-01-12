#ifndef INCLUDE_CSCI_SWITCH
#define INCLUDE_CSCI_SWITCH

// Utility Library switch classes header file.

#include "CSCIDigitalPin.h"
#include <PRIZM.h>    // Tetrix PRIZM controller library

namespace csci
{

/********************** DIGITAL SWITCH ******************************/
// A DigitalSwitch is associated with a digital input port and can be
// either open (inactive) or closed (active).

class DigitalSwitch : public DigitalInput
{
  public:  
  DigitalSwitch(int pinNumber, int closedState = LOW, bool activateInternalPullUp = NoInternalPullUp)
  : DigitalInput(pinNumber, closedState, activateInternalPullUp) { }
  
  bool  open() const    { return inactive(); }
  bool  closed() const  { return active(); }
};

/************************ PUSH BUTTON *******************************/
// A PushButton is a DigitalSwitch which is momentarily closed when pushed.
// It's either open (inactive) or closed (active).

class PushButton : public DigitalSwitch
{
  public:  
  PushButton(int pinNumber, int closedState = LOW, bool activateInternalPullUp = NoInternalPullUp)
  : DigitalSwitch(pinNumber, closedState, activateInternalPullUp) { }
  
  public:
  void waitForClick() const;    // Wait until button is closed, then opened again.
  void waitUntilClosed() const; // Wait until button is closed.
  void waitUntilOpen() const;   // Wait until button is open. 
};

// Routine to wait for either button to be clicked.
// Returns reference to the pin number of the button that was clicked.

const PushButton& WaitForEitherButtonClick
  (const PushButton& buttonA, const PushButton& buttonBPin);

/****************** TETRIX PRIZM START BUTTON ***********************/
// Encapsulates the Tetrix PRIZM green Start Button.

class PrizmStartButton
{
  public:
  PrizmStartButton(PRIZM& prizm);
  
  bool open();    // Returns "true" is button is open (i.e., not pressed)
  bool closed();  // Returns "true" if button is closed (i.e., pressed)
  
  public:
  void waitForClick();    // Wait until button is closed, then opened again.
  
  private:
  PRIZM&  m_Prizm;  // Associated PRIZM object.
};

}   // End namespace

#endif    // INCLUDE_CSCI_SWITCH
