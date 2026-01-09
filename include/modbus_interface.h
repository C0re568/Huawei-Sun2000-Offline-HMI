#ifndef MODBUS_INTERFACE_H
#define MODBUS_INTERFACE_H

#include <Arduino.h>
#include <ModbusMaster.h>
#include "config.h"

// Modbus serial configuration
#define MODBUS_RX_PIN 43
#define MODBUS_TX_PIN 44
#define MODBUS_SERIAL_CONFIG SERIAL_8N1

// Inverter register definitions
#define INVERTER_SOC_REGISTER SOC_REGISTER
#define INVERTER_INPUT_POWER_REGISTER INPUT_POWER_REGISTER
#define INVERTER_ACTIVE_POWER_REGISTER ACTIVE_POWER_REGISTER

class HuaweiInverterInterface {
private:
    ModbusMaster _modbus;
    HardwareSerial& _serial;
    uint8_t _slaveAddress;
    
    bool readRegister(uint16_t registerAddress, float& value) {
        uint16_t rawValue[2];
        uint8_t result = _modbus.readHoldingRegisters(registerAddress, 2);
        
        if (result == _modbus.ku8MBSuccess) {
            // Combine two 16-bit registers into a 32-bit float
            rawValue[0] = _modbus.getResponseBuffer(0);
            rawValue[1] = _modbus.getResponseBuffer(1);
            
            // Assuming big-endian format, swap bytes
            uint32_t combinedValue = (rawValue[1] << 16) | rawValue[0];
            
            // Cast to float (be cautious with direct casting)
            memcpy(&value, &combinedValue, sizeof(float));
            return true;
        }
        
        return false;
    }

public:
    HuaweiInverterInterface(HardwareSerial& serial, uint8_t slaveAddress = INVERTER_SLAVE_ID)
        : _serial(serial), _slaveAddress(slaveAddress) {
        _modbus.begin(_slaveAddress, serial);
    }

    bool begin() {
        _serial.begin(MODBUS_BAUD_RATE, MODBUS_SERIAL_CONFIG, MODBUS_RX_PIN, MODBUS_TX_PIN);
        return true;
    }

    bool readSoC(float& socValue) {
        return readRegister(INVERTER_SOC_REGISTER, socValue);
    }

    bool readInputPower(float& inputPowerValue) {
        return readRegister(INVERTER_INPUT_POWER_REGISTER, inputPowerValue);
    }

    bool readActivePower(float& activePowerValue) {
        return readRegister(INVERTER_ACTIVE_POWER_REGISTER, activePowerValue);
    }

    // Error handling macro
    #define HANDLE_MODBUS_ERROR(error) \
        do { \
            if (!(error)) { \
                /* Log error or take appropriate action */ \
                return false; \
            } \
        } while(0)
};

#endif // MODBUS_INTERFACE_H