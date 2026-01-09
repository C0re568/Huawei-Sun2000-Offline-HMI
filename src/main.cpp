#include <Arduino.h>
#include <config.h>
#include "huawei_inverter.h"

#ifdef HAS_DISPLAY
#include "ui.h"
#endif

#include "wifi_manager.h"

void setup() {
    Serial.begin(115200);

    // Initialize Modbus Inverter
    HuaweiInverter& inverter = HuaweiInverter::getInstance();
    inverter.begin();

    // Initialize WiFi
    WiFiManager::begin();

#ifdef HAS_DISPLAY
    // Initialize UI
    UI::begin();
#endif
}

void loop() {
    // Non-blocking updates
    static unsigned long lastUpdate = 0;
    unsigned long currentMillis = millis();

    // Modbus update every 5 seconds
    if (currentMillis - lastUpdate >= 5000) {
        HuaweiInverter::getInstance().update();
        lastUpdate = currentMillis;

#ifdef HAS_DISPLAY
        // Update UI with latest inverter data
        UI::update();
#endif
    }

    // WiFi and web server handling
    WiFiManager::loop();

#ifdef HAS_DISPLAY
    // Update status indicators
    static unsigned long lastStatusUpdate = 0;
    if (currentMillis - lastStatusUpdate >= 1000) {
        // Update WiFi status
        ConnectionStatus wifiStatus = WiFiManager::getConnectionStatus() ? CONNECTED : DISCONNECTED;
        UI::updateWiFiStatus(wifiStatus);

        // Update Modbus status (assuming a method exists in HuaweiInverter to check connection)
        ConnectionStatus modbusStatus = HuaweiInverter::getInstance().isConnected() ? CONNECTED : DISCONNECTED;
        UI::updateModbusStatus(modbusStatus);

        lastStatusUpdate = currentMillis;
    }
#endif
}