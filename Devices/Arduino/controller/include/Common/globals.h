#ifndef GLOBALS_H
#define GLOBALS_H

#include "TTCInterface.h"

// Forward Declaration
class TTCInterface;

#ifdef DECLARE_GLOBALS

extern TTCInterface gTTCInterface;

#else

TTCInterface gTTCInterface;

#endif

#endif // GLOBALS_H