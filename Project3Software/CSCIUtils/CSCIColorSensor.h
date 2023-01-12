#ifndef INCLUDE_CSCI_COLOR_SENSOR
#define INCLUDE_CSCI_COLOR_SENSOR

// Utility Library ColorSensor classes header file.

#include <Adafruit_TCS34725.h>  // Adafruit TCS34725 color sensor utility library
#include "CSCIDisplays.h"
#include "CSCISwitch.h"

namespace csci
{

/*********************** TAPE COLOR *********************************/
// Recognized tape color.  (Standard colored electrical tape.)

typedef enum
{
  unknown = 0,  // Unknown color
  white = 1,    // White tape
  black = 2,    // Black tape
  gray = 3,     // Gray tape
  red = 4,      // Red tape
  green = 5,    // Green tape
  blue = 6,     // Blue tape
  yellow = 7    // Yellow tape
} TapeColor;

/*********************** COLOR RATIO ********************************/
// ColorRatio encapsulates ratios of (c,r,g,b) color components.

class ColorRatios
{
  public:
  ColorRatios(double cRatio = 1.0, double rRatio = 1.0 / 3.0,
              double gRatio = 1.0 / 3.0, double bRatio = 1.0 / 3.0)
    : m_cRatio(cRatio),
      m_rRatio(rRatio),
      m_gRatio(gRatio),
      m_bRatio(bRatio)
  { }
  
  // Map color ratio to a recognized tape color.
  
  TapeColor getTapeColor();
  
  // Get individual color component ratios.
  
  double  getCRatio()   { return m_cRatio; }
  double  getRRatio()   { return m_rRatio; }
  double  getGRatio()   { return m_gRatio; }
  double  getBRatio()   { return m_bRatio; }
  
  // Set individual color component ratios.
  
  void  setCRatio(double cRatio) { m_cRatio = cRatio; }
  void  setRRatio(double rRatio) { m_rRatio = rRatio; }
  void  setGRatio(double gRatio) { m_gRatio = gRatio; }
  void  setBRatio(double bRatio) { m_bRatio = bRatio; }
  
  // Display color ratios values.
  
  void  display(csci::SerialMonitor& smonitor);
  
  protected:
  
  // Each routine returns true if the color ratios specify
  // the indicated color.
  
  bool  colorIsWhite();
  bool  colorIsBlack();
  bool  colorIsGray();
  bool  colorIsRed();
  bool  colorIsGreen();
  bool  colorIsBlue();
  bool  colorIsYellow();
  
  // Returns true if red, green and blue color ratios are all
  // within the specified tolerance of each other.
  
  bool  rgbAreClose(double tolerance = 0.075);
  
  // Returns true if two color ratios are within the specified
  // tolerance of each other.
  
  bool  valuesAreClose(double ratio1, double ratio2, double tolerance);
  
  private:
  double  m_cRatio;   // Ratio (0 - 1.0) of "clear" background.
  double  m_rRatio;   // Ratio (0 - 1.0) of red component.
  double  m_gRatio;   // Ratio (0 - 1.0) of green component.
  double  m_bRatio;   // Ratio (0 - 1.0) of blue component.
};

/*********************** COLOR SENSOR *******************************/
// A ColorSensor is associated with a device capable of sensing colors.
//
// IMPORTANT: After constructing, but before using a ColorSensor object,
//            the following must be done (in order):
//
//         1. The setup() method must be called to initialize sensor
//            hardware (which uses an I2C interface).  If setup()
//            returns false, no sensor was found, so check the wiring.
//
//         2. Before reliable color readings can be taken, the sensor
//            MUST be calibrated using something "white" as a reference.
//            With the sensor positioned over the white surface
//            at the distance it will be used, call the
//            calibrateWhiteBalance(...) method, which will return
//            four scaling factors.  Then immediately call the
//            setWhiteBalance(...) method passing it the four factors.
//
// Usage: Once calibrated using a small piece of white tape, the
//        simplest way to take a sample is to call getTapeColor(),
//        which will sample the sensor and map the color ratios into
//        one of the recognized TapeColor enum values.
//
//        The sensor sample color ratios can be retrieved by calling
//        the getColorRatio(...) method.  It's then up to the caller
//        to perform further analysis of the color ratios to determine
//        which color is represented.

class ColorSensor
{
  public:
  
  // Supported sensor sample integration times.
  
  typedef enum
  {
    st2_4ms = 1,    // 2.4 milliseconds
    st24ms = 2,     // 24 milliseconds
    st50ms = 3      // 50 milliseconds
  } SampleTime;
  
  // Supported sensor amplifier gain values.
  
  typedef enum
  {
    g1x = 1,        // 1X gain
    g4x = 2,        // 4X gain
    g16x = 3,       // 16X gain
    g60x = 4        // 60X gain
  } Gain;
  
  public:  
  ColorSensor(SampleTime sampleTime = st2_4ms, Gain gain = g1x);
  
  // One-time setup of color sensor.  Returns "true" if successful.
  
  bool setup();
  
  // Take a color sample and return one of the recognized colors.
  
  TapeColor getTapeColor();
  
  // Returns current sensor color ratios.
  // Caller must analyze these ratios to determine the color sensed.
  
  void getColorRatios(ColorRatios& colorRatios);
  
  // Calibrate sensor "white balance" parameters.
  // Sensor should be positioned over standard white reference material.
  // Returns calculated calibration scaling factors.
  
  void calibrateWhiteBalance(double& cScale, double& rScale,
                             double& gScale, double& bScale);
  
  // Set the "white balance" scaling parameters.
                             
  void setWhiteBalance(double cScale, double rScale,
                       double gScale, double bScale);
                       
  // Perform manual "white" balance calibration.
  // Instructions are displayed using "smonitor".
  // Pauses wait for "button" to be clicked.
  
  void manualWhiteBalance(csci::SerialMonitor& smonitor,
                          csci::PushButton& button);

  // Display color balance scaling factors.
  
  void displayScalingFactors(csci::SerialMonitor& smonitor);
  
  private:
  // Associated Adafruit TCS34725 object and control params.
    
  Adafruit_TCS34725 m_tcs;
  uint8_t           m_tcsSampleTime;
  tcs34725Gain_t    m_tcsGain;
  
  // These are calibration scale factors which, when multiplied
  // times the raw sensor values reflected from our standard "white"
  // surface yield these color distribution ratios:
  //  
  // clear = 1.0
  // red = 0.33333
  // green = 0.33333
  // blue = 0.33333
  
  double  m_cScale;
  double  m_rScale;
  double  m_gScale;
  double  m_bScale;
};

}   // End namespace

#endif    // INCLUDE_CSCI_COLOR_SENSOR
