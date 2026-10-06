// leer dos perillas al mismo tiempo,
// cada una es su propia instancia de Perilla

// conexiones:
// cada perilla: patitas de los extremos a 5V y a tierra (GND)
// perilla a: patita del medio a A0
// perilla b: patita del medio a A1

// abre el plotter serial (Serial Plotter) para ver
// las dos perillas como lineas

// incluir biblioteca
#include "Perilla.h"

// crear dos perillas
Perilla perillaA(A0);
Perilla perillaB(A1);

void setup()
{
  // abrir comunicacion serial
  Serial.begin(9600);

  // configurar las dos perillas en porcentaje
  perillaA.setRangoLeido(0, 1023);
  perillaA.setRangoMapeado(0, 100);
  perillaB.setRangoLeido(0, 1023);
  perillaB.setRangoMapeado(0, 100);
}

void loop()
{
  // leer cada perilla
  perillaA.leer();
  perillaB.leer();

  // imprimir en formato nombre:valor para el plotter serial
  Serial.print("a:");
  Serial.print(perillaA.getValorMapeado());
  Serial.print(",b:");
  Serial.println(perillaB.getValorMapeado());

  delay(50);
}
