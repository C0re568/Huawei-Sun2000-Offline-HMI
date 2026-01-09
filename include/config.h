#ifndef CONFIG_H
#define CONFIG_H

// Conditional compilation flags
#ifdef LILYGO_T_DISPLAY_S3
    #define HAS_DISPLAY
    #define BOARD_NAME "LILYGO T-Display-S3"
#endif

#ifdef WAVESHARE_ESP32_S3_1_47
    #define HAS_DISPLAY
    #define BOARD_NAME "Waveshare ESP32-S3 1.47"
#endif

#ifdef GENERIC_ESP32
    #define BOARD_NAME "Generic ESP32"
#endif

// Modbus configuration
#define MODBUS_SERIAL_PORT Serial1
#define MODBUS_BAUD_RATE 9600

// Inverter communication parameters
#define INVERTER_SLAVE_ID 1
#define SOC_REGISTER 37760
#define INPUT_POWER_REGISTER 32064
#define ACTIVE_POWER_REGISTER 37113

// Error handling
#define MAX_MODBUS_RETRIES 3
#define MODBUS_TIMEOUT_MS 1000

#endif // CONFIG_H