// Utility Library analog pin classes implementation file.

#include "CSCIAnalogPin.h"
#include "CSCITimer.h"

namespace csci
{

/*********************** ANALOG OUTPUT *****************************/

AnalogOutput::AnalogOutput(int minOutput, int maxOutput)
  : m_minOutput(minOutput),
    m_maxOutput(maxOutput),
    m_lastPortValue(0)    
    { }
  
void AnalogOutput::output(double fraction)
{
  if ( fraction < 0.0 ) fraction = 0.0;
  if ( fraction > 1.0 ) fraction = 1.0;
    
  int outValue = m_minOutput;
    
  if ( m_minOutput <= m_maxOutput )
  {
    // Output value increases from m_minOutput to m_maxOutput
    outValue += (int) ((m_maxOutput - m_minOutput) * fraction);
  }
  else
  {
    // Output value decreases from m_minOutput to m_maxOutput
    outValue -= (int) ((m_minOutput - m_maxOutput) * fraction);
  }

  outputPortValue(outValue);
}

void AnalogOutput::outputPortValue(int value)
{
  outputRawPortValue(value);
  m_lastPortValue = value;  
}
  
int AnalogOutput::getMaxOutputValue() const
{
  return m_maxOutput;
}

int AnalogOutput::getMinOutputValue() const
{
  return m_minOutput;
}

void AnalogOutput::cycleActiveInactive
  (uint32_t activeTimeMicros, uint32_t inactiveTimeMicros)
{
  // Save last port value output
  
  int lastPortValue = m_lastPortValue;
  
  // Set last port value output
  
  outputPortValue(lastPortValue);
  WaitMicros(activeTimeMicros);
  
  // Now output minimum port value.
  
  output(0.0);
  WaitMicros(inactiveTimeMicros);
  
  // Restore previous port value
  
  outputPortValue(lastPortValue);
}

/********************* PWM ANALOG OUTPUT ***************************/

PWMAnalogOutput::PWMAnalogOutput(int pinNumber, int minOutput, int maxOutput)
  : AnalogOutput(minOutput, maxOutput),
    m_pinNumber(pinNumber)
{
  pinMode(m_pinNumber, OUTPUT);
  output(0.0);  // Init to minimum output
}
  
void PWMAnalogOutput::outputRawPortValue(int value)
{
  analogWrite(m_pinNumber, value);
}

/*********************** ANALOG INPUT ******************************/

AnalogInput::AnalogInput(int pinNumber, int minInput, int maxInput)
  : m_pinNumber(pinNumber),
    m_minInput(minInput),
    m_maxInput(maxInput)
    { }
  
double AnalogInput::input() const
{
  int value = inputPortValue();

  if ( m_minInput <= m_maxInput )
  {
    // Input value increases from m_minInput to m_maxInput
    return (double) (value - m_minInput) / (double) (m_maxInput - m_minInput);
  }
  else
  {
    // Input value decreases from m_minInput to m_maxInput
    return (double) (m_minInput - value) / (double) (m_minInput - m_maxInput);
  }  
}

int AnalogInput::inputPortValue() const
{
  return analogRead(m_pinNumber);
}

int AnalogInput::getMaxInputValue() const
{
  return m_maxInput;
}

int AnalogInput::getMinInputValue() const
{
  return m_minInput;
}

}   // End namespace
