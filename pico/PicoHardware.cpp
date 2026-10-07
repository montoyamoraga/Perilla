#include "../src/Hardware.h"

#include "hardware/adc.h"
#include "pico/stdlib.h"

namespace PerillaHardware
{

    void configurarEntradaAnaloga(uint8_t patita)
    {
        // iniciar el adc solo una vez,
        // aunque haya varias perillas
        static bool adcIniciado = false;
        if (!adcIniciado)
        {
            adc_init();
            adcIniciado = true;
        }

        adc_gpio_init(patita);
    }

    uint16_t leerPatita(uint8_t patita)
    {
        // en pico la patita es un gpio con adc,
        // por ejemplo gpio 26 es la entrada 0
        adc_select_input(patita - ADC_BASE_PIN);
        return adc_read();
    }

    void configurarEntradaPullup(uint8_t patita)
    {
        gpio_init(patita);
        gpio_set_dir(patita, GPIO_IN);
        gpio_pull_up(patita);
    }

    bool leerPatitaDigital(uint8_t patita)
    {
        return gpio_get(patita);
    }

    uint32_t tiempoActual()
    {
        return to_ms_since_boot(get_absolute_time());
    }

}
