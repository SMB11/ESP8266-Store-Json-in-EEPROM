

#include <ESP8266WiFi.h>

// #include <Wire.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <variables.h>
#include <StreamUtils.h>
#include "EEPROMAnything.h"
#include <EEPROM_tasks.h>
#include <PubSubClient.h>

#include <Mqtt_connect.h>


void setup() {
  Serial.begin(115200);
  setup_wifi();
  mqtt_init();
  EEPROM_init();

}

void loop() {
   mqtt_loop();

}