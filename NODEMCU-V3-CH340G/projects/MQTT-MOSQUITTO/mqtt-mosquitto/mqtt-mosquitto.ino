#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

// ─── Configuración WiFi ───────────────────────────────
const char* WIFI_SSID     = "<wifi-ssid>";
const char* WIFI_PASSWORD = "<wifi-password>";

// ─── Configuración Mosquitto ──────────────────────────
const char* MQTT_HOST     = "<mqtt-host>"; // IP de tu servidor Mosquitto
const int   MQTT_PORT     = 1883;            // Puerto MQTTs (TLS)
const char* MQTT_USER     = "<mqtt-user>";    // Si tienes auth activada
const char* MQTT_PASSWORD = "<mqtt-password>";
const char* MQTT_CLIENT   = "esp8266_client";

// ─── Topics ───────────────────────────────────────────
const char* TOPIC_PUB = "tnt001/st001/gw-002/telemetry";
const char* TOPIC_SUB = "tnt001/st001/gw002/command";

// ─── Objetos ──────────────────────────────────────────
WiFiClient   espClient;   // ← WiFiClient normal, sin Secure
PubSubClient client(espClient);

// ─── Callback: mensajes recibidos ─────────────────────
void callback(char* topic, byte* payload, unsigned int length) {
  String mensaje = "";
  for (unsigned int i = 0; i < length; i++) {
    mensaje += (char)payload[i];
  }

  Serial.print("Mensaje recibido [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(mensaje);

  if (String(topic) == TOPIC_SUB) {
    if (mensaje == "ON") {
      digitalWrite(LED_BUILTIN, LOW);
      Serial.println("LED encendido");
    } else if (mensaje == "OFF") {
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println("LED apagado");
    }
  }
}

// ─── Conexión WiFi ────────────────────────────────────
void conectarWiFi() {
  Serial.print("Conectando a WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado. IP: " + WiFi.localIP().toString());
}

// ─── Conexión MQTT ────────────────────────────────────
void conectarMQTT() {
  while (!client.connected()) {
    Serial.print("Conectando a MQTT...");
    if (client.connect(MQTT_CLIENT, MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("conectado!");
      client.subscribe(TOPIC_SUB);
      Serial.print("Suscrito a: ");
      Serial.println(TOPIC_SUB);
    } else {
      Serial.print("Error rc=");
      Serial.print(client.state());
      Serial.println(" — reintentando en 5s");
      delay(5000);
    }
  }
}

// ─── Setup ────────────────────────────────────────────
void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  conectarWiFi();

  client.setServer(MQTT_HOST, MQTT_PORT);
  client.setCallback(callback);

  conectarMQTT();
}

// ─── Loop ─────────────────────────────────────────────
unsigned long ultimaPublicacion = 0;
const long    intervalo         = 5000;

void loop() {
  if (!client.connected()) {
    conectarMQTT();
  }
  client.loop();

  unsigned long ahora = millis();
  if (ahora - ultimaPublicacion >= intervalo) {
    ultimaPublicacion = ahora;

    float temperatura = 22.5;
    String payload = String(temperatura);

    client.publish(TOPIC_PUB, payload.c_str());
    Serial.print("Publicado: ");
    Serial.println(payload);
  }
}