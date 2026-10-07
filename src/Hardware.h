#ifndef PERILLA_HARDWARE_H
#define PERILLA_HARDWARE_H

#include <stdint.h>

/**
 * \~spanish
 * @brief Funciones que tocan el hardware, con una implementacion por plataforma.
 *
 * Perilla.cpp solo usa estas funciones, asi no depende de ninguna plataforma.
 * Las implementaciones estan en src/arduino/ArduinoHardware.cpp y
 * pico/PicoHardware.cpp.
 *
 * \~english
 * @brief Functions that touch the hardware, with one implementation per platform.
 *
 * Perilla.cpp only uses these functions, so it does not depend on any platform.
 * The implementations are in src/arduino/ArduinoHardware.cpp and
 * pico/PicoHardware.cpp.
 */
namespace PerillaHardware
{

    /**
     * \~spanish
     * @brief Configura la patita como entrada analoga.
     * @param patita patita a configurar.
     *
     * \~english
     * @brief Configures the pin as an analog input.
     * @param patita pin to configure.
     */
    void configurarEntradaAnaloga(uint8_t patita);

    /**
     * \~spanish
     * @brief Lee el valor analogo de la patita.
     * @param patita patita a leer.
     * @return valor leido, su rango depende de la placa.
     *
     * \~english
     * @brief Reads the analog value of the pin.
     * @param patita pin to read.
     * @return read value, its range depends on the board.
     */
    uint16_t leerPatita(uint8_t patita);

    /**
     * \~spanish
     * @brief Configura la patita como entrada con resistencia pull-up interna.
     * @param patita patita a configurar.
     *
     * \~english
     * @brief Configures the pin as an input with the internal pull-up resistor.
     * @param patita pin to configure.
     */
    void configurarEntradaPullup(uint8_t patita);

    /**
     * \~spanish
     * @brief Lee el valor digital de la patita.
     * @param patita patita a leer.
     * @return `true` si esta en alto, `false` si esta en bajo.
     *
     * \~english
     * @brief Reads the digital value of the pin.
     * @param patita pin to read.
     * @return `true` if high, `false` if low.
     */
    bool leerPatitaDigital(uint8_t patita);
}

#endif
