#ifndef HUAWEI_INVERTER_H
#define HUAWEI_INVERTER_H

#include <ModbusMaster.h>
#include <HardwareSerial.h>

class HuaweiInverter {
public:
    uint16_t soc;
    uint16_t pvPower;
    uint16_t gridPower;

    HuaweiInverter() {
        Serial1.begin(9600, SERIAL_8N1, 44, 43); // RX=44, TX=43
        modbus.begin(1, Serial1); // Slave ID 1
    }

    bool update() {
        bool success = true;

        // Read SOC (37760)
        uint8_t result = modbus.readHoldingRegisters(37760, 1);
        if (result == modbus.ku8MBSuccess) {
            soc = modbus.getResponseBuffer(0);
        } else {
            success = false;
        }

        // Read PV Power (32064)
        result = modbus.readHoldingRegisters(32064, 1);
        if (result == modbus.ku8MBSuccess) {
            pvPower = modbus.getResponseBuffer(0);
        } else {
            success = false;
        }

        // Read Grid Power (37113)
        result = modbus.readHoldingRegisters(37113, 1);
        if (result == modbus.ku8MBSuccess) {
            gridPower = modbus.getResponseBuffer(0);
        } else {
            success = false;
        }

        return success;
    }

private:
    ModbusMaster modbus;
};

#endif // HUAWEI_INVERTER_H