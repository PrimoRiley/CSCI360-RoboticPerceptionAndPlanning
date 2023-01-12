#ifndef INCLUDE_CSCI_ANALOG_PIN
#define INCLUDE_CSCI_ANALOG_PIN

// Utility Library analog pin classes header file.

#include "CSCICore.h"

namespace csci
{

/*********************** ANALOG OUTPUT *****************************/
// An AnalogOutput is associated with an output port that can take on
// integer values from minOutput to maxOutput.  minOutput will produce
// the lowest voltage output, maxOutput the highest.  AnalogOutput is
// the virtual base class of all analog output classes.

class AnalogOutput
{
  protected:
  AnalogOutput(int minOutput = 0, int maxOutput = 255);
  
  public:
  // Output a fraction of the voltage range
  // 0.0 = minimum voltage
  // 1.0 = maximum voltage
  void output(double fraction);
  
  // Output a port voltage that corresponds to the value passed
  void outputPortValue(int value);
  
  int getMinOutputValue() const;  // Returns value which produces minimum output voltage
  int getMaxOutputValue() const;  // Returns value which produces maximum output voltage

  // Cycle pin active/inactive for the specified time.  
  void cycleActiveInactive(uint32_t activeTimeMicros, uint32_t inactiveTimeMicros);
  
  protected:
  // Output raw port value passed with with no special processing.
  virtual void outputRawPortValue(int value) = 0;
  
  private:
  int   m_minOutput;        // Value which produces minimum output voltage
  int   m_maxOutput;        // Value which produces maximum output voltage
  int   m_lastPortValue;    // Last port value output.  
};

/********************* PWM ANALOG OUTPUT ***************************/
// The PWMAnalogOutput class is a subclass of AnalogOutput, producing
// an average analog voltage by generating (in hardware) the PWM stream.
//
// Arduino Uno pins 5 & 6 generate the PWM stream at 980 Hz.
// Arduino Uno pins 3, 9, 10 & 11 generate the PWM stream at 490 Hz.
//
// Arduino Mega pins 4 & 13 generate the PWM stream at 980 Hz.
// Arduino Mega pins 2, 3, 5 to 12, 44, 45 & 46 generate at 490 Hz.

class PWMAnalogOutput : public AnalogOutput
{
  public:
  PWMAnalogOutput(int pinNumber, int minOutput = 0, int maxOutput = 255);
  
  protected:
  // Output raw port value passed with no special processing.
  void outputRawPortValue(int value) override;
  
  private:
  int m_pinNumber;    // Pin number of hardware PWM output port.
};

/*********************** ANALOG INPUT ******************************/
// An AnalogInput is associated with an input port and can take on
// integer values from minInput to maxInput.  minInput represents
// the lowest voltage possible, maxInput the highest.

class AnalogInput
{
  protected:
  // By default, Arduino analog inputs return values ranging from 0 to 1023
  AnalogInput(int pinNumber, int minInput = 0, int maxInput = 1023);
  
  public:
  // Get the fraction of the possible input voltage range.
  // 0.0 = minimum voltage
  // 1.0 = maximum voltage
  double input() const;
  
  // Get the value corresponding to the input port voltage.
  int inputPortValue() const;
  
  int getMinInputValue() const;   // Returns value corresponding to minimum input voltage
  int getMaxInputValue() const;   // Returns value corresponding to maximum input voltage

  private:
  int           m_pinNumber;        // Input port pin number
  int           m_minInput;         // Value which produces minimum input voltage
  int           m_maxInput;         // Value which produces maximum input voltage
};

}   // End namespace

#endif    // INCLUDE_CSCI_ANALOG_PIN
