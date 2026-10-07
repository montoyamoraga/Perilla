// leer un encoder y mostrar por usb
// la direccion del giro y los pasos acumulados

// conexiones:
// patita A (CLK) a gpio 14
// patita B (DT) a gpio 15
// comun del encoder a tierra (GND)
// no hace falta resistencia externa
// no conectes el encoder a 5V (VBUS o VSYS)
// si el encoder tiene pulsador, ese contacto es un boton en otra patita

#include "Perilla.h"

#include <stdio.h>

#include "pico/stdlib.h"

int main()
{
    // abrir comunicacion serial por usb
    stdio_init_all();

    // crear una perilla encoder en gpio 14 y gpio 15
    Perilla perilla(14, 15, Perilla::ENCODER);

    // cada clic suma o resta 2 pasos
    perilla.setSensibilidad(2);

    while (true)
    {
        // leer seguido: un giro que no se alcanza a ver no vuelve
        perilla.leer();

        if (perilla.getDireccion() == Perilla::HORARIO)
        {
            printf("horario, pasos: %ld\n", (long)perilla.getPasos());
        }
        else if (perilla.getDireccion() == Perilla::ANTIHORARIO)
        {
            printf("antihorario, pasos: %ld\n", (long)perilla.getPasos());
        }

        sleep_ms(1);
    }
}
