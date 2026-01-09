#ifndef CONFIG_H
#define CONFIG_H

// Display configuration
#ifdef LILYGO_T_DISPLAY_S3
#define HAS_DISPLAY
#endif

#ifdef WAVESHARE_ESP32_S3_1_47
#define HAS_DISPLAY
#endif

// Modbus Register Definitions
#define SOC_REGISTER 37760
#define INPUT_POWER_REGISTER 32064
#define ACTIVE_POWER_REGISTER 37113

// WiFi Configuration
#define WIFI_SSID_MAX_LENGTH 32
#define WIFI_PASSWORD_MAX_LENGTH 64

// Modbus Configuration
#define MODBUS_SLAVE_ADDRESS 1
#define MODBUS_BAUD_RATE 9600

#endif // CONFIG_H