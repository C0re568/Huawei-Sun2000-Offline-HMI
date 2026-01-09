#ifndef HUAWEI_INVERTER_H
#define HUAWEI_INVERTER_H

#include <Arduino.h>
#include <ModbusMaster.h>
#include "config.h"

class HuaweiInverter {
public:
    // Singleton pattern
    static HuaweiInverter& getInstance() {
        static HuaweiInverter instance;
        return instance;
    }

    void begin() {
        // Initialize Modbus communication
        node.begin(1, Serial1);  // Modbus address 1, Serial1
        Serial1.begin(9600, SERIAL_8N1, 43, 44);  // RX, TX pins
    }

    void update() {
        // Non-blocking update of inverter data
        static unsigned long lastUpdateTime = 0;
        unsigned long currentTime = millis();

        // Update every 5 seconds
        if (currentTime - lastUpdateTime >= 5000) {
            lastUpdateTime = currentTime;

            // Read SOC
            uint16_t rawSoc = readRegister(SOC_REGISTER);
            _soc = rawSoc / 10;  // Convert to percentage

            // Read Input Power
            uint16_t rawInputPower = readRegister(INPUT_POWER_REGISTER);
            _inputPower = rawInputPower;

            // Read Active Power
            uint16_t rawActivePower = readRegister(ACTIVE_POWER_REGISTER);
            _activePower = rawActivePower;
        }
    }

    // Getters
    int getSoc() const { return _soc; }
    int getInputPower() const { return _inputPower; }
    int getActivePower() const { return _activePower; }

private:
    // Private constructor for singleton
    HuaweiInverter() = default;

    // Prevent copying
    HuaweiInverter(const HuaweiInverter&) = delete;
    HuaweiInverter& operator=(const HuaweiInverter&) = delete;

    // Modbus node
    ModbusMaster node;

    // Inverter data
    int _soc = 0;
    int _inputPower = 0;
    int _activePower = 0;

    // Read a single Modbus register
    uint16_t readRegister(uint16_t reg) {
        uint8_t result = node.readHoldingRegisters(reg, 1);
        
        if (result == node.ku8MBSuccess) {
            return node.getResponseBuffer(0);
        } else {
            // Handle Modbus error
            Serial.print("Modbus read error for register ");
            Serial.print(reg);
            Serial.print(": ");
            Serial.println(result);
            return 0;
        }
    }
};

#endif // HUAWEI_INVERTER_H