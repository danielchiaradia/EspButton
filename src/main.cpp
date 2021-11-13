#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "Secrets.h"

ADC_MODE(ADC_VCC);

uint32_t getWifiChannel(String ssid)
{
  int networksFound = WiFi.scanNetworks();
  int i;
  for (i = 0; i < networksFound; i++)
  {
    if (ssid == WiFi.SSID(i))
    {
      return WiFi.channel(i);
    }
  }
  return -1;
}

void setupWiFi()
{
  IPAddress local_IP(192, 168, 178, 49);
  IPAddress gateway(192, 168, 178, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(local_IP, gateway, subnet);
  WiFi.mode(WIFI_STA);
  WiFi.hostname(HOSTNAME);
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);

  int channel = getWifiChannel(WIFI_SSID);
  WiFi.begin(WIFI_SSID, PASSWORD, channel, bssid);
  WiFi.persistent(true);
  WiFi.setAutoConnect(true);
}

void sendState(int start)  
{
  HTTPClient http;
  http.begin(callback + ESP.getVcc() + "/" + (millis() - start));
  http.GET();
}

void setup()
{
  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);
  long st = millis();

  if (WiFi.SSID() != WIFI_SSID)
  {
    setupWiFi();
  }

  if (WiFi.waitForConnectResult(6000) == WL_CONNECTED)
  {
    sendState(st);
  }
  else 
  {
    // channel changed
    setupWiFi();
    if (WiFi.waitForConnectResult(6000) == WL_CONNECTED)
    {
      sendState(st);
    }
  }

  digitalWrite(3, HIGH);
}

void loop()
{
  // Set GPIO 3 to high should cut VCC anyways
  ESP.deepSleep(10000);
}