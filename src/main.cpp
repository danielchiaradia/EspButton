#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "Secrets.h"

const String callback = "http://192.168.178.35:1880/buttons/m/";

ADC_MODE(ADC_VCC);

void setup()
{
  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);

  if (WiFi.SSID() != WIFI_SSID)
  {
    WiFi.hostname(HOSTNAME);
    WiFi.mode(WIFI_STA);

    IPAddress local_IP(192, 168, 178, 49);
    IPAddress gateway(192, 168, 178, 1);
    IPAddress subnet(255, 255, 255, 0);

    WiFi.config(local_IP, gateway, subnet);
    WiFi.begin(WIFI_SSID, PASSWORD);
    WiFi.persistent(true);
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
  }

  if (WiFi.waitForConnectResult() == WL_CONNECTED)
  {
    HTTPClient http;
    http.begin(callback + ESP.getVcc());
    http.GET();
    WiFi.disconnect(true);
  }

  digitalWrite(3, HIGH);
}

void loop()
{
  ESP.deepSleep(300);
}