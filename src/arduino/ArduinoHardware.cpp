#include "./../Hardware.h"

#include <Arduino.h>

namespace PerillaHardware
{

    void configurarEntradaAnaloga(uint8_t patita)
    {
        // en Arduino analogRead no necesita configuracion previa
        (void)patita;
    }

    uint16_t leerPatita(uint8_t patita)
    {
        return analogRead(patita);
    }
}
