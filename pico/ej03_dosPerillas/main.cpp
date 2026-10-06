// leer dos perillas al mismo tiempo,
// cada una es su propia instancia de Perilla

// conexiones:
// cada perilla: patitas de los extremos a 3V3 y a tierra (GND)
// perilla a: patita del medio a gpio 26
// perilla b: patita del medio a gpio 27
// no conectes las perillas a 5V (VBUS o VSYS), solo a 3V3

#include "Perilla.h"

#include <stdio.h>

#include "pico/stdlib.h"

int main()
{
    // abrir comunicacion serial por usb
    stdio_init_all();

    // crear dos perillas
    Perilla perillaA(26);
    Perilla perillaB(27);

    // configurar las dos perillas en porcentaje
    perillaA.setRangoLeido(0, 4095);
    perillaA.setRangoMapeado(0, 100);
    perillaB.setRangoLeido(0, 4095);
    perillaB.setRangoMapeado(0, 100);

    while (true)
    {
        // leer cada perilla
        perillaA.leer();
        perillaB.leer();

        // imprimir en formato nombre:valor
        printf("a:%u,b:%u\n", perillaA.getValorMapeado(), perillaB.getValorMapeado());

        sleep_ms(50);
    }
}
