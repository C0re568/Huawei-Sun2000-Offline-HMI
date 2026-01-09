#ifndef HAL_LILYGO_T_DISPLAY_S3_H
#define HAL_LILYGO_T_DISPLAY_S3_H

#include <Arduino.h>

// Board-specific feature detection
#if defined(LILYGO_T_DISPLAY_S3)
    #define HAS_DISPLAY
    #define HAS_TOUCH
    #define TFT_PARALLEL_8_BIT
#endif

namespace HAL {
    class LilygoTDisplayS3 {
    public:
        // Pin Definitions
        static constexpr int TFT_CS   = 6;
        static constexpr int TFT_DC   = 7;
        static constexpr int TFT_RST  = 5;
        static constexpr int TFT_WR   = 8;
        static constexpr int TFT_RD   = 9;
        static constexpr int TFT_D0   = 39;
        static constexpr int TFT_D1   = 40;
        static constexpr int TFT_D2   = 41;
        static constexpr int TFT_D3   = 42;
        static constexpr int TFT_D4   = 45;
        static constexpr int TFT_D5   = 46;
        static constexpr int TFT_D6   = 47;
        static constexpr int TFT_D7   = 48;
        static constexpr int TFT_BL   = 38;

        // Touch Panel Pins
        static constexpr int TOUCH_SDA = 17;
        static constexpr int TOUCH_SCL = 18;
        static constexpr int TOUCH_INT = 16;

        // Modbus Serial Pins
        static constexpr int MODBUS_TX = 43;
        static constexpr int MODBUS_RX = 44;

        // Initialize board-specific hardware
        static void init() {
            #ifdef HAS_DISPLAY
            // Initialize display pins
            pinMode(TFT_CS, OUTPUT);
            pinMode(TFT_DC, OUTPUT);
            pinMode(TFT_RST, OUTPUT);
            pinMode(TFT_WR, OUTPUT);
            pinMode(TFT_RD, OUTPUT);
            pinMode(TFT_BL, OUTPUT);

            // Set up parallel data pins
            for (int pin : {TFT_D0, TFT_D1, TFT_D2, TFT_D3, TFT_D4, TFT_D5, TFT_D6, TFT_D7}) {
                pinMode(pin, OUTPUT);
            }
            #endif

            #ifdef HAS_TOUCH
            // Initialize touch panel I2C pins
            pinMode(TOUCH_SDA, INPUT_PULLUP);
            pinMode(TOUCH_SCL, INPUT_PULLUP);
            pinMode(TOUCH_INT, INPUT_PULLUP);
            #endif

            // Initialize Modbus serial pins
            pinMode(MODBUS_TX, OUTPUT);
            pinMode(MODBUS_RX, INPUT);
        }
    };
}

#endif // HAL_LILYGO_T_DISPLAY_S3_H