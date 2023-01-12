#ifndef INCLUDE_CSCI_CORE
#define INCLUDE_CSCI_CORE

// CSCI Utility Library Core classes header file.

#include <Arduino.h>              // Basic Arduino declarations

namespace csci
{

// Passed to some routines to indicate whether they should wait (or not) for
// the operation to complete before returning.

static const bool Wait = true;
static const bool noWait = false;

// Class whose ctor performs any one-time initialization needed by the library.

// *** IMPORTANT *** The gcc compiler requires the UtilityInit ctor take an argument,
// otherwise, the ctor will NOT be called at startup before the program's "main" entry
// point is called.  Crazy stuff!!

class UtilityInit
{
   public:
   UtilityInit(int dummy);
};

}   // End namespace

#endif    // INCLUDE_CSCI_CORE
