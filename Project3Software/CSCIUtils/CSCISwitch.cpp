// Utility Library switch classes implementation file.

#include "CSCISwitch.h"
#include "CSCITimer.h"

namespace csci
{

// ---------------------------------------------------------
const PushButton& WaitForEitherButtonClick
  (const PushButton& buttonA, const PushButton& buttonB)
{
  while ( true )
  {
    if ( buttonA.closed() )
    {
      buttonA.waitForClick();
      return buttonA;
    }
  
    if ( buttonB.closed() )
    {
      buttonB.waitForClick();
      return buttonB;
    }
  }    
}

// ---------------------------------------------------------
void PushButton::waitForClick() const
{
  waitUntilClosed();
  waitUntilOpen();
}

// ---------------------------------------------------------
void PushButton::waitUntilClosed() const
{
  // Wait until button stays closed for a minimum amount of time.

  const int minPressTime = 50;    // Min press time (in milliseconds)
  
  do
  {
    // Wait for initial button contact.
  
    while ( open() )
      { }
  
    // Now wait a small time for button debounce.
  
    WaitMillis(minPressTime);
    
  } while ( open() );
}

// ---------------------------------------------------------
void PushButton::waitUntilOpen() const
{
  // Wait until button stays open for a minimum amount of time.

  const int minPressTime = 50;    // Min press time (in milliseconds)
  
  do
  {
    // Wait for initial button release.
  
    while ( closed() )
      { }
  
    // Now wait a small time for button debounce.
  
    WaitMillis(minPressTime);
    
  } while ( closed() );
}

// ---------------------------------------------------------
PrizmStartButton::PrizmStartButton(PRIZM& prizm)
  : m_Prizm(prizm)
  { }
  
// ---------------------------------------------------------
bool PrizmStartButton::open()
{
  return ( m_Prizm.readStartButton() == 0 );
}

// ---------------------------------------------------------
bool PrizmStartButton::closed()
{
  return ( m_Prizm.readStartButton() == 1 );
}

// ---------------------------------------------------------
void PrizmStartButton::waitForClick()
{
  while ( open() )
  { }
  
  while ( closed() )
  { }
}

}   // End namespace
