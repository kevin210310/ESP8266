#include <CloudIoTCore.h>
int inPin = 4;    // pushbutton connected to digital pin 7
bool val = false;
int estado_rele;
const int RELE = D1;
#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

//Cliente NTP
#include <NTPClient.h>
#include <WiFiUdp.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

#include "esp8266_mqtt.h"
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(RELE, OUTPUT);
  pinMode(inPin, INPUT);
  setupCloudIoT(); // Creates globals for MQTT
  pinMode(LED_BUILTIN, OUTPUT);
  timeClient.begin();
  timeClient.setTimeOffset(0);
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
    Serial.print(medida());
    Serial.println(" -- "+getTime());

    //aqui va el objeto
    String data = 
    "{"
      "\"vibration\":" + String(medida()) + ","
      "\"onoff\":" + String(estado_rele) + ","
      "\"battery_level\": 0.8887897987,"
      "\"signal_level\": 0.84556346436,"
      "\"plate_temperature\": 5.76798789798,"
      "\"created_at_origin\": \"" + String(getTime()) + "\""
    "}";
    Serial.println(data);
    publishTelemetry(data);
  }
}
