#ifndef HUAWEI_INVERTER_H
#define HUAWEI_INVERTER_H

#include <Arduino.h>
#include <ModbusMaster.h>

class HuaweiInverter {
public:
    // Constructor
    HuaweiInverter(uint8_t slaveId = 1);

    // Initialize Modbus communication
    void begin();

    // Non-blocking update method to poll inverter data
    bool update();

    // Getter methods for latest polled values
    float getSoc() const { return soc; }
    float getInputPower() const { return inputPower; }
    float getActivePower() const { return activePower; }

    // Check if data is valid and recently updated
    bool isDataValid() const { return millis() - lastUpdateTime < DATA_VALID_TIMEOUT; }

private:
    // Modbus communication object
    ModbusMaster node;

    // Slave address for the Huawei inverter
    uint8_t slaveId;

    // Polled values
    float soc = 0.0f;           // State of Charge
    float inputPower = 0.0f;    // Input Power
    float activePower = 0.0f;   // Active Power

    // Timing and state tracking
    unsigned long lastUpdateTime = 0;
    unsigned long updateInterval = 5000;  // 5 seconds between updates
    bool isUpdating = false;

    // Modbus register addresses
    static const uint16_t SOC_REGISTER = 37760;
    static const uint16_t INPUT_POWER_REGISTER = 32064;
    static const uint16_t ACTIVE_POWER_REGISTER = 37113;

    // Timeout for considering data valid (ms)
    static const unsigned long DATA_VALID_TIMEOUT = 10000;

    // Internal method to process Modbus response
    bool processResponse(uint16_t response, float& value);

    // Error handling
    void handleModbusError(uint8_t result);
};

// Implementation in the header for template-like flexibility
HuaweiInverter::HuaweiInverter(uint8_t slaveId) : slaveId(slaveId) {}

void HuaweiInverter::begin() {
    // Configure Serial1 for Modbus communication
    Serial1.begin(9600, SERIAL_8N1, 43, 44);  // RX: 43, TX: 44
    node.begin(slaveId, Serial1);
}

bool HuaweiInverter::update() {
    // Non-blocking update method
    if (isUpdating) return false;

    // Check if it's time to update
    unsigned long currentTime = millis();
    if (currentTime - lastUpdateTime < updateInterval) return false;

    isUpdating = true;

    // Read SOC
    uint8_t result = node.readHoldingRegisters(SOC_REGISTER, 1);
    if (result == node.ku8MBSuccess) {
        processResponse(node.getResponseBuffer(0), soc);
    } else {
        handleModbusError(result);
        isUpdating = false;
        return false;
    }

    // Read Input Power
    result = node.readHoldingRegisters(INPUT_POWER_REGISTER, 1);
    if (result == node.ku8MBSuccess) {
        processResponse(node.getResponseBuffer(0), inputPower);
    } else {
        handleModbusError(result);
        isUpdating = false;
        return false;
    }

    // Read Active Power
    result = node.readHoldingRegisters(ACTIVE_POWER_REGISTER, 1);
    if (result == node.ku8MBSuccess) {
        processResponse(node.getResponseBuffer(0), activePower);
    } else {
        handleModbusError(result);
        isUpdating = false;
        return false;
    }

    // Update timing
    lastUpdateTime = currentTime;
    isUpdating = false;
    return true;
}

bool HuaweiInverter::processResponse(uint16_t response, float& value) {
    // Convert raw Modbus register value to meaningful float
    // Assumes the register represents the value directly
    value = static_cast<float>(response) / 10.0f;  // Adjust scaling as needed
    return true;
}

void HuaweiInverter::handleModbusError(uint8_t result) {
    // Graceful error handling
    switch (result) {
        case node.ku8MBIllegalFunction:
            Serial.println("Modbus Error: Illegal Function");
            break;
        case node.ku8MBIllegalDataAddress:
            Serial.println("Modbus Error: Illegal Data Address");
            break;
        case node.ku8MBIllegalDataValue:
            Serial.println("Modbus Error: Illegal Data Value");
            break;
        case node.ku8MBSlaveDeviceFailure:
            Serial.println("Modbus Error: Slave Device Failure");
            break;
        default:
            Serial.print("Modbus Error: Unknown (code ");
            Serial.print(result);
            Serial.println(")");
    }
}

#endif // HUAWEI_INVERTER_H