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

// el filtro guarda su valor multiplicado por esta escala,
// para no perder los decimales al trabajar con enteros
static const int32_t escalaFiltro = 256;

// division entera que redondea al mas cercano en vez de truncar,
// asi el filtro no se queda pegado antes de llegar a la lectura
static int32_t dividirRedondeado(int32_t numerador, int32_t denominador)
{
    if (numerador >= 0)
    {
        return (numerador + denominador / 2) / denominador;
    }

    return (numerador - denominador / 2) / denominador;
}

Perilla::Perilla(uint8_t nuevaPatita)
{
    setPatita(nuevaPatita);

    PerillaHardware::configurarEntradaAnaloga(patita);

    valorLeido = 0;
    valorMapeado = 0;

    // sin filtro hasta que se llame a setFiltro()
    filtroActivo = false;
    filtroIniciado = false;
    porcentajeFiltro = 0;
    valorFiltradoEscalado = 0;

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
    // con 0 el valor quedaria quieto para siempre
    if (porcentaje < 1)
    {
        porcentaje = 1;
    }
    if (porcentaje > 100)
    {
        porcentaje = 100;
    }

    porcentajeFiltro = porcentaje;

    // al activarlo, la proxima leer() parte desde la lectura actual
    // y no desde cero. si ya estaba activo, solo cambia el porcentaje
    if (!filtroActivo)
    {
        filtroIniciado = false;
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
        int32_t lecturaEscalada = (int32_t)lectura * escalaFiltro;

        if (!filtroIniciado)
        {
            // primera lectura con filtro: partir desde ella
            valorFiltradoEscalado = lecturaEscalada;
            filtroIniciado = true;
        }
        else
        {
            // misma idea que valorFiltrado + porcentaje * (lectura - valorFiltrado),
            // en enteros, para no usar float
            int32_t diferencia = lecturaEscalada - valorFiltradoEscalado;
            valorFiltradoEscalado += dividirRedondeado(diferencia * porcentajeFiltro, 100);
        }

        valorLeido = (uint16_t)dividirRedondeado(valorFiltradoEscalado, escalaFiltro);
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
