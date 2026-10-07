# Perilla

Biblioteca para leer perillas con microcontroladores, y mapear su valor a otro rango.

Una perilla puede ser un potenciómetro o un encoder. El tipo se elige al crearla.

Funciona con placas Arduino y con placas Raspberry Pi Pico, tanto desde Arduino como desde el Pico SDK.

## Conexiones

Un potenciómetro tiene tres patitas:

- una de las patitas de los extremos va a voltaje (5V o 3.3V, según la placa).
- la otra patita de los extremos va a tierra (GND).
- la patita del medio va a una entrada análoga.

En Raspberry Pi Pico usa 3.3V (patita `3V3`, nunca 5V) y una de las patitas con conversor análogo-digital: GPIO 26, 27 o 28.

## Uso con Arduino

Instala la biblioteca copiando esta carpeta en tu carpeta de bibliotecas de Arduino (por ejemplo `~/Documents/Arduino/libraries/Perilla`), o desde Arduino IDE con *Sketch → Include Library → Add .ZIP Library...*.

```cpp
#include "Perilla.h"

// perilla conectada a la patita A0
Perilla perilla(A0);

void setup()
{
  Serial.begin(9600);

  // Arduino Uno lee de 0 a 1023
  perilla.setRangoLeido(0, 1023);
  // convertir a porcentaje
  perilla.setRangoMapeado(0, 100);
}

void loop()
{
  perilla.leer();

  Serial.println(perilla.getValorMapeado());
}
```

Ejemplo en [examples/ej00_leerPerilla/](./examples/ej00_leerPerilla/).

## Uso con Raspberry Pi Pico SDK

La carpeta [pico/](./pico/) tiene un proyecto CMake que compila la biblioteca y un ejemplo para Pico 2. Necesitas el [Pico SDK](https://github.com/raspberrypi/pico-sdk) instalado, o la extensión Raspberry Pi Pico de VS Code.

```bash
cd pico
mkdir build
cd build
PICO_SDK_PATH=/ruta/a/pico-sdk cmake ..
cmake --build .
```

Esto genera un archivo `.uf2` por ejemplo, como `ej00_leerPerilla.uf2`. Para cargarlo, conecta la Pico manteniendo presionado el botón BOOTSEL y copia el archivo a la unidad que aparece.

En Pico, la perilla se lee de 0 a 4095 (12 bits), no de 0 a 1023 como en Arduino Uno.

Para otra placa, cambia `PICO_BOARD` en [pico/CMakeLists.txt](./pico/CMakeLists.txt) (por ejemplo `pico` para la Pico original).

Ejemplo completo en [pico/ej00_leerPerilla/main.cpp](./pico/ej00_leerPerilla/main.cpp).

## Ejemplos

Cada ejemplo existe para Arduino, en [examples/](./examples/), y para Pico SDK, en [pico/](./pico/). En Arduino la perilla va en A0, y en Pico en GPIO 26.

| Ejemplo | Qué hace |
| --- | --- |
| `ej00_leerPerilla` | imprime el valor leído y el valor mapeado |
| `ej01_brilloLed` | controla el brillo de un led (en Arduino un led externo en la patita 9, en Pico el led de la placa) |
| `ej02_umbral` | enciende el led de la placa cuando la perilla pasa de la mitad |
| `ej03_dosPerillas` | lee dos perillas a la vez, en formato para el plotter serial |
| `ej04_encoder` | lee un encoder: dirección del giro y pasos acumulados |

## Referencia

| Método | Descripción |
| --- | --- |
| `Perilla(uint8_t patita)` | crea un potenciómetro y configura la patita como entrada análoga |
| `Perilla(uint8_t patita, Tipo tipo)` | elige el tipo. `ENCODER` con una sola patita queda como potenciómetro |
| `Perilla(uint8_t patitaA, uint8_t patitaB, Tipo tipo)` | encoder en dos patitas digitales, con el común a tierra |
| `void setRangoLeido(uint16_t min, uint16_t max)` | rango que entrega la placa al leer un potenciómetro, por ejemplo `0, 1023` |
| `void setRangoMapeado(uint16_t min, uint16_t max)` | rango al que se convierte la lectura del potenciómetro, por ejemplo `0, 100` |
| `void setSensibilidad(uint8_t pasosPorClic)` | cuántos pasos vale cada clic del encoder. El valor inicial es 1. Un 0 se toma como 1 |
| `void leer()` | lee la perilla. En un encoder hay que llamarla en cada vuelta del loop |
| `uint16_t getValor()` | última lectura del potenciómetro, sin mapear. En un encoder queda en 0 |
| `uint16_t getValorMapeado()` | última lectura del potenciómetro, convertida al rango mapeado. En un encoder queda en 0 |
| `Direccion getDireccion()` | `HORARIO`, `ANTIHORARIO` o `QUIETA`, según la última `leer()` |
| `int32_t getPasos()` | pasos acumulados del encoder. Puede ser negativo |
| `void setPatita(uint8_t patita)` | cambia la patita guardada (no la vuelve a configurar) |

Si no configuras los rangos, ambos parten en `0, 1023`, así que `getValorMapeado()` entrega lo mismo que `getValor()`.

## Encoder

```cpp
Perilla perilla(2, 3, Perilla::ENCODER);

void setup()
{
  perilla.setSensibilidad(2);
}

void loop()
{
  perilla.leer();

  // getDireccion() es HORARIO, ANTIHORARIO o QUIETA
  // getPasos() es el total, y puede ser negativo
}
```

`setSensibilidad(2)` hace que cada clic sume o reste 2. Si no la llamas, cada clic vale 1. El pulsador del eje, si el encoder tiene uno, se lee como un botón en otra patita.

Ejemplo en [examples/ej04_encoder/](./examples/ej04_encoder/).

## Documentación

La documentación se genera con [Doxygen](https://www.doxygen.nl/) a partir de los comentarios en [src/](./src/), en español y en inglés, y se publica en <https://piruetasxyz.github.io/Perilla/>.

Para generarla en tu computador, en español:

```bash
(cat Doxyfile; echo "OUTPUT_LANGUAGE = Spanish"; echo "HTML_OUTPUT = es") | doxygen -
```

Queda en `build/docs/es/index.html`. Para inglés, usa `English` y `en`.

Cada comentario tiene una sección `\~spanish` y una sección `\~english`. Si agregas algo público sin documentar, la generación falla.

## Cómo está organizado

El código de [src/Perilla.cpp](./src/Perilla.cpp) no depende de ninguna plataforma. Todo lo que toca el hardware pasa por [src/Hardware.h](./src/Hardware.h), que tiene una implementación por plataforma:

- [src/arduino/ArduinoHardware.cpp](./src/arduino/ArduinoHardware.cpp): usa `analogRead`.
- [pico/PicoHardware.cpp](./pico/PicoHardware.cpp): usa las funciones `adc_*` del Pico SDK.

Para agregar otra plataforma, basta con escribir otra implementación de `Hardware.h`.

## Contenido

- [.github/](./.github): flujos de trabajo que compilan los ejemplos de Arduino y de Pico, revisan la biblioteca con Arduino Lint, y publican la documentación.
- [docs/](./docs/): página principal de la documentación y página para elegir idioma.
- [examples/](./examples/): ejemplos para Arduino.
- [Doxyfile](./Doxyfile): configuración de Doxygen.
- [pico/](./pico/): proyecto CMake, implementación y ejemplo para Pico SDK.
- [src/](./src/): código de la biblioteca.
- [keywords.txt](./keywords.txt): colores de sintaxis para Arduino IDE.
- [library.properties](./library.properties): metadatos para Arduino.
- [LICENSE](./LICENSE): licencia MIT.
- [README.md](./README.md): este documento.

## Versiones

- v0.1.0: octubre 2026, compatibilidad con Raspberry Pi Pico SDK, y valores por defecto para los rangos.
- v0.0.1: septiembre 2025, primera versión para Arduino.

## Licencia

MIT, ver [LICENSE](./LICENSE).
