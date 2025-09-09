#include "Perilla.h"

Perilla::Perilla(uint8_t nuevaPatita)
{
    setPatita(nuevaPatita);
}

void Perilla::setPatita(uint8_t nuevaPatita)
{
    patita = nuevaPatita;
}

void Perilla::setRangoLeido(uint16_t nuevoValorLeidoMin, uint16_t nuevoValorLeidoMax)
{
    valorLeidoMin = nuevoValorLeidoMin;
    valorLeidoMax = nuevoValorLeidoMax;
}
void Perilla::setRangoMapeado(uint16_t nuevoValorMapeadoMin, uint16_t nuevoValorMapeadoMax)
{
    valorMapeadoMin = nuevoValorMapeadoMin;
    valorMapeadoMax = nuevoValorMapeadoMax;
}

void Perilla::leer()
{
    valorLeido = analogRead(patita);
    valorMapeado = map(valorLeido, valorLeidoMin, valorLeidoMax, valorMapeadoMin, valorMapeadoMax);
}

uint16_t Perilla::getValor()
{
    return valorLeido;
}
uint16_t Perilla::getValorMapeado()
{
    return valorMapeado;
}
