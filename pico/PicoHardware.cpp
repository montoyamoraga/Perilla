#include "../src/Hardware.h"

#include "hardware/adc.h"

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

}
