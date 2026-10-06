#include "Perilla.h"

#include "Hardware.h"

// misma formula que map() de Arduino,
// para no depender de Arduino.h
static int32_t mapear(int32_t valor, int32_t entradaMin, int32_t entradaMax, int32_t salidaMin, int32_t salidaMax)
{
    // evitar division por cero si el rango de entrada esta vacio
    if (entradaMax == entradaMin)
    {
        return salidaMin;
    }

    return (valor - entradaMin) * (salidaMax - salidaMin) / (entradaMax - entradaMin) + salidaMin;
}

Perilla::Perilla(uint8_t nuevaPatita)
{
    setPatita(nuevaPatita);

    PerillaHardware::configurarEntradaAnaloga(patita);

    valorLeido = 0;
    valorMapeado = 0;

    // sin filtro hasta que se llame a setFiltro()
    filtroActivo = false;
    porcentajeFiltro = 0;
    valorFiltrado = 0;

    // rangos iguales por defecto, asi el valor mapeado
    // es igual al valor leido hasta que se configuren
    setRangoLeido(0, 1023);
    setRangoMapeado(0, 1023);
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

void Perilla::setFiltro(uint8_t porcentaje)
{
    if (porcentaje > 100)
    {
        porcentaje = 100;
    }

    porcentajeFiltro = porcentaje;

    // al activarlo, partir de la lectura actual y no desde cero.
    // si ya estaba activo, solo cambia el porcentaje
    if (!filtroActivo)
    {
        valorFiltrado = PerillaHardware::leerPatita(patita);
        filtroActivo = true;
    }
}

void Perilla::quitarFiltro()
{
    filtroActivo = false;
}

void Perilla::leer()
{
    uint16_t lectura = PerillaHardware::leerPatita(patita);

    if (filtroActivo)
    {
        // misma idea que valorFiltrado + porcentaje * (lectura - valorFiltrado),
        // en enteros, para no usar float
        int32_t diferencia = (int32_t)lectura - (int32_t)valorFiltrado;
        valorFiltrado = (uint16_t)((int32_t)valorFiltrado + (diferencia * porcentajeFiltro) / 100);
        valorLeido = valorFiltrado;
    }
    else
    {
        valorLeido = lectura;
    }

    valorMapeado = mapear(valorLeido, valorLeidoMin, valorLeidoMax, valorMapeadoMin, valorMapeadoMax);
}

uint16_t Perilla::getValor()
{
    return valorLeido;
}
uint16_t Perilla::getValorMapeado()
{
    return valorMapeado;
}
