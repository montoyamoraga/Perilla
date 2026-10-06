// encender el led de la placa cuando la perilla
// pasa de la mitad de su recorrido

// conexiones:
// perilla: patitas de los extremos a 5V y a tierra (GND),
// patita del medio a A0

// incluir biblioteca
#include "Perilla.h"

// crear una perilla en la patita A0
Perilla perilla(A0);

void setup()
{
  // configurar el led de la placa como salida
  pinMode(LED_BUILTIN, OUTPUT);

  // configurar rango de lectura
  perilla.setRangoLeido(0, 1023);
  // trabajar en porcentaje, de 0 a 100
  perilla.setRangoMapeado(0, 100);
}

void loop()
{
  // leer la perilla y calcular el valor mapeado
  perilla.leer();

  // encender el led si la perilla pasa del 50%
  if (perilla.getValorMapeado() > 50)
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
  }
}
