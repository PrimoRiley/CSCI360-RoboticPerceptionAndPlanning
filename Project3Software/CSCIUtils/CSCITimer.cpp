// Utility Library timer classes implementation file.

#include <CSCITimer.h>

namespace csci
{

/******************* TIMER (MICROSECONDS) **************************/

TimerMicros::TimerMicros(uint32_t waitTimeInMicros)
  : Timer(),
    m_microsRemaining(0),
    m_microsExcess(0)
{
  if ( waitTimeInMicros > 0 )
  {
    start(waitTimeInMicros);
  }
}

void TimerMicros::start(uint32_t waitTimeInMicros)
{
  m_microsRemaining = waitTimeInMicros;
  m_microsExcess = 0;
  m_lastUptime = micros();
  setActive();
}

void TimerMicros::stop()
{
  setInActive();
  m_microsExcess = 0;
}

bool TimerMicros::done()
{
  // If we're inactive, then we can never be done.
  
  if ( !isActive() ) return false;
  
  uint32_t currentUptime = micros();
  uint32_t elapsedUptime = currentUptime - m_lastUptime;
  
  //  *** IMPORTANT ***
  //
  // In a perfect world, the uptime would continue to increase every time
  // micros() was called.  However, every 71.58 minutes (approximate), the
  // uptime overflows the uint32_t and starts counting from zero again.
  
  // If the current uptime is less than the last uptime, the uptime count overflowed.
  
  if ( currentUptime < m_lastUptime )
  {
    static const uint32_t uint32MaxValue = 0xFFFFFFFFUL;  // Max value storable in an uint32_t.
    
    // The elapsed uptime is time to overflow + currentUptime (time after overflow).
    
    elapsedUptime = (uint32MaxValue - m_lastUptime) + currentUptime;
  }

  if ( elapsedUptime >= m_microsRemaining )
  {
    setInActive();
    m_microsExcess = elapsedUptime - m_microsRemaining;
    m_microsRemaining = 0;
    return true;
  }
  
  m_microsRemaining -= elapsedUptime;
  m_lastUptime = currentUptime;
  return false;
}

uint32_t TimerMicros::timeRemaining() const
{
  return m_microsRemaining;
} 

uint32_t TimerMicros::excessTime() const
{
  return m_microsExcess;
}

/******************* TIMER (MILLISECONDS) **************************/

TimerMillis::TimerMillis(uint32_t waitTimeInMillis)
  : Timer(),
    m_numMillisRemaining(0),
    m_millisTimer(),
    m_millisExcess(0)
{
  if ( waitTimeInMillis > 0 )
  {
    start(waitTimeInMillis);
  }
}

void TimerMillis::start(uint32_t waitTimeInMillis)
{
  m_numMillisRemaining = waitTimeInMillis;
  m_millisExcess = 0;
  m_millisTimer.start(MICROS_PER_MILLIS);
  setActive();
}

void TimerMillis::stop()
{
  m_millisTimer.stop();
  m_millisExcess = 0;
  setInActive();
}

bool TimerMillis::done()
{
  // If we're inactive, then we can never be done.
  
  if ( !isActive() ) return false;
  
  // If we've waited at least one millisecond...
  
  if ( m_millisTimer.done() )
  {
    // Calculate how many full milliseconds were exceeded.
    
    uint32_t excessFullMillis = m_millisTimer.excessTime() / MICROS_PER_MILLIS;
    
    // Calculate how many more microseconds beyond that were exceeded.
    
    uint32_t excessMicros = m_millisTimer.excessTime() % MICROS_PER_MILLIS;
    
    // If the number of full milliseconds we waited does NOT exceed
    // the number of milliseconds remaining to wait...
    
    uint32_t fullMillisWaited = 1 + excessFullMillis;
    
    if ( fullMillisWaited < m_numMillisRemaining )
    {
      m_numMillisRemaining -= fullMillisWaited;
      
      // Wait another millisecond, adjusted for excess microseconds.
      
      m_millisTimer.start(MICROS_PER_MILLIS - excessMicros);
    }
    else
    {
      // We waited more milliseconds than requested.  Calc and store
      // the excess full milliseconds we waited.
      
      setInActive();
      m_millisExcess = fullMillisWaited - m_numMillisRemaining;
      m_numMillisRemaining = 0;
      
      return true;
    }
  } 
    
  return false;
}

uint32_t TimerMillis::timeRemaining() const
{
  return m_numMillisRemaining;
}

uint32_t TimerMillis::excessTime() const
{
  return m_millisExcess;
}

// ---------------------------------------------------------
void WaitMillis(unsigned long milliseconds)
{
  TimerMillis timer(milliseconds);

  while ( !timer.done() )
    { }
}

// ---------------------------------------------------------
void WaitMicros(unsigned long microseconds)
{
  TimerMicros timer(microseconds);

  while ( !timer.done() )
    { }
}

}   // End namespace
