#include "wifi_manager.h"

AsyncWebServer WiFiManager::server(80);
bool WiFiManager::isConnected = false;

void WiFiManager::begin() {
    // Initialize SPIFFS for credential storage
    if (!SPIFFS.begin(true)) {
        Serial.println("Failed to mount SPIFFS");
        return;
    }

    setupWiFi();
    setupWebServer();
}

void WiFiManager::setupWiFi() {
    String ssid, password;
    if (loadCredentials(ssid, password)) {
        WiFi.begin(ssid.c_str(), password.c_str());
        unsigned long startAttemptTime = millis();

        while (WiFi.status() != WL_CONNECTED) {
            if (millis() - startAttemptTime > 10000) {
                // Failed to connect, start captive portal
                startCaptivePortal();
                return;
            }
            delay(500);
            Serial.print(".");
        }

        isConnected = true;
        Serial.println("\nWiFi connected");
        Serial.println("IP address: " + WiFi.localIP().toString());
    } else {
        // No saved credentials, start captive portal
        startCaptivePortal();
    }
}

void WiFiManager::startCaptivePortal() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP("HuaweiInverter");
    Serial.println("Captive Portal Started");
}

void WiFiManager::setupWebServer() {
    server.on("/", HTTP_GET, handleRoot);
    server.on("/configure", HTTP_POST, handleConfigure);
    server.on("/api/data", HTTP_GET, handleApiData);

    server.begin();
}

void WiFiManager::handleRoot(AsyncWebServerRequest* request) {
    String html = R"(
<!DOCTYPE html>
<html>
<head>
    <title>Huawei Inverter WiFi Configuration</title>
</head>
<body>
    <h1>WiFi Configuration</h1>
    <form action="/configure" method="post">
        SSID: <input type="text" name="ssid"><br>
        Password: <input type="password" name="password"><br>
        <input type="submit" value="Save">
    </form>
</body>
</html>
)";
    request->send(200, "text/html", html);
}

void WiFiManager::handleConfigure(AsyncWebServerRequest* request) {
    if (request->hasParam("ssid", true) && request->hasParam("password", true)) {
        String ssid = request->getParam("ssid", true)->value();
        String password = request->getParam("password", true)->value();

        saveCredentials(ssid, password);
        request->send(200, "text/plain", "WiFi credentials saved. Rebooting...");
        delay(1000);
        ESP.restart();
    } else {
        request->send(400, "text/plain", "Missing SSID or password");
    }
}

void WiFiManager::handleApiData(AsyncWebServerRequest* request) {
    HuaweiInverter& inverter = HuaweiInverter::getInstance();

    // Create JSON response
    StaticJsonDocument<200> doc;
    doc["soc"] = inverter.getSoc();
    doc["input_power"] = inverter.getInputPower();
    doc["active_power"] = inverter.getActivePower();

    String jsonResponse;
    serializeJson(doc, jsonResponse);

    request->send(200, "application/json", jsonResponse);
}

void WiFiManager::loop() {
    // Reconnect logic if WiFi is disconnected
    if (isConnected && WiFi.status() != WL_CONNECTED) {
        isConnected = false;
        Serial.println("WiFi disconnected. Attempting to reconnect...");
        setupWiFi();
    }
}

bool WiFiManager::loadCredentials(String& ssid, String& password) {
    File credFile = SPIFFS.open("/wifi_creds.json", "r");
    if (!credFile) {
        Serial.println("Failed to open credentials file");
        return false;
    }

    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, credFile);
    credFile.close();

    if (error) {
        Serial.println("Failed to parse credentials");
        return false;
    }

    ssid = doc["ssid"].as<String>();
    password = doc["password"].as<String>();

    return !ssid.isEmpty() && !password.isEmpty();
}

void WiFiManager::saveCredentials(const String& ssid, const String& password) {
    File credFile = SPIFFS.open("/wifi_creds.json", "w");
    if (!credFile) {
        Serial.println("Failed to open credentials file for writing");
        return;
    }

    StaticJsonDocument<200> doc;
    doc["ssid"] = ssid;
    doc["password"] = password;

    serializeJson(doc, credFile);
    credFile.close();
}