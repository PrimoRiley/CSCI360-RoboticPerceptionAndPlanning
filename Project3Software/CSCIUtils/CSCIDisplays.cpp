// Displays classes implementation file for Arduino sensors/devices

#include "CSCIDisplays.h"

namespace csci
{

// ---------------------------------------------------------

SerialMonitor::SerialMonitor(uint32_t dataRate)
{
  m_dataRate = dataRate;
}

void SerialMonitor::setup()
{
  Serial.begin(m_dataRate);
}

void SerialMonitor::sendIntegerValue(int value, int base)
{
  Serial.print(value, base);
}

void SerialMonitor::sendUnsignedIntegerValue(unsigned int value, int base)
{
  Serial.print(value, base);
}

void SerialMonitor::sendLongValue(long value, int base)
{
  Serial.print(value, base);
}

void SerialMonitor::sendUnsignedLongValue(uint32_t value, int base)
{
  Serial.print(value, base);
}

void SerialMonitor::sendBooleanValue(bool value)
{
  if ( value)
  {
    sendText("true");
  }
  else
  {
    sendText("false");
  }
}

void SerialMonitor::sendDoubleValue(double value, int decimals)
{
  Serial.print(value, decimals);
}

void SerialMonitor::sendDigitalLogicValue(int value)
{
  if ( value == HIGH )
  {
    sendText("HIGH");
  }
  else
  {
    if ( value == LOW )
    {
      sendText("LOW");
    }
    else
    {
      sendText("UNKOWN LOGIC VALUE (");
      sendIntegerValue(value);
      sendText(")");
    }
  }  
}

void SerialMonitor::sendText(const String& text)
{
  Serial.print(text);
}

void SerialMonitor::sendNewline()
{
  Serial.println();
}

}   // End namespace
