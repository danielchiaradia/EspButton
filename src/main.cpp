#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "Secrets.h"

ADC_MODE(ADC_VCC);

void setup()
{
  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);
  long st = millis();

  if (WiFi.SSID() != WIFI_SSID)
  {
    IPAddress local_IP(192, 168, 178, 49);
    IPAddress gateway(192, 168, 178, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.config(local_IP, gateway, subnet);
    WiFi.mode(WIFI_STA);
    WiFi.hostname(HOSTNAME);
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    
    WiFi.begin(WIFI_SSID, PASSWORD, 1, bssid);
    WiFi.persistent(true);
    WiFi.setAutoConnect(true);
  }

  if (WiFi.waitForConnectResult() == WL_CONNECTED)
  {
    HTTPClient http;
    http.begin(callback + ESP.getVcc() + "/" + (millis() - st));
    http.GET();
  }

  digitalWrite(3, HIGH);
}

void loop()
{
  // Set GPIO 3 to high should cut VCC anyways
  ESP.deepSleep(10000);
}