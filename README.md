
# Sistema de Iluminación y Confort Térmico Adaptativo para Oficinas

## 1. Descripción del proyecto

Este proyecto consiste en un prototipo IoT diseñado para optimizar el consumo energético en oficinas mediante la automatización de la iluminación y la activación de un sistema de climatización simulado.

El sistema utiliza un ESP32, un sensor de luminosidad LDR y un sensor de movimiento PIR. Los datos se transmiten mediante el protocolo MQTT hacia Node-RED, donde se visualizan en un Dashboard en tiempo real.

## 2. Objetivo

Desarrollar un sistema inteligente que permita controlar la iluminación según las condiciones de luminosidad y activar la climatización simulada cuando se detecte presencia en una oficina.

## 3. Componentes utilizados

- ESP32 DevKitC V4.
- Sensor de luminosidad LDR.
- Sensor de movimiento PIR.
- LED indicador.
- Módulo relé.
- Plataforma de simulación Wokwi.

## 4. Tecnologías utilizadas

- Arduino C/C++ para la programación del ESP32.
- MQTT para la comunicación de datos.
- HiveMQ como broker MQTT.
- Node-RED para el procesamiento y visualización.
- Dashboard 2.0 para el monitoreo.
- Wokwi para la simulación del circuito.

## 5. Arquitectura del sistema

1. Los sensores LDR y PIR recopilan información del entorno.
2. El ESP32 procesa las lecturas y controla los actuadores.
3. Los datos se publican mediante MQTT en el broker HiveMQ.
4. Node-RED recibe y procesa los mensajes.
5. El Dashboard presenta la luminosidad, el historial de lecturas, la detección de movimiento y el estado de los actuadores.

## 6. Lógica de funcionamiento

- Iluminación: cuando la lectura analógica del LDR supera el umbral configurado de 2000, se activa el LED. En caso contrario, permanece apagado.
- Presencia: cuando el sensor PIR detecta movimiento, se activa el relé que representa la habilitación de la climatización.
- Monitoreo: los estados y las lecturas se transmiten mediante MQTT cada 2 segundos.

**Nota:** La climatización se representa mediante un relé simulado activado por presencia. El prototipo no incorpora un sensor de temperatura.

## 7. Evidencias

### Circuito en Wokwi

![Circuito Wokwi](evidencias/Captura%20del%20circuito%20Wokwi.png)

### Dashboard de Node-RED

![Dashboard Node-RED](evidencias/Captura%20del%20Dashboard%20Node-RED.png)

## 8. Código fuente

El código del ESP32 se encuentra en la carpeta `sketch`.

## 9. Simulación

[Ver proyecto en Wokwi](https://wokwi.com/projects/479169865109240833)
