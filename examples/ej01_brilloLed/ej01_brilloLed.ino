// controlar el brillo de un led con una perilla

// conexiones:
// perilla: patitas de los extremos a 5V y a tierra (GND),
// patita del medio a A0
// led: patita larga a la patita 9 con una resistencia de 220 ohm,
// patita corta a tierra (GND)

// la patita 9 tiene PWM en Arduino Uno,
// en otras placas usa cualquier patita con PWM

// incluir biblioteca
#include "Perilla.h"

// crear una perilla en la patita A0
Perilla perilla(A0);

// patita del led
const int patitaLed = 9;

void setup()
{
  // configurar la patita del led como salida
  pinMode(patitaLed, OUTPUT);

  // configurar rango de lectura
  perilla.setRangoLeido(0, 1023);
  // analogWrite recibe valores de 0 a 255
  perilla.setRangoMapeado(0, 255);
}

void loop()
{
  // leer la perilla y calcular el valor mapeado
  perilla.leer();

  // usar el valor mapeado como brillo del led
  analogWrite(patitaLed, perilla.getValorMapeado());
}
