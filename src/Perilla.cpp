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

    // sin filtro hasta que se llame a setFiltro()
    filtroActivo = false;
    filtroIniciado = false;
    porcentajeFiltro = 0;
    valorFiltradoEscalado = 0;

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
    if (tipo == ENCODER)
    {
        leerEncoder();
        return;
    }

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
