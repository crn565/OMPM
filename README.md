# Open-Multi-Power-Meter


## Implementación de Nilmtk  usando  nuevo hardware basado en ESP32 sobre un bus RS485 con 6 medidores PZEM004- 

En este repositorio mostramos  la mayoria de posibilidades de NILMTK  usando en lugar de hardware comercial, como primicia un hardware  muy  facil de obtener.

El hardware usado para la adquisicion y captura de las medidas eléctricas  se basa en  los siguientes componentes:

 - 1xx ESP32  node MCU.
 
 - 6 x PZEM004.
 
 - 1 x Lector  de tarjetas SD. 
 
 - 1 x Tarjeta SD 32GB clase 3.
 
 - 1 x Microinterruptor.
 
  - 1 x Bus RS485.
 
  - 1 x Display I2C  de 2 lineas  y 16 caracteres (recomendable pero puede omitirse). 
 
  -OPCIONAL: Conexión a Internet para la captura fecha y hora inicial.  
 
 El  microcontrolador  usado es un  ESP32 Node MCU,  al que se ha conectado  un adaptador de tarjetas  SD  usando las lineas MISO /MOSI, CS y SCK del controlador. Por tanto, e  en la tarjeta SD  donde almacenaremos de forma opcional las medidas de los diferentes modulos PZEM004 usando para ello un fichero individual en formato CSV  para cada medidor ( es decir, en nuestro experimento tendremos 6 ficheros de medida: 5 de los aplicativos y 1 correspondiente al consumo agregado). 
 
 Adicionalmente  al controlador con su modulo de memoria tenemos  6 modulos PZEM 004 conectados al controlador mediante un bus RS485. Cada modulo cuenta  con su respectiva bobinas de Rogowsky  que nos servira para capturar las medidas de la intensidad  para  los 6 dispositivos electricos.
 
 La medida de la tension es tomada  mediante cableado paralelo que alimenta asimismo a los  6 módulos  de medida.  
 
 Las medidas de Tension, Corriente, Potencia, Frecuencia  y  Factor de Potencia  obtenidas por cada módulo, son enviadas mediante las lineas RX  y TX al controlador principal mediante  el uso  de un bus RS485.
 
 Todo el conjunto se alimenta con +5 v DC  directamente desde el propio bus  USB. dado que el consumo de la parte rx/tx de cada módulo PZEM004 es muy pequeño pues sólo se requiere para alimentar a los optoacopladores de la parte de transmision de cada modulo.
 
 
 El modulo de medidor multi-función PZEM-004T permite medir el voltaje RMS, corriente RMS, potencia activa y energía que toma una carga conectada a una linea monofasica de 110 / 220V como por ejemplo un estufa, Nevera, motor, electrodomestico, etc.... Esta informacion puede ser enviada a un microcontrolador (por ejemplo podria ser un Arduino,PIC, un ordenador usando un adaptador USB a TTL,un PLC, o  a un modulo con WiFi, como en este caso es  un ESP32.

Este módulo cuenta ademas con salidas optoacopladas, alarma de sobrecarga, almacenamiento de valores cuando se corta la luz, y boton de reset. Asimismo cuenta  con comunicación serie, viniendo equipado con un interfaz serial TTL,  de modo que a través de cuatro terminales se puede comunicar con la placa adaptadora, leer, y establecer los parámetros midiendo Voltaje RMS, Corriente RMS, Potencia Activa y Energia.

 Los rangos de medida:

   1. Potencia, rango: 0 a 22kW

   -  De 0 a 10kW, el formato es 0.000 a 9.999

   -  De 10 a 22kW, el formato es 10.00 a 22.00

2. Energía, rango: 0 a 9999kWh

  -  De 0 a 10kWh, el formato es 0.000 a 9.999

   - De 10 a 100kWh, el formato es 10.00 a 99.99

   - De 100 a 1000kWh, el formato es 100.0 a 999.9

   - De 1000 a 9999kWh, el formato es 1000 a 9999

3. Voltaje, rango: 80 a 260VAC

  - El formato es 110.0 ~ 220.0.

4. Corriente, rango : 0 a 100A

 - El formato es 00.00 a 99.99
 
 
 Los dispositivos contemplados  conectados a cada modulo PZEM004, cuyas  medidas  se introduciran en NILMTK para su analisis,  son los siguientes:
   
   1 - Contador  de medición del consumo agregado..
   
   2- Soldador eléctrico.
   
   3 - Lámpara LED.
   
   4 - Lampara Halógena.
   
   5 - Ordenador portatil.
   
   6 - Ventilador
   
  
   
 
   
   
   
   
   
   
##   CARASTERICTICAS DEL  PZEM-004T-10A: Rango de medición 10A (derivación incorporada)


PZEM-004T-100A: Rango de medición 100A (transformador externo)

1. Descripción de la función

1.1 Voltage

1.1.1 Rango de medición: 80 ~ 260 V
1.1.2 Resolución: 0.1 V
1.1.3. Precisión de medición: 0.5%

1.2 actual

1.2.1 Rango de medición: 0 ~ 10A (PZEM-004T-10A); 0 ~ 100A (PZEM-004T-100A)
1.2.2 Corriente de medida inicial: 0.01A (PZEM-004T-10A); 0. 024 (PZEM-004T-100A)
1.2.3. Resolución: 0.001A
1.2.4 Precisión de la medición: 0.5%

1.3 Potencia activa

1.3.1 Rango de medición: 0 ~ 2.3kW (PZEM-004T-10A); 0 ~ 23kW (PZEM-004T-100A)
1.3.2 Potencia de medida inicial: 0.4 W
1.3.3. Resolución: 0.1 W
1.3.4 Formato de visualización:
<1000 W, muestra un decimal, como: 999.9 W
≥ 1000 W, muestra solo un número entero, como: 1000 W
1.3.5 Precisión de la medición: 0.5%

1.4 Factor de potencia

1.4.1. Rango de medición: 0.00 ~ 1.00
1.4.2 Resolución: 0.01
1.4.3. Precisión de medición: 1%

Frecuencia 1.5

1.5.1 Rango de medición: 45 Hz ~ 65 Hz
1.5.2 Resolución: 0.1 Hz
1.5.3. Precisión de medición: 0.5%

1.6 Energía activa

1.6.1. Rango de medición: 0 ~ 9999.99kWh
1.6.2 Resolución: 1 Wh
1.6.3. Precisión de medición: 0.5%
1.6.4 Formato de visualización:
<10kWh, la unidad de visualización es Wh (1kWh = 1000Wh), como: 9999Wh
≥ 10kWh, la unidad de visualización es kWh, como: 9999.99kWh
1.6.5 Restablecer energía: use el software para restablecer.

1.7 Alarma de sobrecarga

El umbral de potencia activa se puede configurar, cuando la potencia activa medida supera el umbral, puede alarmar

1.8 Interfaz de comunicación

Interfaz RS485.

2 Protocolo de comunicación

2.1 Protocolo de capa física

La capa física utiliza la interfaz de comunicación UART a RS485 .
La velocidad en baudios es 9600, 8 bits de datos, 1 bit de parada, sin paridad

2.2 Protocolo de la capa de aplicación

La capa de aplicación utiliza el protocolo Modbus-RTU para comunicarse. En la actualidad, solo admite códigos de función como 0x03 (Leer registro de retención), 0x04 (Leer registro de entrada), 0x06 (Escribir registro único), 0x41 (Calibración), 0x42 (Restablecer energía) .etc.

El código de función 0x41 es solo para uso interno (la dirección puede ser solo 0xF8), se usa para la calibración de fábrica y el regreso a las ocasiones de mantenimiento de fábrica, después del código de función para aumentar la contraseña de 16 bits, la contraseña predeterminada es 0x3721

El rango de direcciones del esclavo es 0x01 ~ 0xF7. La dirección 0x00 se utiliza como dirección de transmisión, el esclavo no necesita responder al maestro. La dirección 0xF8 se usa como dirección general, esta dirección solo se puede usar en un entorno de esclavo único y se puede usar para operaciones de calibración, etc.

2.3. Leer el resultado de la medición

El formato de comando del maestro lee el resultado de la medición es (total de 8 bytes):

Dirección esclava + 0x04 + Dirección de registro Byte alto + Dirección de registro Byte bajo + Número de registros Byte alto + Número de registros Byte bajo + Comprobación CRC Byte alto + Comprobación CRC Byte bajo.

El formato de comando de la respuesta del esclavo se divide en dos tipos:


Respuesta correcta: Dirección esclava + 0x04 + Número de bytes + Registro 1 Byte alto de datos + Byte bajo de datos del registro 1 +… + Comprobación de CRC Byte alto + Comprobación de CRC Byte bajo

Respuesta de error: dirección esclava + 0x84 + código anormal + comprobación CRC byte alto + comprobación CRC byte bajo

Código anormal analizado de la siguiente manera (lo mismo a continuación)

0x01, función ilegal
0x02, dirección ilegal
0x03, datos ilegales
0x04, error de esclavo
El registro de los resultados de la medición se organiza como la siguiente tabla

registro de los resultados de la medición

Por ej.ample, el maestro envía el siguiente comando (el código de verificación CRC se reemplaza por 0xHH y 0xLL, lo mismo a continuación)

0x01 + 0x04 + 0x00 + 0x00 + 0x00 + 0x0A + 0xHH + 0xLL

Indica que el maestro necesita leer 10 registros con la dirección de esclavo 0x01 y la dirección de inicio del registro es 0x0000

La respuesta correcta del esclavo es la siguiente:

0x01 + 0x04 + 0x14 + 0x08 + 0x98 + 0x03 + 0xE8 + 0x00 + 0x00 + 0x08 + 0x98 + 0x00 + 0x00 + 0x00 + 0x00 + 0x00 + 0x00 + 0x01 + 0xF4 + 0x00 + 0x64 + 0x00 + 0x00 + 0xHH + 0xLL

Los datos anteriores muestran

voltage es 0x0898, convertido a decimal es 2200, muestra 220.0V
La corriente es 0x000003E8, convertida a decimal es 1000, muestra 1.000A
La potencia es 0x00000898, convertida a decimal es 2200, muestra 220.0W
La energía es 0x00000000, convertida a decimal es 0, muestra 0Wh
La frecuencia es 0x01F4, convertida a decimal es 500, muestra 50.0Hz
El factor de potencia es 0x0064, convertido a decimal es 100, muestra 1.00
El estado de la alarma es 0x0000, indica que la potencia actual es menor que el umbral de potencia de la alarma
2.4 Leer y modificar los parámetros del esclavo

En la actualidad, solo admite la lectura y modificación de la dirección del esclavo y el umbral de alarma de potencia

El registro está organizado como la siguiente tabla

El registro está organizado como la siguiente tabla


El formato de comando del maestro para leer los parámetros del esclavo y leer los resultados de la medición es el mismo (descrito en detalles en la Sección 2.3), solo es necesario cambiar el código de función de 0x04 a 0x03

El formato de comando del maestro para modificar los parámetros del esclavo es (total de 8 bytes):

Dirección esclava + 0x06 + Dirección de registro Byte alto + Dirección de registro Byte bajo + Valor de registro Byte alto + Valor de registro Byte bajo + Comprobación de CRC Byte alto + Comprobación de CRC Byte bajo.

El formato de comando de la respuesta del esclavo se divide en dos tipos:

Respuesta correcta: dirección esclava + 0x06 + Número de bytes + Dirección de registro Byte bajo + Valor de registro Byte alto + Valor de registro Byte bajo + Comprobación de CRC Byte alto + Comprobación de CRC Byte bajo.

Respuesta de error: Dirección de esclavo + 0x86 + Código anormal + Comprobación de CRC byte alto + Comprobación de CRC byte bajo.

Por ej.ample, el maestro establece el umbral de alarma de potencia del esclavo:

0x01 + 0x06 + 0x00 + 0x01 + 0x08 + 0xFC + 0xHH + 0xLL

Indica que el maestro necesita configurar el registro 0x0001 (umbral de alarma de potencia) en 0x08FC (2300W).

Configurado correctamente, el esclavo vuelve a los datos que se envían desde el maestro.

Por ej.ample, el maestro establece la dirección del esclavo

0x01 + 0x06 + 0x00 + 0x02 + 0x00 + 0x05 + 0xHH + 0xLL

Indica que el maestro necesita configurar el registro 0x0002 (dirección Modbus-RTU) en 0x0005

Configurado correctamente, el esclavo vuelve a los datos que se envían desde el maestro.

2.5 Restablecer energía

El formato de comando del maestro para restablecer el esclavo energía es (4 bytes en total):

Dirección de esclavo + 0x42 + comprobación CRC byte alto + comprobación CRC byte bajo.

Respuesta correcta: dirección de esclavo + 0x42 + control CRC byte alto + control CRC byte bajo.

Respuesta de error: dirección esclava + 0xC2 + código anormal + comprobación de CRC byte alto + comprobación de CRC byte bajo

2.6 Calibración

El formato de comando del maestro para calibrar el esclavo es (6 bytes en total):

0xF8 + 0x41 + 0x37 + 0x21 + CRC comprobar byte alto + CRC comprobar byte bajo.

Respuesta correcta: 0xF8 + 0x41 + 0x37 + 0x21 + CRC comprobar byte alto + CRC comprobar byte bajo.

Respuesta de error: 0xF8 + 0xC1 + Código anormal + Comprobación de CRC byte alto + Comprobación de CRC byte bajo.

Cabe señalar que la calibración toma de 3 a 4 segundos, después de que el maestro envía el comando, si la calibración es exitosa, tomará de 3 a 4 segundos recibir la respuesta del esclavo.

2.7. Verificación CRC

La verificación CRC usa formato de 16 bits, ocupa dos bytes, el polinomio generador es X16 + X15 + X2 +1, el valor del polinomio usado para el cálculo es 0xA001.

El valor de la verificación CRC es una trama de datos que divide todos los resultados de la verificación de todos los bytes excepto el valor de verificación CRC.

