#ifndef HAL_WAVESHARE_ESP32_S3_1_47_H
#define HAL_WAVESHARE_ESP32_S3_1_47_H

#include <Arduino.h>

// Board-specific feature detection
#if defined(WAVESHARE_ESP32_S3_1_47)
    #define HAS_DISPLAY
    #define HAS_SDCARD
    #define TFT_SPI
#endif

namespace HAL {
    class WaveshareESP32S3_1_47 {
    public:
        // Display SPI Pins
        static constexpr int TFT_MOSI  = 35;
        static constexpr int TFT_MISO  = 37;
        static constexpr int TFT_SCLK  = 36;
        static constexpr int TFT_CS    = 34;
        static constexpr int TFT_DC    = 33;
        static constexpr int TFT_RST   = 38;
        static constexpr int TFT_BL    = 39;

        // SD Card SPI Pins
        static constexpr int SD_MOSI   = 11;
        static constexpr int SD_MISO   = 13;
        static constexpr int SD_SCLK   = 12;
        static constexpr int SD_CS     = 10;

        // Modbus Serial Pins
        static constexpr int MODBUS_TX = 43;
        static constexpr int MODBUS_RX = 44;

        // Initialize board-specific hardware
        static void init() {
            #ifdef HAS_DISPLAY
            // Initialize display SPI pins
            pinMode(TFT_MOSI, OUTPUT);
            pinMode(TFT_MISO, INPUT);
            pinMode(TFT_SCLK, OUTPUT);
            pinMode(TFT_CS, OUTPUT);
            pinMode(TFT_DC, OUTPUT);
            pinMode(TFT_RST, OUTPUT);
            pinMode(TFT_BL, OUTPUT);
            #endif

            #ifdef HAS_SDCARD
            // Initialize SD card SPI pins
            pinMode(SD_MOSI, OUTPUT);
            pinMode(SD_MISO, INPUT);
            pinMode(SD_SCLK, OUTPUT);
            pinMode(SD_CS, OUTPUT);
            #endif

            // Initialize Modbus serial pins
            pinMode(MODBUS_TX, OUTPUT);
            pinMode(MODBUS_RX, INPUT);
        }
    };
}

#endif // HAL_WAVESHARE_ESP32_S3_1_47_H