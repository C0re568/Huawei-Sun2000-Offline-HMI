#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

#include "huawei_inverter.h"

// Forward declarations
extern HuaweiInverter inverter;

class WiFiManager {
private:
    AsyncWebServer server{80};
    const char* HOSTNAME = "huawei-inverter";
    const char* CONFIG_FILE = "/wifi_config.json";

    // WiFi credentials storage
    struct WiFiCredentials {
        String ssid;
        String password;
    } credentials;

    // Load WiFi credentials from SPIFFS
    bool loadCredentials() {
        if (!SPIFFS.begin(true)) {
            Serial.println("Failed to mount SPIFFS");
            return false;
        }

        File configFile = SPIFFS.open(CONFIG_FILE, "r");
        if (!configFile) {
            Serial.println("No saved WiFi credentials");
            return false;
        }

        StaticJsonDocument<256> doc;
        DeserializationError error = deserializeJson(doc, configFile);
        configFile.close();

        if (error) {
            Serial.println("Failed to parse WiFi config");
            return false;
        }

        credentials.ssid = doc["ssid"].as<String>();
        credentials.password = doc["password"].as<String>();
        return true;
    }

    // Save WiFi credentials to SPIFFS
    bool saveCredentials(const String& ssid, const String& password) {
        if (!SPIFFS.begin(true)) {
            Serial.println("Failed to mount SPIFFS");
            return false;
        }

        File configFile = SPIFFS.open(CONFIG_FILE, "w");
        if (!configFile) {
            Serial.println("Failed to open config file for writing");
            return false;
        }

        StaticJsonDocument<256> doc;
        doc["ssid"] = ssid;
        doc["password"] = password;

        if (serializeJson(doc, configFile) == 0) {
            Serial.println("Failed to write config file");
            configFile.close();
            return false;
        }

        configFile.close();
        return true;
    }

    // Setup captive portal routes
    void setupCaptivePortalRoutes() {
        server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
            String html = R"(
<!DOCTYPE html>
<html>
<head>
    <title>WiFi Configuration</title>
    <style>
        body { font-family: Arial, sans-serif; max-width: 400px; margin: 0 auto; padding: 20px; }
        input { width: 100%; margin: 10px 0; padding: 5px; }
        button { width: 100%; padding: 10px; }
    </style>
</head>
<body>
    <h1>WiFi Configuration</h1>
    <form action="/configure" method="post">
        <input type="text" name="ssid" placeholder="WiFi SSID" required>
        <input type="password" name="password" placeholder="WiFi Password" required>
        <button type="submit">Save Configuration</button>
    </form>
</body>
</html>
)";
            request->send(200, "text/html", html);
        });

        server.on("/configure", HTTP_POST, [this](AsyncWebServerRequest *request) {
            String ssid = request->arg("ssid");
            String password = request->arg("password");

            if (saveCredentials(ssid, password)) {
                String response = R"(
<!DOCTYPE html>
<html>
<head>
    <title>Configuration Saved</title>
    <meta http-equiv="refresh" content="5;url=/">
</head>
<body>
    <h1>WiFi Configuration Saved</h1>
    <p>Rebooting in 5 seconds...</p>
</body>
</html>
)";
                request->send(200, "text/html", response);
                delay(1000);
                ESP.restart();
            } else {
                request->send(500, "text/plain", "Failed to save configuration");
            }
        });
    }

    // Setup data API routes
    void setupDataRoutes() {
        server.on("/api/data", HTTP_GET, [](AsyncWebServerRequest *request) {
            StaticJsonDocument<256> doc;
            doc["soc"] = inverter.getSoc();
            doc["input_power"] = inverter.getInputPower();
            doc["active_power"] = inverter.getActivePower();
            doc["wifi_status"] = WiFi.status() == WL_CONNECTED ? "Connected" : "Disconnected";
            doc["ip_address"] = WiFi.localIP().toString();

            String jsonResponse;
            serializeJson(doc, jsonResponse);
            request->send(200, "application/json", jsonResponse);
        });
    }

public:
    void begin() {
        // Attempt to load saved credentials
        if (loadCredentials()) {
            WiFi.begin(credentials.ssid.c_str(), credentials.password.c_str());
            WiFi.setHostname(HOSTNAME);

            // Wait for connection with timeout
            unsigned long startAttemptTime = millis();
            while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
                delay(500);
                Serial.print(".");
            }
        }

        // If not connected, start AP mode
        if (WiFi.status() != WL_CONNECTED) {
            WiFi.mode(WIFI_AP_STA);
            WiFi.softAP("Huawei Inverter Config");
            Serial.println("Captive Portal Mode");
        } else {
            Serial.println("Connected to WiFi");
            Serial.println(WiFi.localIP());
        }

        // Setup mDNS
        if (!MDNS.begin(HOSTNAME)) {
            Serial.println("Error setting up mDNS");
        }

        // Setup web server routes
        setupCaptivePortalRoutes();
        setupDataRoutes();

        // Start server
        server.begin();
        MDNS.addService("http", "tcp", 80);
    }

    void loop() {
        // Reconnect logic if WiFi is disconnected
        if (WiFi.status() != WL_CONNECTED) {
            static unsigned long lastReconnectAttempt = 0;
            if (millis() - lastReconnectAttempt > 30000) {
                Serial.println("WiFi disconnected. Attempting to reconnect...");
                WiFi.reconnect();
                lastReconnectAttempt = millis();
            }
        }
    }
};

// Global WiFi Manager instance
WiFiManager wifiManager;

// Initialization function to be called in main.cpp
void initWiFi() {
    wifiManager.begin();
}

// Loop function to be called in main.cpp
void loopWiFi() {
    wifiManager.loop();
}