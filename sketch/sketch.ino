
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

// =====================================================
// PROYECTO: Sistema de Iluminación y Confort Térmico
//          Adaptativo para Oficinas
// PLATAFORMA: ESP32 - Wokwi
// COMUNICACIÓN: MQTT mediante HiveMQ
// =====================================================

// -----------------------------------------------------
// 1. CONFIGURACIÓN WI-FI
// -----------------------------------------------------

const char* ssid = "Wokwi-GUEST";
const char* password = "";

// -----------------------------------------------------
// 2. CONFIGURACIÓN DEL BROKER MQTT
// -----------------------------------------------------

const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 8883;
const char* mqtt_topic = "oficina/prueba";

// -----------------------------------------------------
// 3. DEFINICIÓN DE PINES Y UMBRAL
// -----------------------------------------------------

const int PIN_LDR = 34;
const int PIN_PIR = 27;
const int PIN_LED = 25;
const int PIN_RELE = 26;

const int UMBRAL_LUZ = 2000;

// -----------------------------------------------------
// 4. OBJETOS DE COMUNICACIÓN
// -----------------------------------------------------

WiFiClientSecure espClient;
PubSubClient mqttClient(espClient);

// -----------------------------------------------------
// 5. CONEXIÓN WI-FI
// -----------------------------------------------------

void conectarWiFi() {

  Serial.println("Conectando a Wi-Fi...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Wi-Fi conectado");
  Serial.print("Direccion IP: ");
  Serial.println(WiFi.localIP());
}

// -----------------------------------------------------
// 6. CONEXIÓN MQTT
// -----------------------------------------------------

void conectarMQTT() {

  while (!mqttClient.connected()) {

    Serial.print("Conectando a HiveMQ...");

    if (mqttClient.connect("ESP32-Oficina-01")) {

      Serial.println(" conectado");

    } else {

      Serial.print(" fallo. Estado: ");
      Serial.println(mqttClient.state());

      delay(3000);
    }
  }
}

// -----------------------------------------------------
// 7. CONFIGURACIÓN INICIAL
// -----------------------------------------------------

void setup() {

  Serial.begin(115200);

  // Configurar entradas y salidas
  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_RELE, OUTPUT);

  // Mantener actuadores apagados al iniciar
  digitalWrite(PIN_RELE, LOW);
  digitalWrite(PIN_LED, LOW);

  // Conectar a Wi-Fi
  conectarWiFi();

  // Configuración TLS para la demostración.
  // No valida el certificado del servidor.
  espClient.setInsecure();

  // Configurar broker MQTT
  mqttClient.setServer(mqtt_server, mqtt_port);

  Serial.println("Sistema IoT iniciado");
}

// -----------------------------------------------------
// 8. BUCLE PRINCIPAL
// -----------------------------------------------------

void loop() {

  // Comprobar conexión MQTT
  if (!mqttClient.connected()) {
    conectarMQTT();
  }

  // Mantener comunicación MQTT
  mqttClient.loop();

  // ---------------------------------------------------
  // 8.1 LECTURA DE SENSORES
  // ---------------------------------------------------

  int lecturaLuz = analogRead(PIN_LDR);
  int movimiento = digitalRead(PIN_PIR);

  // ---------------------------------------------------
  // 8.2 CONTROL AUTOMÁTICO DE ILUMINACIÓN
  // ---------------------------------------------------

  if (lecturaLuz > UMBRAL_LUZ) {
    digitalWrite(PIN_LED, HIGH);
  } else {
    digitalWrite(PIN_LED, LOW);
  }

  // ---------------------------------------------------
  // 8.3 CONTROL DE CLIMATIZACIÓN MEDIANTE RELÉ
  // ---------------------------------------------------

  if (movimiento == HIGH) {
    digitalWrite(PIN_RELE, HIGH);
  } else {
    digitalWrite(PIN_RELE, LOW);
  }

  // ---------------------------------------------------
  // 8.4 LECTURA DE ESTADOS DE ACTUADORES
  // ---------------------------------------------------

  int estadoLED = digitalRead(PIN_LED);
  int estadoRele = digitalRead(PIN_RELE);

  // ---------------------------------------------------
  // 8.5 GENERACIÓN DEL MENSAJE JSON
  // ---------------------------------------------------

  char mensaje[150];

  snprintf(
    mensaje,
    sizeof(mensaje),
    "{\"luz\":%d,\"movimiento\":%d,\"led\":%d,\"rele\":%d}",
    lecturaLuz,
    movimiento,
    estadoLED,
    estadoRele
  );

  // ---------------------------------------------------
  // 8.6 PUBLICACIÓN DE DATOS MEDIANTE MQTT
  // ---------------------------------------------------

  mqttClient.publish(mqtt_topic, mensaje);

  // Mostrar telemetría en el monitor serial
  Serial.println("Datos enviados por MQTT:");
  Serial.println(mensaje);
  Serial.println("--------------------");

  // Intervalo de publicación: 2 segundos
  delay(2000);
}
