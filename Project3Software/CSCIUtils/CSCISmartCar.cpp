// SmartCar classes implementation file

#include "CSCISmartCar.h"
#include "CSCITimer.h"

namespace csci
{

/****************** TETRIX MECANUM SMART CAR ***********************/

TMSmartCar::TMSmartCar(PRIZM& prizm, EXPANSION& exc,
                       ColorSensor& colorSensor,
                       double fbMultiplier,
                       double lrMultiplier,
                       double diagMultiplier,
                       double spinMultiplier,
                       int rangeFinderPort,
                       double speedFraction)
  : TMDriveTrain(prizm, exc, fbMultiplier, lrMultiplier,
                 diagMultiplier, spinMultiplier, speedFraction),
    m_Prizm(prizm),
    m_Exc(exc),
    m_CSensor(colorSensor),
    m_SonicPort(rangeFinderPort)
  { }
    
bool TMSmartCar::setupCar()
{
  // Setup drive train
  
  TMDriveTrain::setup();
  
  // Setup color sensor
  
  if ( !m_CSensor.setup() )
  {
    return false;
  }
  
  return true;
}

double TMSmartCar::getRangeSensorDistanceCM()
{
  return static_cast<double>(m_Prizm.readSonicSensorCM(m_SonicPort));
}

TapeColor TMSmartCar::getTapeColor()
{
  return m_CSensor.getTapeColor();
}
  
double TMSmartCar::getBatteryVoltage()
{
  return static_cast<double>(m_Prizm.readBatteryVoltage()) / 100.0;
}
  
bool TMSmartCar::startButtonPressed()
{
  return ( m_Prizm.readStartButton() == 1 );
}

void TMSmartCar::waitStartButtonClicked()
{
  // Wait for start button to be pressed.
  
  while ( !startButtonPressed() )
  { }
  
  // Wait for start button to be released.
  
  while ( startButtonPressed() )
  { }
}

void TMSmartCar::setRedLEDState(bool state)
{  
  m_Prizm.setRedLED(state ? 1 : 0);
}

void TMSmartCar::setGreenLEDState(bool state)
{  
  m_Prizm.setGreenLED(state ? 1 : 0);
}

}   // End namespace
