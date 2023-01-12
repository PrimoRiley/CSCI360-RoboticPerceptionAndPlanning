// Utility Library switch classes implementation file.

#include "CSCIColorSensor.h"

namespace csci
{

// ---------------------------------------------------------
TapeColor ColorRatios::getTapeColor()
{
  // Note: Colors where r,g,b are nearly equal should be checked first.
  
  // *** Start of r,g,b nearly equal colors. ***
  
  if ( colorIsWhite() )
  {
    return TapeColor::white;
  }
    
  if ( colorIsBlack() )
  {
    return TapeColor::black;
  }
  
  if ( colorIsGray() )
  {
    return TapeColor::gray;
  }
  
  // *** End of r,g,b nearly equal colors. ***
  
  if ( colorIsRed() )
  {
    return TapeColor::red;
  }
  
  if ( colorIsGreen() )
  {
    return TapeColor::green;
  }

  if ( colorIsBlue() )
  {
    return TapeColor::blue;
  }
  
  if ( colorIsYellow() )
  {
    return TapeColor::yellow;
  }
  
  return TapeColor::unknown;
}

void ColorRatios::display(SerialMonitor& smonitor)
{
  smonitor.sendText(F("C_Ratio: "));
  smonitor.sendDoubleValue(getCRatio(),5);
  
  smonitor.sendText(F("    R_Ratio: "));
  smonitor.sendDoubleValue(getRRatio(),5);
  
  smonitor.sendText(F("    G_Ratio: "));
  smonitor.sendDoubleValue(getGRatio(),5);
  
  smonitor.sendText(F("    B_ratio: "));
  smonitor.sendDoubleValue(getBRatio(),5);
  
  smonitor.sendNewline();
}

bool ColorRatios::colorIsWhite()
{
  // Color is "white" if r, g and b ratios are almost equal AND
  // the clear intensity ratio is very high.
  
  return rgbAreClose() && valuesAreClose(getCRatio(), 1.0, 0.15);
}

bool ColorRatios::colorIsBlack()
{
  // Color is "black" if r, g and b ratios are almost equal AND
  // the clear intensity ratio is very low.
  
  return rgbAreClose(0.15) && valuesAreClose(getCRatio(), 0.05, 0.05);
}

bool ColorRatios::colorIsGray()
{
  // Color is "gray" if r, g and b ratios are almost equal AND
  // the clear intensity ratio is midrange.
  
  return rgbAreClose() && valuesAreClose(getCRatio(), 0.4, 0.15);
}

bool ColorRatios::colorIsRed()
{
  // Color is "red" if red ratio is above midrange AND green and blue
  // ratios are close to each other AND clear < 0.4.
  
  return valuesAreClose(getRRatio(), 0.75, 0.25) &&
         valuesAreClose(getGRatio(), getBRatio(), 0.1) &&
         valuesAreClose(getCRatio(), 0.2, 0.2);
}

bool ColorRatios::colorIsGreen()
{
  // Color is "green" if the green ratio is greater than 1.5 times
  // the red ratio AND the green and blue ratios are close to each
  // other AND the clear ratio < 0.4.
  
  return ( (getGRatio() / getRRatio()) >= 1.5 ) &&
         valuesAreClose(getGRatio(), getBRatio(), 0.1) &&
         valuesAreClose(getCRatio(), 0.2, 0.2 );
}

bool ColorRatios::colorIsBlue()
{
  // Color is "blue" if blue ratio is greater than 2.5 times
  // the red ratio AND clear ratio < 0.4.
  
  return ( (getBRatio() / getRRatio()) >= 2.5 ) &&
         valuesAreClose(getCRatio(), 0.2, 0.2 );
}

bool ColorRatios::colorIsYellow()
{
  // Color is "yellow" if red ration is greater than 2.5 times
  // the blue ratio and clear ratio is > 0.6.
   
  return ( (getRRatio() / getBRatio()) >= 2.5 ) &&
         valuesAreClose(getCRatio(), 0.8, 0.2 );
}

bool ColorRatios::rgbAreClose(double tolerance)
{
  return ( valuesAreClose(getRRatio(), getGRatio(), tolerance) &&
           valuesAreClose(getRRatio(), getBRatio(), tolerance) &&
           valuesAreClose(getRRatio(), getBRatio(), tolerance) );
}

bool ColorRatios::valuesAreClose
        (double value1, double value2, double tolerance)
{
  return ( abs(value2 - value1) <= tolerance );
}

// ---------------------------------------------------------
ColorSensor::ColorSensor(SampleTime sampleTime, Gain gain)
  : m_tcs(),  // Must call setup() to finish initialization.
  
    // Default scale factors are only approximate for the
    // default sampleTime = st2_4ms, gain = g1X, with battery
    // fully charged (i.e., brightest "clear" led).
  
    m_cScale(0.00325309),
    m_rScale(1.0785966),
    m_gScale(0.9758731),
    m_bScale(1.0669166)
{
  // Map our sample time into TCS34725 sensor sample time.
  
  m_tcsSampleTime = TCS34725_INTEGRATIONTIME_24MS;

  switch ( sampleTime )
  {
    case st2_4ms: m_tcsSampleTime = TCS34725_INTEGRATIONTIME_2_4MS; break;
    case st24ms:  m_tcsSampleTime = TCS34725_INTEGRATIONTIME_24MS;  break;
    case st50ms:  m_tcsSampleTime = TCS34725_INTEGRATIONTIME_50MS;  break;
  }
  
  // Map our gain value into TCS34725 gain value.
  
  m_tcsGain = tcs34725Gain_t::TCS34725_GAIN_1X;
  
  switch( gain )
  {
    case g1x:  m_tcsGain = tcs34725Gain_t::TCS34725_GAIN_1X;  break;
    case g4x:  m_tcsGain = tcs34725Gain_t::TCS34725_GAIN_4X;  break;
    case g16x: m_tcsGain = tcs34725Gain_t::TCS34725_GAIN_16X; break;
    case g60x: m_tcsGain = tcs34725Gain_t::TCS34725_GAIN_60X; break;        
  }
}
  
bool ColorSensor::setup()
{
  if ( !m_tcs.begin() )
  {
    return false;
  }
  
  m_tcs.setIntegrationTime(m_tcsSampleTime);
  m_tcs.setGain(m_tcsGain);
  
  // The color sensor library can return incorrect values the first
  // reading after initialization.  So, take some samples, but ignore them.
  
  for ( uint16_t sample = 1; sample <= 10; ++sample )
  {
    // Get raw color data from sensor
    
    uint16_t raw_c, raw_r, raw_g, raw_b;
    
    m_tcs.getRawData(&raw_r, &raw_g, &raw_b, &raw_c);
  }
  
  return true;
}

TapeColor ColorSensor::getTapeColor()
{
  ColorRatios ratios;
  
  getColorRatios(ratios);
  
  return ratios.getTapeColor();
}  

void ColorSensor::getColorRatios(ColorRatios& ratios)
{
  // Get raw color data from sensor
    
  uint16_t raw_c, raw_r, raw_g, raw_b;
    
  m_tcs.getRawData(&raw_r, &raw_g, &raw_b, &raw_c);
  
  // Calculate color ratios
  
  double clear = static_cast<double>(raw_c);
  
  ratios.setRRatio( ( static_cast<double>(raw_r) / clear ) * m_rScale );
  ratios.setGRatio( ( static_cast<double>(raw_g) / clear ) * m_gScale );
  ratios.setBRatio( ( static_cast<double>(raw_b) / clear ) * m_bScale );
  ratios.setCRatio( clear * m_cScale );
}

void ColorSensor::calibrateWhiteBalance(double& cScale, double& rScale,
                                        double& gScale, double& bScale)
{
  uint32_t sum_raw_c = 0, sum_raw_r = 0, sum_raw_g = 0, sum_raw_b = 0;
           
  // Number of raw samples to take
  
  const uint16_t num_samples = 50;           
  
  for ( uint16_t sample = 1; sample <= num_samples; ++sample )
  {
    // Get raw color data from sensor
    
    uint16_t raw_c, raw_r, raw_g, raw_b;
    
    m_tcs.getRawData(&raw_r, &raw_g, &raw_b, &raw_c);
    
    sum_raw_c += raw_c;
    sum_raw_r += raw_r;
    sum_raw_g += raw_g;
    sum_raw_b += raw_b;
  }
  
  // Compute average of raw data samples
  
  double num_colors = static_cast<double>(num_samples);
  
  double avg_c = sum_raw_c / num_colors;
  double avg_r = sum_raw_r / num_colors;
  double avg_g = sum_raw_g / num_colors;
  double avg_b = sum_raw_b / num_colors;
  
  // Return scaling factor so clear component = 1.0;
  
  cScale = 1.0 / avg_c;
  
  // Return scaling factors to balance red = green = blue = 1/3
  
  double one_third = (1.0 / 3.0);
  
  rScale = one_third / (avg_r * cScale);
  gScale = one_third / (avg_g * cScale);
  bScale = one_third / (avg_b * cScale);
}
  
void ColorSensor::setWhiteBalance(double cScale, double rScale,
                                  double gScale, double bScale)
{
  m_cScale = cScale;
  m_rScale = rScale;
  m_gScale = gScale;
  m_bScale = bScale;
}

void ColorSensor::manualWhiteBalance(csci::SerialMonitor& smonitor,
                                     csci::PushButton& button)
{
  smonitor.sendNewline();
  smonitor.sendText(F("Position color sensor over white tape."));
  smonitor.sendNewline();
  smonitor.sendText(F("Then click pushbutton to calibrate."));
  smonitor.sendNewline();

  button.waitForClick();

  double cScale, rScale, gScale, bScale;

  calibrateWhiteBalance(cScale, rScale, gScale, bScale);
  setWhiteBalance(cScale, rScale, gScale, bScale);

  // Display white balance scaling factors.

  displayScalingFactors(smonitor);
  
  // Read calibrated color ratios and display them.

  ColorRatios ratios;

  getColorRatios(ratios);

  smonitor.sendNewline();
  smonitor.sendText(F("New calibration ratios are:"));
  smonitor.sendNewline();
  smonitor.sendNewline();
  
  ratios.display(smonitor);
  
  smonitor.sendNewline();

  smonitor.sendText(F("Click pushbutton again to proceed..."));
  smonitor.sendNewline();
  smonitor.sendNewline();

  button.waitForClick();  
}                                       

void ColorSensor::displayScalingFactors(csci::SerialMonitor& smonitor)
{
  // Display white balance scaling factors

  smonitor.sendNewline();
  smonitor.sendText(F("New scaling ratios are:"));
  smonitor.sendNewline();
  
  smonitor.sendText(F("C_Scale: "));
  smonitor.sendDoubleValue(m_cScale,9);
  
  smonitor.sendText(F("    R_Scale: "));
  smonitor.sendDoubleValue(m_rScale,7);
  
  smonitor.sendText(F("    G_Scale: "));
  smonitor.sendDoubleValue(m_gScale,7);
  
  smonitor.sendText(F("    B_Scale: "));
  smonitor.sendDoubleValue(m_bScale,7);
  
  smonitor.sendNewline();  
}

}   // End namespace
