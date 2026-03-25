#include <CloudIoTCore.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

//Cliente NTP
#include <NTPClient.h>
#include <WiFiUdp.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
const int rele = 2;
int estado_rele;
#include "esp8266_mqtt.h"
void setup()
{
  pinMode(rele, OUTPUT);
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  setupCloudIoT(); // Creates globals for MQTT
  pinMode(LED_BUILTIN, OUTPUT);
  timeClient.begin();
  timeClient.setTimeOffset(0);
  digitalWrite(rele, HIGH);
}

static unsigned long lastMillis = 0;
void loop()
{
  if (!mqtt->loop())
  {
    mqtt->mqttConnect();
  }

  delay(10); // <- fixes some issues with WiFi stability

  // TODO: Replace with your code here
  if (millis() - lastMillis > 5000)
  {
    lastMillis = millis();
    String data = 
    "{"
      "\"carbon_dioxide\":" + String(medida()) + ","
      "\"onoff\":" + String(estado_rele) + "," 
      "\"battery\": 2.5,"
      "\"signal\": " + String(WiFi.RSSI()) + ""
    "}";
    Serial.println(data);
    publishTelemetry(data);
  }
}
