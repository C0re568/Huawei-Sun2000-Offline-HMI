#include <Arduino.h>
#include "config.h"
#include "modbus_interface.h"

#ifdef HAS_DISPLAY
    #include <lvgl.h>
#endif

// Global Modbus interface
HuaweiInverterInterface* inverterInterface = nullptr;

void setup() {
    // Initialize serial for debugging
    Serial.begin(115200);

    // Initialize Modbus interface
    inverterInterface = new HuaweiInverterInterface(MODBUS_SERIAL_PORT);
    if (inverterInterface->begin()) {
        Serial.println("Modbus interface initialized successfully");
    } else {
        Serial.println("Failed to initialize Modbus interface");
    }

    #ifdef HAS_DISPLAY
        // Initialize LVGL
        lv_init();
        
        // TODO: Add display driver initialization specific to the board
        // This will depend on the specific display used (parallel or SPI)
    #endif
}

void loop() {
    if (inverterInterface) {
        float socValue, inputPower, activePower;
        
        if (inverterInterface->readSoC(socValue)) {
            Serial.print("State of Charge: ");
            Serial.print(socValue);
            Serial.println("%");
        }
        
        if (inverterInterface->readInputPower(inputPower)) {
            Serial.print("Input Power: ");
            Serial.print(inputPower);
            Serial.println(" W");
        }
        
        if (inverterInterface->readActivePower(activePower)) {
            Serial.print("Active Power: ");
            Serial.print(activePower);
            Serial.println(" W");
        }
    }

    #ifdef HAS_DISPLAY
        // TODO: Add LVGL display update logic
        // lv_task_handler();
    #endif

    delay(5000); // Update every 5 seconds
}

// Optional: Add error handling or interrupt handlers as needed