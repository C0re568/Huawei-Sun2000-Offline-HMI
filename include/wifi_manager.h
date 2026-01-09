#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include "huawei_inverter.h"

class WiFiManager {
public:
    static void begin();
    static void loop();
    static bool getConnectionStatus();

private:
    static AsyncWebServer server;
    static bool isConnected;
    
    static void setupWiFi();
    static void setupWebServer();
    static void handleRoot(AsyncWebServerRequest* request);
    static void handleConfigure(AsyncWebServerRequest* request);
    static void handleApiData(AsyncWebServerRequest* request);
    static void startCaptivePortal();
    static bool loadCredentials(String& ssid, String& password);
    static void saveCredentials(const String& ssid, const String& password);
};

#endif // WIFI_MANAGER_H