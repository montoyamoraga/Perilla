// leer una perilla y mostrar por usb
// su valor leido y su valor mapeado a porcentaje

// conexiones:
// perilla: patitas de los extremos a 3V3 y a tierra (GND),
// patita del medio a gpio 26
// no conectes la perilla a 5V (VBUS o VSYS), solo a 3V3

#include "Perilla.h"

#include <stdio.h>

#include "pico/stdlib.h"

int main()
{
    // abrir comunicacion serial por usb
    stdio_init_all();

    // crear una perilla en gpio 26 (entrada 0 del adc)
    Perilla perilla(26);

    // el adc de pico lee 12 bits, de 0 a 4095
    perilla.setRangoLeido(0, 4095);
    // configurar rango mapeado, en porcentaje
    perilla.setRangoMapeado(0, 100);

    while (true)
    {
        // leer la perilla y calcular el valor mapeado
        perilla.leer();

        // imprimir valor leido y valor mapeado
        printf("valor: %u, mapeado: %u\n", perilla.getValor(), perilla.getValorMapeado());

        sleep_ms(100);
    }
}
