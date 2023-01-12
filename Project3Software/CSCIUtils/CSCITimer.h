#ifndef INCLUDE_CSCI_TIMER
#define INCLUDE_CSCI_TIMER

// Utility Library timer classes header file.

#include <Arduino.h>

namespace csci
{

const uint32_t MICROS_PER_MILLIS = 1000;  // Number of microseconds in a millisecond.
const uint32_t MILLIS_PER_SECOND = 1000;  // Number of milliseconds in a second.
const uint32_t MICROS_PER_SECOND = MILLIS_PER_SECOND * MICROS_PER_MILLIS;

/************************** TIMER **********************************/
// Timer is the abstract base class for timers of various resolutions.
// The time interval can be specified and the timer started or stopped.
// Whether or not the time interval has passed or not can also be checked.

class Timer
{
  protected:
  // The default constructor does NOT start timing.
  
  Timer() : m_active(false) { };

  // Start timer using the time interval provided.
  // This restarts the timer if it was already running.

  virtual void start(uint32_t waitTime) = 0;

  // Stop any timer that's currently running.

  virtual void stop() = 0;

  // Returns "true" if we're active AND done timing (i.e., time interval has elapsed).

  virtual bool done() = 0;
  
  public:
  // Returns "true" if timer is active;

  virtual bool isActive() { return m_active; }

  protected:
  void setActive()    { m_active = true; }  
  void setInActive()  { m_active = false; }

  protected:
  bool m_active;    // "true" if the timer is active (i.e., waiting).
};

/******************* TIMER (MICROSECONDS) **************************/
// TimerMicros is a class which times an interval at microsecond resolution.
// Since the wait interval is a uint32_t, the maximum interval is
// 4,294,967,295 microseconds (about 71.58 minutes).

class TimerMicros : public Timer
{
  public:
  // The default constructor does NOT start timing.
  // Pass in a non-zero specific interval to have ctor start timing.
  
  TimerMicros(uint32_t waitTimeInMicros = 0);

  // Start timer using the time interval provided.
  // This restarts the timer if it was already running.

  void start(uint32_t waitTime) override;

  // Stop any timer that's currently running.

  void stop() override;

  // Returns "true" if we're active AND done timing (i.e., time interval has elapsed).

  bool done() override;
  
  // Returns microseconds remaining before timer is done.
  
  uint32_t timeRemaining() const; 
  
  // Returns the number of excess microseconds elapsed past timer expiration.
  // Value is valid only if timer has expired (i.e., "done()" returns true).
  
  uint32_t excessTime() const;

  private:
  uint32_t m_microsRemaining;  // Number of microseconds remaining.
  uint32_t m_lastUptime;       // Last uptime value read.
  uint32_t m_microsExcess;     // Number of excess microseconds.
};

/******************* TIMER (MILLISECONDS) **************************/
// TimerMillis is a class which times an interval at millisecond resolution.
// Since the wait interval is uint32_t, the maximum interval is
// 4,294,967,295 milliseconds (about 49 days, 17 hours, 2 min, 47 seconds).

class TimerMillis : public Timer
{
  public:
  // The default constructor does NOT start timing.
  // Pass in a non-zero specific interval to have ctor start timing.
  
  TimerMillis(uint32_t waitTimeInMillis = 0);

  // Start timer using the time interval provided.
  // This restarts the timer if it was already running.

  void start(uint32_t waitTime) override;

  // Stop any timer that's currently running.

  void stop() override;

  // Returns "true" if we're active AND done timing (i.e., time interval has elapsed).

  bool done() override;
  
  // Returns milliseconds remaining before timer is done.
  
  uint32_t timeRemaining() const;   
  
  // Returns the number of excess milliseconds elapsed past timer expiration.
  // Value is valid only if timer has expired (i.e., "done()" returns true).
  
  uint32_t excessTime() const;   

  private:
  uint32_t    m_numMillisRemaining;   // Number of milliseconds remaining in interval.
  TimerMicros m_millisTimer;          // Timer to delay one millisecond.
  uint32_t    m_millisExcess;         // Number of excess milliseconds;
};

// Routine to wait a specified number of milliseconds.
// Note: Argument is "unsigned long" so this can be a replacement for the
//       Arduino "delay(...)" routine.

void WaitMillis(unsigned long milliseconds);

// Routine to wait a specified number of microseconds.

void WaitMicros(unsigned long microseconds);

}   // End namespace

#endif    // INCLUDE_CSCI_TIMER
