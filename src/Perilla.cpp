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

// un clic mecanico rebota unos pocos milisegundos
static const uint32_t tiempoAntirreboteEncoder = 5;

Perilla::Perilla(uint8_t nuevaPatita)
{
    iniciar(nuevaPatita, nuevaPatita, POTENCIOMETRO);
}

Perilla::Perilla(uint8_t nuevaPatita, Tipo nuevoTipo)
{
    // un encoder necesita dos patitas
    if (nuevoTipo == ENCODER)
    {
        nuevoTipo = POTENCIOMETRO;
    }

    iniciar(nuevaPatita, nuevaPatita, nuevoTipo);
}

Perilla::Perilla(uint8_t nuevaPatitaA, uint8_t nuevaPatitaB, Tipo nuevoTipo)
{
    if (nuevoTipo != ENCODER)
    {
        nuevoTipo = POTENCIOMETRO;
    }

    iniciar(nuevaPatitaA, nuevaPatitaB, nuevoTipo);
}

void Perilla::iniciar(uint8_t nuevaPatitaA, uint8_t nuevaPatitaB, Tipo nuevoTipo)
{
    patita = nuevaPatitaA;
    patitaB = nuevaPatitaB;
    tipo = nuevoTipo;

    valorLeido = 0;
    valorMapeado = 0;

    sensibilidad = 1;
    direccion = QUIETA;
    pasos = 0;
    // en reposo la patita A esta en alto, por la pull-up
    patitaAAnterior = true;
    esperandoAntirrebote = false;
    tiempoAnteriorClic = 0;

    // rangos iguales por defecto, asi el valor mapeado
    // es igual al valor leido hasta que se configuren
    setRangoLeido(0, 1023);
    setRangoMapeado(0, 1023);

    if (tipo == ENCODER)
    {
        PerillaHardware::configurarEntradaPullup(patita);
        PerillaHardware::configurarEntradaPullup(patitaB);
    }
    else
    {
        PerillaHardware::configurarEntradaAnaloga(patita);
    }
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

void Perilla::setSensibilidad(uint8_t pasosPorClic)
{
    if (pasosPorClic == 0)
    {
        pasosPorClic = 1;
    }

    sensibilidad = pasosPorClic;
}

void Perilla::leer()
{
    if (tipo == ENCODER)
    {
        leerEncoder();
        return;
    }

    valorLeido = PerillaHardware::leerPatita(patita);
    valorMapeado = mapear(valorLeido, valorLeidoMin, valorLeidoMax, valorMapeadoMin, valorMapeadoMax);
}

void Perilla::leerEncoder()
{
    direccion = QUIETA;

    bool lecturaA = PerillaHardware::leerPatitaDigital(patita);

    // flanco de bajada: en reposo A esta en alto y un clic la lleva a tierra
    if (!lecturaA && patitaAAnterior)
    {
        uint32_t ahora = PerillaHardware::tiempoActual();

        if (!esperandoAntirrebote || (ahora - tiempoAnteriorClic) >= tiempoAntirreboteEncoder)
        {
            // B en alto es horario. Si se siente al reves, se cambian las patitas.
            if (PerillaHardware::leerPatitaDigital(patitaB))
            {
                direccion = HORARIO;
                pasos += sensibilidad;
            }
            else
            {
                direccion = ANTIHORARIO;
                pasos -= sensibilidad;
            }

            esperandoAntirrebote = true;
            tiempoAnteriorClic = ahora;
        }
    }

    patitaAAnterior = lecturaA;
}

uint16_t Perilla::getValor()
{
    return valorLeido;
}
uint16_t Perilla::getValorMapeado()
{
    return valorMapeado;
}

Perilla::Direccion Perilla::getDireccion()
{
    return direccion;
}

int32_t Perilla::getPasos()
{
    return pasos;
}
