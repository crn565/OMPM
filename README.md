# Implementación de Nilmtk usando nuevo hardware basado en ESP32 sobre un bus RS485 con 6 medidores PZEM004

Por Carlos Rodriguez Navarro  

Febrero de 2023


***Este extracto es referido a  un paper de mi autoria  referida al OMPM publicada en la revista científica “Inventions:”   C. Rodríguez-Navarro, F. Portillo, F. Martínez, F. Manzano-Agugliaro, and A. Alcayde, “Development and Application of an Open Power Meter Suitable for NILM,” *Inventions*, vol. 9, no. 1, p. 2, Dec. 2023, doi: 10.3390/inventions9010002.***




**ABSTRACT:** *En el contexto de la creciente dependencia del sector energético mundial de los combustibles fósiles y la creciente preocupación por el medio ambiente, existe una necesidad urgente de avances en el monitoreo y la optimización de la energía. Para abordar este desafío, el presente estudio presenta el Open Multi Power Meter, una novedosa solución de hardware abierto diseñada para mediciones eléctricas eficientes y precisas. Este dispositivo está diseñado en torno a una arquitectura de microcontrolador único, con un conjunto completo de módulos de medición interconectados a través de un bus RS485, lo que garantiza una alta precisión y escalabilidad. Un aspecto importante de este desarrollo es la integración con el kit de herramientas de monitoreo de carga no intrusiva, que utiliza algoritmos avanzados para la desagregación de energía, incluida la optimización combinatoria y el modelo de Markov oculto finito. Se realizaron análisis comparativos utilizando conjuntos de datos públicos junto con monitores de hardware comercial y abierto para validar el diseño y las capacidades de este dispositivo. Estos estudios demuestran la notable efectividad del dispositivo, caracterizado por su simplicidad, flexibilidad y adaptabilidad en diversos escenarios de monitoreo de energía. La introducción de esta herramienta rentable y escalable marca una contribución al campo de la investigación energética, mejorando las prácticas de eficiencia energética. Esta investigación proporciona una solución práctica para la gestión de la energía y abre avances en el campo, destacando su impacto potencial tanto en la investigación académica como en las aplicaciones del mundo real.*

**Introducción**

A pesar de la gran cantidad de opciones disponibles de medidores inteligentes de consumo, existe una disparidad significativa en el rendimiento y la precisión de los monitores de energía en el mercado, ya que algunos ofrecen funciones esenciales y otros, como el oZm, brindan una alta precisión, pero necesitan más escalabilidad y capacidad de expansión.

En este contexto, este documento presenta el Open Multi Power Meter (OMPM), una solución para abordar estas brechas, particularmente en el campo de la monitorización de carga no intrusiva (NILM), que ofrece un equilibrio entre precisión, escalabilidad y facilidad de uso. El OMPM es una solución de hardware abierto cuyo firmware también ha si-do desarrollado en código abierto [11]. La naturaleza de hardware abierto del dispositivo no solo lo hace accesible a una gama más amplia de usuarios, sino que también fomenta la innovación y la personalización, lo que permite adaptarlo a necesidades operativas o de investigación específicas.

Un elemento central de la utilidad de OMPM es su compatibilidad con el kit de herramientas de monitoreo de carga no intrusiva (NILMTK) que emplea algoritmos avanzados para la desagregación de energía, un método que utiliza técnicas computacionales para estimar el uso de energía de los electrodomésticos individuales a partir de una sola lectura de medidor que registra la demanda total de energía. Los algoritmos de Optimización Combinatoria (CO) y Modelo de Markov Oculto Finito (FHMM) del NILMTK son particularmente hábiles para diseccionar patrones complejos de uso de energía, lo que los hace ideales para evaluar el rendimiento del OMPM. Al aprovechar estas herramientas, el OMPM puede proporcionar información detallada sobre el consumo de electricidad, lo que conduce a decisiones de gestión de la energía más informadas y a la optimización de la eficiencia, ya que, sin una retroalimentación directa, esperar que los consumidores participen activamente en un sistema energético sostenible y eficiente no es realista.

El análisis comparativo del OMPM y otros conjuntos de datos públicos, incluidas las mediciones de monitores comerciales y de hardware abierto, subraya la precisión y esca-labilidad del dispositivo. Los resultados de estas comparaciones validan la eficacia del OMPM y destacan su simplicidad y adaptabilidad, lo que lo convierte en una herramienta valiosa para una amplia gama de aplicaciones, desde la investigación académica hasta las soluciones prácticas de gestión de la energía.

La introducción del OMPM representa una contribución significativa a la medición inteligente y la eficiencia energética. Aborda las brechas críticas en las tecnologías actuales de monitoreo de energía y ofrece una solución escalable y rentable que puede adaptarse a las diversas necesidades de los sistemas energéticos modernos.

**El proyecto OMPM**

El OMPM es un desarrollo innovador en el campo de la tecnología de medición inteligente. Destaca por integrar una única arquitectura de microcontrolador con un conjunto de módulos de medida interconectados a través de un sistema de bus RS485, permitiendo la integración de múltiples módulos de medición de bajo coste según sea necesario. Este diseño garantiza un equilibrio entre la alta precisión en las mediciones eléctricas y la flexibilidad necesaria para la implementación a gran escala.

El nuevo hardware usado para la adquisición y captura de las medidas eléctricas se basa en los siguientes componentes:

-   ESP32 node MCU
-   6 x PZEM004
-   Lector de tarjetas SD
-   Tarjeta SD
-   Bus RS485
-   Conexión a Internet

El microcontrolador usado es un **ESP32 Node MCU**, al que se ha conectado un adaptador de tarjetas SD usando las líneas MISO /MOSI, CS y SCK del controlador. Es por tanto en la tarjeta SD donde almacenamos las medidas usando un fichero en formato CSV para cada medidor.

En la siguiente imagen podemos ver en detalle el conexionado del adaptador de SD al controlador ESP32.

![](media/95a984ea364d84b5722a0738826053ba.jpeg)

Ilustración 1-Detalle de conexiones lector SD

![Un cable conectado Descripción generada automáticamente con confianza baja](media/dde465bc04f80c26e51d6687c0b4138c.jpeg)

Ilustración 2- Detalle conexionado lector sd al ESP32

El módulo de medida PZEM004 mide las 5 características eléctricas fundamentales como son el voltaje RMS, corriente RMS, potencia Activa y la Energía contando con salidas opto-acopladas y comunicación en serie (viene con interfaz serial TTL, a través de varios terminales de comunicarse con la placa adaptadora, leer, y establecer los parámetros).

![Diagrama Descripción generada automáticamente](media/29c05c1b3c1e873fb0034f5742c08680.png)

Ilustración 3-Esquema de bloques PZEM004

En el montaje tenemos **6 módulos PZEM 004** con sus respectivas bobinas de Rogowsky que nos servirán para capturar las medidas de la intensidad para 6 dispositivos eléctricos. Respecto a las medidas de la tensión es tomada mediante cableado paralelo, que alimenta asimismo a los 6 módulos de medida.

Las medidas de tensión, Corriente, Potencia, Frecuencia y Factor de Potencia obtenidas por cada módulo son enviadas mediante las líneas RX y TX al controlador principal mediante el uso de un bus RS485.

Todo el conjunto se alimenta con +5 v DC directamente desde el propio bus USB dado que el consumo de la parte Rx/Tx de cada módulo PZEM004 es muy pequeño pues solo se requiere para alimentar a los optoacopladores de la parte de transmisión de cada módulo.

El módulo de medidor multi-función PZEM-004T permite medir el voltaje RMS, corriente RMS, potencia activa y energía que toma una carga conectada a una línea monofásica de 110 / 220V como por ejemplo un estufa, Nevera, motor, electrodoméstico, etc.... esta información puede ser enviada a un microcontrolador (por ejemplo Arduino o PIC), a un ordenador usando un adaptador USB a TTL, a un módulo WiFi (como en este caso usando un ESP32 ) o a un PLC.

Este es el esquema del circuito final que se ha implementado (el display es opcional):

![Diagrama, Esquemático Descripción generada automáticamente](media/1de64d7c0fd30edd30c24c10199d1c45.png)

Ilustración 4- Esquema del circuito

Como queda reflejado en el esquema superior, el Bus RS485 esta implementado mediante diodos Shockley rápidos en todas las líneas de transmisión de cada módulo PZEM004 y una resistencia común de 10K entre el positivo y dicha línea.

En cuanto al firmware del ESP32, para usar cada módulo PZEM004 antes debemos programar una dirección única para cada modulo para que cada uno sea identificado biunívocamente.

Estas son las direcciones de los contadores establecedoras individualmente:

-   uint8_t addr0=0x110; //1primer pzem es reconocido como 10 consumo agregado
-   uint8_t addr1=0x120; //2primer pzem es reconocido como 20 enchufe 1
-   uint8_t addr2=0x130; //2primer pzem es reconocido como 30 enchufe 2
-   uint8_t addr3=0x140; //3primer pzem es reconocido como 40 enchufe 3
-   uint8_t addr4=0x150; //4primer pzem es reconocido como 50 enchufe 4
-   uint8_t addr5=0x160; //5primer pzem es reconocido como 60 enchufe 5

En cuanto al software de adquisición al completo se adjunta en el anexo final. Resumidamente inicializamos la tarjeta SD, capturamos la fecha y hora actual mediante una conexión a la red y con ello creamos 6 ficheros para cada aplicativo (ver más abajo).

Una vez creados los ficheros le añadimos las cabeceras en la primera línea, lo cual nos va a servir para identificar las 5 medidas junto con el timestamp de 13 dígitos.

El cuerpo del programa principal va tomando de forma periódica todas las lecturas de cada contador, asegurándose antes de que cada contador esta accesible activo (de no estarlo pararía al siguiente). Lógicamente cada grupo de medidas es registrado en su correspondiente fichero junto el valor de la marca de tiempo correspondiente.

El programa supervisa continuamente el número total de medidas tomada para cada contador para asegurarse de que todos los contadores son accesibles y operativos.

Los dispositivos contemplados conectados a cada módulo PZEM004, cuyas medidas se introducirán en NILMTK para su análisis, son los siguientes:

1 - Contador de medición del consumo agregado

2 - Ventilador

3 - Ordenador portátil

4 - Lámpara Halógena

5 - Lámpara LED

6 - Monitor de Ordenador de 17"

Todas las medidas se capturan a una frecuencia aproximada superior a 1HZ. Estas son las especificaciones eléctricas de las medidas con el PZEM004:

**Voltaje:**

-   Rango de medición: 80 \~ 260 V
-   Resolución: 0.1 V
-   Precisión de medición: 0.5%

**Corriente:**

-   Rango de medición: 0 \~ 10A (PZEM-004T-10A); 0 \~ 100A (PZEM-004T-100A)
-   Corriente de medida inicial: 0.01A (PZEM-004T-10A); 0. 024 (PZEM-004T-100A)
-   Resolución: 0.001A
-   Precisión de la medición: 0.5%

**Potencia activa:**

-   Rango de medición: 0 \~ 2.3kW (PZEM-004T-10A); 0 \~ 23kW (PZEM-004T-100A)
-   Potencia de medida inicial: 0.4 W
-   Resolución: 0.1 W
-   Formato de visualización:
    -   \<1000 W, muestra un decimal, como: 999.9 W
    -   ≥ 1000 W, muestra solo un número entero, como: 1000 W
-   Precisión de la medición: 0.5%

**Factor de potencia**

-   Rango de medición: 0.00 \~ 1.00
-   Resolución: 0.01
-   Precisión de medición: 1%

**Frecuencia**

-   Rango de medición: 45 Hz \~ 65 Hz
-   Resolución: 0.1 Hz
-   Precisión de medición: 0.5%

**Energía activa:**

-   Rango de medición: 0 \~ 9999.99kWh
-   Resolución: 1 Wh
-   Precisión de medición: 0.5%
-   Formato de visualización:
    -   \<10kWh, la unidad de visualización es Wh (1kWh = 1000Wh), como: 9999Wh
    -   ≥ 10kWh, la unidad de visualización es kWh, como: 9999.99kWh

A continuación, se presenta imagen del montaje final:

![Imagen que contiene interior, tabla, escritorio, computadora Descripción generada automáticamente](media/dd535301d1c5d9509a0e598247596cf1.jpg)

# **Desagregación con NILMTK**

Tanto el código como yodos los resultados se han almacenado en el repositorio de Github actual en forma de cuadernos de Jupyter Notebook (para su ejecución se requiere tener instalado además de Jupyter Notebook el toolkit NILTK disponible asimismo en Github). Respecto al conjunto de datos también está disponible en este mismo repositorio en formato HDFS5(el fichero se llama dsualm2.h5). Asimismo en este mismo repositorio esta disponible el firmware para usar con el ESP32.

Basándonos en conversor DSUAL, se ha creado un nuevo conversor llamado UALM2 para generar el nuevo dataset formado por 5 medidas dado que no disponemos del calculo de la potencia reactiva ni aparente.

Este es el esquema del montaje obtenido en el paso 2:

![Un papalote volando en el cielo Descripción generada automáticamente con confianza media](media/4fac1900871270e673dd2b4433d696eb.png)

Esta es la fracción de consumo calculado para cada aparato:

![Gráfico, Gráfico circular Descripción generada automáticamente](media/994c0cc11436aa28dba703577b8d57ed.png)

Podemos representar gráficamente la tensión, frecuencia, potencia activa y la corriente para todos los aplicativos, como por ejemplo el agregado:

![Gráfico, Gráfico de barras Descripción generada automáticamente](media/e92be0d595add001a0098124e5838fd8.png)

Es muy interesante ver como quedan registrados todas las medidas, donde se aprecia como en la primera hora se registran las 32 posibilidades en orden secuencial y en la segunda hora ya fue aleatorio:

![Gráfico, Histograma Descripción generada automáticamente](media/5b2a0c53c52473bb4364cbc0ccc27be2.png)

Ilustración 5-Representación de la potencia de todos los contadores en la línea del tiempo

Como primer paso obtenemos una primera estimación del funcionamiento de los dos algoritmos, lo cual en principio nos adelanta que el tiempo de muestreo a seleccionar será pequeño:

![Tabla Descripción generada automáticamente](media/98d6726bbbb3caa779a2d02e64719d29.png)

Ilustración 6- Datos preliminares de ejecución

El resultado ya usando muchos más periodos de muestreo, los dos algoritmos CO y FHMM y los tres diferentes métodos de relleno nos da los siguientes resultados:

![Tabla Descripción generada automáticamente](media/685c4fa1d67c18be26675cbee8024419.png)

Ilustración 7 -Resultados ejecución tres métodos, dos algoritmos y tiempos

Como vemos, la combinación más eficiente es la del método combinatorio, método de relleno median y 30 segundos de muestreo.

En la imagen podemos ver como los resultados son bastante buenos si comparamos la lectura real con la estimación que nos da el algoritmo.

![Interfaz de usuario gráfica, Aplicación Descripción generada automáticamente](media/aa15521180568a9da8b61b135a16ed64.png)

Ilustración 8-Resultados método CO, 30 segundos, método median

La proporción de la energía desagregada es la siguiente usando CO, 30” y método median es la siguinte:

![Gráfico, Gráfico circular Descripción generada automáticamente](media/7f3b2e7bfb722da96a61d7a1f9c76550.png)

Ilustración 9- Tanto por ciento de la energía desagregada

Ahora veremos la comparación entre la señal real y la estimada, lo cual no son datos muy buenos.

![Gráfico, Gráfico circular Descripción generada automáticamente](media/a3e60b52977dbf89f475902489eba36e.png)

Ilustración 10- Comparación Datos reales y Predicciones

Finalmente, aplicando las métricas de NILMTK obtenemos los siguientes resultados para las medidas de F1, EAE, MNEAP y RMSE:

![Tabla Descripción generada automáticamente](media/c21600607d4593a579e547605867d813.png)

Ilustración 11-Resultados métricas

Para la métrica F1 se obtienen valores muy buenos para el ventilador, ordenador portátil (que mejora incluso a la lampara halógena) o la lampara LED. El dato más perjudicado es la TV, aunque gráficamente no se entiende ese valor.

![Interfaz de usuario gráfica, Gráfico, Aplicación, Gráfico de líneas Descripción generada automáticamente](media/d349c1948922f43aa96dc605411fa456.png)

La métrica MNEAP también nos da valores muy buenos siendo el mejor sin duda para la lámpara halógena. El peor dato vuelve a ser para la TV.

![Imagen que contiene Interfaz de usuario gráfica Descripción generada automáticamente](media/3a385dc52679ed386b45d91c39de9a6d.png)

# 

Sobre la métrica RMSE esta vez los datos son muy buenos para todos los aplicativos.

![Interfaz de usuario gráfica Descripción generada automáticamente con confianza baja](media/eb97b8255da1a33a72fce82a04fd6024.png)

# 

Ahora veamos el promedio l para las 4 métricas y los diferentes periodos de muestreo:

![Tabla Descripción generada automáticamente](media/b9dbd1f467a30c1f4ddda0c4cc273369.png)

Ahora veamos los diferentes resultados máximos para las 4 métricas y los diferentes aplicativos:

![Tabla Descripción generada automáticamente](media/f94c5322b359501847b3e8e26a5f4ca1.png)

Esta es la correspondencia de índice:

![Tabla Descripción generada automáticamente](media/f6ce4b00e3cf4f7b3b49c38448b6d337.png)

Comparación de las diferentes métricas con los diferentes métodos de relleno;

![Gráfico, Gráfico de barras Descripción generada automáticamente](media/2d3bb676a1d664c69412764d4d7f2522.png)

Destaca nuevamente el valor tan nefasto para la métrica F1 respecto a la TV usando los tres diferentes métodos de relleno.

Por último, veamos los resultados obtenidos para las diferentes métricas y los diferentes aplicativos observando nuevamente como para el ventilador no tenemos valores.

![Gráfico, Gráfico en cascada Descripción generada automáticamente](media/001728098313718d10e034f3c6aa5c54.png)

**Conclusiones**

Este estudio presenta una novedosa solución basada en hardware caracterizada por la escalabilidad, la asequibilidad y la replicabilidad, al tiempo que mantiene la alta precisión típica de las soluciones de nivel profesional. El sistema emplea software de código abierto tanto en el firmware del microcontrolador como en la fase de procesamiento. Este software de procesamiento se basa en el kit de herramientas NILMTK y está adaptado para dar cabida a un nuevo conjunto de datos generado por este nuevo hardware.

Un aspecto destacable de este trabajo es el desarrollo de un nuevo convertidor adaptado a los archivos de medición OMPM. Este convertidor crea un nuevo conjunto de datos que admite una marca de tiempo de 13 dígitos, lo que facilita la aplicación de las diversas fases de NILMTK, incluida la validación, el entrenamiento y la evaluación de métricas

.

Surgen diferencias significativas cuando se comparan los resultados obtenidos de la aplicación de métricas de NILMTK al conjunto de datos OMPM con los derivados del conjunto de datos DEPS (generado con hardware profesional). En particular, el conjunto de datos OMPM requiere tiempos de muestreo más cortos y exhibe una notable diferencia del 200% en la métrica RMSE en comparación con el conjunto de datos DEPS.

Los prometedores resultados obtenidos con el conjunto de datos OMPM utilizando métricas NILMTK abren nuevas posibilidades para que los investigadores generen sus conjuntos de datos y mejoren aún más la investigación de NILM. La escalabilidad de la solución propuesta, facilitada por la implementación de un bus RS485, permite el uso de múltiples canales con un solo microcontrolador. Esta escalabilidad garantiza la captura de todas las mediciones eléctricas fundamentales con una precisión encomiable. Después de haber sido evaluado con éxito con seis módulos, el número de circuitos en un hogar tí-pico, el sistema tiene el potencial de expansión futura para acomodar aún más módulos

.

Para evaluar este nuevo hardware, se eligieron aplicaciones con bajo consumo de energía para aumentar la complejidad de las tareas de desagregación. El hardware arrojó resultados altamente satisfactorios en varias métricas, lo que sugiere su utilidad potencial en la investigación en curso de NILM. El trabajo futuro podría centrarse en mejorar la precisión de los módulos de medición. Actualmente, cada módulo se alimenta directamente de la tensión de red mediante un circuito RC simple, un diodo rectificador y un diodo Zener, con un regulador U3 (7133) en la salida. Se podría lograr una mejora alimentando el regulador a partir de una fuente aislada e independiente, como un R05P125, que ofrece una dirección prometedora para una mayor investigación y desarrollo.

**Publicaciones**

Toda esta documentación referida al OMPM esta publicada en la revista científica “Inventions:”

-   C. Rodríguez-Navarro, F. Portillo, F. Martínez, F. Manzano-Agugliaro, and A. Alcayde, “Development and Application of an Open Power Meter Suitable for NILM,” *Inventions*, vol. 9, no. 1, p. 2, Dec. 2023, doi: 10.3390/inventions9010002.

Hay un artículo de mi autoría sobre el NILM pero que usa el hardware OZM en lugar del OMPM:

-   C. Rodriguez-Navarro, A. Alcayde, V. Isanbaev, L. Castro-Santos, A. Filgueira-Vizoso, and F. G. Montoya, “DSUALMH- A new high-resolution dataset for NILM,” *Renewable Energy and Power Quality Journal*, vol. 21, no. 1, pp. 238–243, Jul. 2023, doi: 10.24084/repqj21.286.


