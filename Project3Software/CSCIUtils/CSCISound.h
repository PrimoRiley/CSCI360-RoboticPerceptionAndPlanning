#ifndef INCLUDE_CSCI_SOUND
#define INCLUDE_CSCI_SOUND

// Utility Library sound classes header file.

#include "CSCIDigitalPin.h"

namespace csci
{

/*********************** ACTIVE BUZZER *****************************/
// An ActiveBuzzer is associated with a HIGH/LOW output port and is
// either on (active) or off (inactive).

class ActiveBuzzer : public DigitalOutput
{
  public:
  ActiveBuzzer(int pinNumber, int onState = HIGH);
  
  void on()   { active(); }
  void off()  { inactive(); }
  
  void alert();   // Generate one special "alert" beep cycle.
  
  void beep(uint32_t durationMilliSeconds);
};

}   // End namespace

#endif    // INCLUDE_CSCI_SOUND
