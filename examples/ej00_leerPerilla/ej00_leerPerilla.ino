// leer una perilla y mostrar en el monitor serial
// su valor leido y su valor mapeado a porcentaje

// conexiones:
// perilla: patitas de los extremos a 5V y a tierra (GND),
// patita del medio a A0

// incluir biblioteca
#include "Perilla.h"

// crear una perilla en la patita A0
Perilla perilla(A0);

void setup()
{
  // abrir comunicacion serial
  Serial.begin(9600);

  // configurar rango de lectura,
  // Arduino Uno lee de 0 a 1023 (10 bits),
  // otras placas como ESP32 leen de 0 a 4095 (12 bits)
  perilla.setRangoLeido(0, 1023);
  // configurar rango mapeado, en porcentaje
  perilla.setRangoMapeado(0, 100);
}

void loop()
{
  // leer la perilla y calcular el valor mapeado
  perilla.leer();

  // imprimir valor leido y valor mapeado
  Serial.print("valor: ");
  Serial.print(perilla.getValor());
  Serial.print(", mapeado: ");
  Serial.println(perilla.getValorMapeado());

  delay(100);
}
