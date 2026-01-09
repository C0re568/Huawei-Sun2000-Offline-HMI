#ifndef HAL_GENERIC_ESP32_H
#define HAL_GENERIC_ESP32_H

#include <Arduino.h>

// Board-specific feature detection
#if defined(GENERIC_ESP32)
    // No default features for generic board
    // Features can be manually enabled via build flags
#endif

namespace HAL {
    class GenericESP32 {
    public:
        // Modbus Serial Pins (configurable)
        static constexpr int MODBUS_TX = 43;
        static constexpr int MODBUS_RX = 44;

        // Optional GPIO for status LED or user interaction
        static constexpr int STATUS_LED = 2;  // Default ESP32 onboard LED

        // Initialize board-specific hardware
        static void init() {
            // Initialize Modbus serial pins
            pinMode(MODBUS_TX, OUTPUT);
            pinMode(MODBUS_RX, INPUT);

            // Optional status LED
            pinMode(STATUS_LED, OUTPUT);
            digitalWrite(STATUS_LED, LOW);  // Turn off by default

            // Placeholder for any generic initialization
            // This can be expanded based on specific requirements
        }

        // Optional method to provide flexibility for custom configurations
        static void configurePin(int pin, int mode) {
            pinMode(pin, mode);
        }
    };
}

#endif // HAL_GENERIC_ESP32_H