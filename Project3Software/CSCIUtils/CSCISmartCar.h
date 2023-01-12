#ifndef INCLUDE_CSCI_SMART_CAR
#define INCLUDE_CSCI_SMART_CAR

// SmartCar class header file for autonomous "smart car" vehicle.

#include <CSCIDriveTrain.h>
#include <CSCIColorSensor.h>
#include <CSCISwitch.h>
#include <PRIZM.h>            // Tetrix PRIZM controller library

namespace csci
{

/****************** TETRIX MECANUM SMART CAR ***********************/
// TMSmartCar is a Tetrix mechanum drive train (TMDTrain) with the
// following attached:
//
// 1 Ultrasonic range finder.
// 1 Color sensor.
// 1 Green "Start" pushbutton.
// 1 Red LED
// 1 Green LED
//
// IMPORTANT: The "front" of the car is considered the end where the
//            range finder and color sensor are mounted (i.e., the
//            sensors point forward).

class TMSmartCar : public TMDriveTrain
{
  public:
  // Construct using references to already instantiated PRIZM,
  // EXPANSION and ColorSensor objects, the various movement
  // adjustment multipliers, the Tetrix port (2,3,4,5) the sonic
  // rangefinder is connected to, and the initial speed fraction
  // (whose default is small for safety).
  
  // See note (in DriveTrain ctor comments) concerning movement
  // adjustment multipliers.
  
  TMSmartCar(PRIZM& prizm, EXPANSION& exc,
             ColorSensor& colorSensor,
             double fbMultiplier,
             double lrMultiplier,
             double diagMultiplier,
             double spinMultiplier,
             int rangeFinderPort = 5,
             double speedFraction = 0.1);

  // Perform additonal setup.  MUST be called after construction,
  // but before any other methods are called.  Returns "true" if
  // setup was successful.
  
  bool setupCar();

  // Get range sensor distance (in centimeters).
  
  double getRangeSensorDistanceCM();
  
  // Get tape color via color sensor
  
  TapeColor getTapeColor();
  
  // Get the battery voltage
  
  double getBatteryVoltage();
  
  // Returns "true" if Tetrix (green) Start button is pressed.
  
  bool startButtonPressed();
  
  // Wait for Tetrix Start button to be pressed, then released.
  
  void waitStartButtonClicked();
  
  // Set the Tetrix Red or Green LED states.
  // state = true (LED on)
  // state = false (LED off)
  
  void setRedLEDState(bool state);
  void setGreenLEDState(bool state);
  
  protected:
  PRIZM&        m_Prizm;      // Associated Tetrix PRIZM controller.
  EXPANSION&    m_Exc;        // Associated Tetrix EXPANSION controller.
  ColorSensor&  m_CSensor;    // Associated color sensor
  int           m_SonicPort;  // Tetrix port of sonic range finder
};
  
}	// End namespace

#endif    // INCLUDE_CSCI_SMART_CAR
