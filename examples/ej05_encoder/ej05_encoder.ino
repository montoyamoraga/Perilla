// leer un encoder y mostrar en el monitor serial
// su valor de 0 a 100, la direccion del giro y los pasos acumulados

// conexiones:
// patita A (CLK) a la patita 2
// patita B (DT) a la patita 3
// comun del encoder a tierra (GND)
// no hace falta resistencia externa
// si el encoder tiene pulsador, ese contacto es un boton en otra patita

// incluir biblioteca
#include "Perilla.h"

// crear una perilla encoder en las patitas 2 y 3
Perilla perilla(2, 3, Perilla::ENCODER);

void setup()
{
  // abrir comunicacion serial
  Serial.begin(9600);

  // el valor va de 0 a 100, igual que con un potenciometro
  perilla.setRangoMapeado(0, 100);
  // cada clic suma o resta 2
  perilla.setSensibilidad(2);
}

void loop()
{
  // leer seguido: un giro que no se alcanza a ver no vuelve
  perilla.leer();

  if (perilla.getDireccion() == Perilla::HORARIO)
  {
    Serial.print("horario, valor: ");
    Serial.print(perilla.getValorMapeado());
    Serial.print(", pasos: ");
    Serial.println(perilla.getPasos());
  }
  else if (perilla.getDireccion() == Perilla::ANTIHORARIO)
  {
    Serial.print("antihorario, valor: ");
    Serial.print(perilla.getValorMapeado());
    Serial.print(", pasos: ");
    Serial.println(perilla.getPasos());
  }
}
