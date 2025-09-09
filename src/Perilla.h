#ifndef PERILLA_H
#define PERILLA_H

#include <Arduino.h>

class Perilla
{
public:
    Perilla(uint8_t nuevaPatita);
    void setPatita(uint8_t nuevaPatita);
    void setRangoLeido(uint16_t nuevoValorLeidoMin, uint16_t nuevoValorLeidoMax);
    void setRangoMapeado(uint16_t nuevoValorMapeadoMin, uint16_t nuevoValorMapeadoMax);
    void leer();
    uint16_t getValor();
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