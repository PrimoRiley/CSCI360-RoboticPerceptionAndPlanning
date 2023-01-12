#ifndef INCLUDE_CSCI_DISPLAYS
#define INCLUDE_CSCI_DISPLAYS

// Displays classes header file for Arduino sensors/devices.

#include <Arduino.h>

namespace csci
{

/********************* SERIAL MONITOR ******************************/
// A SerialMonitor uses the serial communications line between the
// Arduino and the IDE to send text and numbers.  The IDE will display
// output from the serial monitor in a separate window.

class SerialMonitor
{
  public:
  // Pass ctor the serial data rate (in bits per second)
  SerialMonitor(uint32_t dataRate = 9600);

  // Call from Arduino's main "setup" routine
  void setup();
  
  // Send text representation of values of various types (does NOT advance to a new line)
  // "base" can be one of DEC, HEX, OCT, BIN or BYTE.  Default is DEC.
  void sendIntegerValue(int value, int base = DEC);
  void sendUnsignedIntegerValue(uint16_t value, int base = DEC);
  void sendLongValue(long value, int base = DEC);
  void sendUnsignedLongValue(uint32_t value, int base = DEC);
  void sendBooleanValue(bool value);
  void sendDoubleValue(double value, int decimals = 3); // decimals = number of decimal places
  void sendDigitalLogicValue(int value);  // Pass in the value representing a HIGH or a LOW
  
  // Send some text (does NOT advance to a new line)
  void sendText(const String& text);
  
  // Send a "newline" sequence to advance to the beginning of a new line
  void sendNewline();
  
  private:
  uint32_t  m_dataRate;   // Holds serial data rate (in bits per second)
};

}   // End namespace

#endif    // INCLUDE_CSCI_DISPLAYS
