#include <TFT_eSPI.h>

// Create TFT instance
TFT_eSPI tft = TFT_eSPI();

void setup() {
    // Initialize TFT display
    tft.init();
    // Set landscape orientation (rotation 1 for landscape, pins at bottom)
    tft.setRotation(1);
    // Fill screen with green to verify ST7789 driver
    tft.fillScreen(TFT_GREEN);
}

void loop() {
    // Minimal loop, no blocking delays
}