#ifndef PERILLA_H
#define PERILLA_H

#include <stdint.h>

/**
 * \~spanish
 * @brief Lee una perilla (potenciometro) conectada a una entrada analoga,
 * y mapea su valor a otro rango.
 *
 * Por defecto ambos rangos son de 0 a 1023, asi que getValorMapeado()
 * entrega lo mismo que getValor() hasta que se configuren.
 *
 * \~english
 * @brief Reads a knob (potentiometer) connected to an analog input,
 * and maps its value to another range.
 *
 * By default both ranges are 0 to 1023, so getValorMapeado() returns the
 * same as getValor() until they are configured.
 */
class Perilla
{
public:
    /**
     * \~spanish
     * @brief Crea la perilla y configura la patita como entrada analoga.
     * @param nuevaPatita patita donde esta conectada la patita del medio de la perilla.
     *
     * \~english
     * @brief Creates the knob and configures the pin as an analog input.
     * @param nuevaPatita pin the knob's middle leg is connected to.
     */
    Perilla(uint8_t nuevaPatita);

    /**
     * \~spanish
     * @brief Cambia la patita guardada, sin volver a configurarla.
     * @param nuevaPatita nueva patita.
     *
     * \~english
     * @brief Changes the stored pin, without configuring it again.
     * @param nuevaPatita new pin.
     */
    void setPatita(uint8_t nuevaPatita);

    /**
     * \~spanish
     * @brief Configura el rango que entrega la placa al leer.
     *
     * Por ejemplo de 0 a 1023 en Arduino Uno, o de 0 a 4095 en Raspberry Pi Pico.
     *
     * @param nuevoValorLeidoMin valor minimo leido.
     * @param nuevoValorLeidoMax valor maximo leido.
     *
     * \~english
     * @brief Configures the range the board returns when reading.
     *
     * For example 0 to 1023 on Arduino Uno, or 0 to 4095 on Raspberry Pi Pico.
     *
     * @param nuevoValorLeidoMin minimum read value.
     * @param nuevoValorLeidoMax maximum read value.
     */
    void setRangoLeido(uint16_t nuevoValorLeidoMin, uint16_t nuevoValorLeidoMax);

    /**
     * \~spanish
     * @brief Configura el rango al que se convierte la lectura.
     *
     * Por ejemplo de 0 a 100 para porcentaje, o de 0 a 255 para el brillo de un led.
     *
     * @param nuevoValorMapeadoMin valor minimo mapeado.
     * @param nuevoValorMapeadoMax valor maximo mapeado.
     *
     * \~english
     * @brief Configures the range the reading is converted to.
     *
     * For example 0 to 100 for a percentage, or 0 to 255 for an LED's brightness.
     *
     * @param nuevoValorMapeadoMin minimum mapped value.
     * @param nuevoValorMapeadoMax maximum mapped value.
     */
    void setRangoMapeado(uint16_t nuevoValorMapeadoMin, uint16_t nuevoValorMapeadoMax);

    /**
     * \~spanish
     * @brief Lee la patita y calcula el valor mapeado.
     *
     * \~english
     * @brief Reads the pin and calculates the mapped value.
     */
    void leer();

    /**
     * \~spanish
     * @brief Ultima lectura, sin mapear.
     * @return valor dentro del rango leido.
     *
     * \~english
     * @brief Last reading, not mapped.
     * @return value within the read range.
     */
    uint16_t getValor();

    /**
     * \~spanish
     * @brief Ultima lectura, convertida al rango mapeado.
     * @return valor dentro del rango mapeado.
     *
     * \~english
     * @brief Last reading, converted to the mapped range.
     * @return value within the mapped range.
     */
    uint16_t getValorMapeado();

private:
    uint8_t patita;
    uint16_t valorLeido;
    uint16_t valorLeidoMin;
    uint16_t valorLeidoMax;
    uint16_t valorMapeado;
    uint16_t valorMapeadoMin;
    uint16_t valorMapeadoMax;
};

#endif