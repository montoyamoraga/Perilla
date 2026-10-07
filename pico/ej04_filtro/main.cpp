// suavizar la lectura de una perilla con un filtro opcional
// y mostrar por usb su valor mapeado

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

    // cada leer() incorpora un 20% de la lectura nueva
    // y conserva un 80% del valor anterior
    perilla.setFiltro(20);

    // para volver a la lectura sin filtro:
    // perilla.quitarFiltro();

    while (true)
    {
        // leer la perilla. el filtro es una configuracion del objeto,
        // no un parametro de leer()
        perilla.leer();

        printf("mapeado: %u\n", perilla.getValorMapeado());

        sleep_ms(100);
    }
}
