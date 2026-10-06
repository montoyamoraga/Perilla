// encender el led de la placa cuando la perilla
// pasa de la mitad de su recorrido

// conexiones:
// perilla: patitas de los extremos a 3V3 y a tierra (GND),
// patita del medio a gpio 26
// no conectes la perilla a 5V (VBUS o VSYS), solo a 3V3

#include "Perilla.h"

#include "pico/stdlib.h"

int main()
{
    // configurar el led de la placa como salida
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    // crear una perilla en gpio 26 (entrada 0 del adc)
    Perilla perilla(26);

    // el adc de pico lee 12 bits, de 0 a 4095
    perilla.setRangoLeido(0, 4095);
    // trabajar en porcentaje, de 0 a 100
    perilla.setRangoMapeado(0, 100);

    while (true)
    {
        // leer la perilla y calcular el valor mapeado
        perilla.leer();

        // encender el led si la perilla pasa del 50%
        if (perilla.getValorMapeado() > 50)
        {
            gpio_put(PICO_DEFAULT_LED_PIN, true);
        }
        else
        {
            gpio_put(PICO_DEFAULT_LED_PIN, false);
        }

        sleep_ms(10);
    }
}
