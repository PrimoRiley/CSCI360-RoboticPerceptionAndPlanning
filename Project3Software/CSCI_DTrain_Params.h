#ifndef INCLUDE_CSCI_DTRAIN_PARAMS
#define INCLUDE_CSCI_DTRAIN_PARAMS

namespace csci
{

  // Drive train movement adjustment multipliers.
  // These are experimentally determined for a specific drive train.
  
  double FBMultiplier = 36.0 / 35.5;      // Front-Back	
  double LRMultiplier = 36.0 / 32.75;     // Left-Right
  double DiagMultiplier = 36.0 / 34;      // Diagonal
  double SpinMultiplier = 360.0 / 355.0;  // Spin in place    

}   // End namespace

#endif    // INCLUDE_CSCI_DTRAIN_PARAMS
