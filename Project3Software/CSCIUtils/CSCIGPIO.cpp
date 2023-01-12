// GPIO port pin class implementation.

#include <CSCIGPIO.h>

namespace csci
{

//********************** GPIO OUTPUT PIN ****************************/

GPIOOutputPin::GPIOOutputPin(int pinNumber, int activeState)
  : m_pinNumber(pinNumber),
    m_activeState(activeState)
{
  // Do nothing if default ctor was called.
  
  if ( pinNumber < 0 ) return;
  
  setup(pinNumber, activeState);
}

void GPIOOutputPin::setup(int pinNumber, int activeState)
{
  m_pinNumber = pinNumber;
  m_activeState = activeState;
  
  pinMode(m_pinNumber, OUTPUT);   // Init as an output port
  inactive();                     // Init to be inactive
}
  
void GPIOOutputPin::active()
{
  digitalWrite(m_pinNumber, m_activeState);
}
  
void GPIOOutputPin::inactive()
{
  digitalWrite(m_pinNumber, (m_activeState == HIGH) ? LOW : HIGH);
}

void GPIOOutputPin::setActiveState(int state)
{
  m_activeState = state;
}
  
int GPIOOutputPin::getActiveState() const
{
  return m_activeState;
}


/********************** GPIO INPUT PIN *****************************/

GPIOInputPin::GPIOInputPin(int pinNumber, int activeState, bool activateInternalPullUp)
  : m_pinNumber(pinNumber),
    m_activeState(activeState)
{
  if ( activateInternalPullUp )
  {
    pinMode(m_pinNumber, INPUT_PULLUP);
  }
  else
  {
    pinMode(m_pinNumber, INPUT);
  }
}
  
bool GPIOInputPin::active() const
{
  return digitalRead(m_pinNumber) == m_activeState;
}

bool GPIOInputPin::inactive() const
{
  return !active();
}

void GPIOInputPin::setActiveState(int state)
{
  m_activeState = state;
}
  
int GPIOInputPin::getActiveState() const
{
  return m_activeState;
}

}   // End namespace
