# Prompts for Kilo Code AI / AI Agents

### Prompt 1: Project Setup & HAL
> "Act as a Senior Embedded Engineer. Following the principles in `AGENTS.md`, set up the PlatformIO project structure to support multiple hardware targets (LILYGO T-Display-S3, Waveshare ESP32-S3-1.47, and a generic ESP32). Create the `platformio.ini` with environments for each board, and establish the `lib/hal` directory with corresponding configuration files. Ensure build flags correctly define features like `HAS_DISPLAY` for each environment."

### Prompt 2: Modbus Driver Implementation
> "Act as a Senior Embedded Engineer. Implement the `HuaweiInverter` class as a reusable Modbus driver using the `ModbusMaster` library. Configure it for `HardwareSerial` (Serial1) on pins 43 (TX) and 44 (RX) at 9600 baud, 8N1. Create a non-blocking `update()` method to poll registers 37760 (SOC), 32064 (Input Power), and 37113 (Active Power), ensuring graceful handling of Modbus exceptions."

### Prompt 3: Conditional UI Implementation
> "Act as a Senior Embedded Engineer. Integrate the LVGL library to create a user interface that is conditionally compiled based on the `HAS_DISPLAY` feature flag. The UI should display key data from the inverter (SOC, Input Power, Active Power) in a clear, at-a-glance format. Use a suitable display driver like LovyanGFX or TFT_eSPI, managed through the HAL."

### Prompt 4: WiFi and Web Server
> "Act as a Senior Embedded Engineer. Implement a WiFi manager that starts a captive portal for credential configuration if a connection fails. Add an asynchronous web server that provides a data dashboard and a `/api/data` JSON endpoint. This feature should be fully functional on headless boards."
