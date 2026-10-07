// suavizar la lectura de una perilla con un filtro opcional
// y mostrar en el monitor serial su valor mapeado

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

  // cada leer() incorpora un 20% de la lectura nueva
  // y conserva un 80% del valor anterior
  perilla.setFiltro(20);

  // para volver a la lectura sin filtro:
  // perilla.quitarFiltro();
}

void loop()
{
  // leer la perilla. el filtro es una configuracion del objeto,
  // no un parametro de leer()
  perilla.leer();

  Serial.println(perilla.getValorMapeado());

  delay(100);
}
