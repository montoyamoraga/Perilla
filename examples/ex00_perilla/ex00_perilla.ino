
// include library
#include "Perilla.h"

// create an instance of Perilla on pin A0
Perilla perilla(A0);

void setup()
{
  // setup reading range
  perilla.setRangoLeido(0, 1023);
  // setup mapped range
  perilla.setRangoMapeado(0, 100);
}

void loop()
{
}
