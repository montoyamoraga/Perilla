// controlar el brillo del led de la placa con una perilla

// conexiones:
// perilla: patitas de los extremos a 3V3 y a tierra (GND),
// patita del medio a gpio 26
// no conectes la perilla a 5V (VBUS o VSYS), solo a 3V3

#include "Perilla.h"

#include "pico/stdlib.h"
#include "hardware/pwm.h"

int main()
{
    // configurar el led de la placa para usar pwm
    gpio_set_function(PICO_DEFAULT_LED_PIN, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(PICO_DEFAULT_LED_PIN);
    // el pwm cuenta de 0 a 255
    pwm_set_wrap(slice, 255);
    pwm_set_enabled(slice, true);

    // crear una perilla en gpio 26 (entrada 0 del adc)
    Perilla perilla(26);

    // el adc de pico lee 12 bits, de 0 a 4095
    perilla.setRangoLeido(0, 4095);
    // el brillo va de 0 a 255
    perilla.setRangoMapeado(0, 255);

    while (true)
    {
        // leer la perilla y calcular el valor mapeado
        perilla.leer();

        // usar el valor mapeado como brillo del led
        pwm_set_gpio_level(PICO_DEFAULT_LED_PIN, perilla.getValorMapeado());

        sleep_ms(10);
    }
}
