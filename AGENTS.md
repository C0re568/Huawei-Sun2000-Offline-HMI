# AI Development Guidelines & Workflow

## Project Context
We are building a C++ firmware for the **LilyGO T-Display-S3** to monitor a Huawei Solar System via **RS485/Modbus RTU**. The device works autonomously without Wi-Fi, but optional WiFi support is added for web interface and testing.

## Role Definition
You are a Senior Embedded Systems Engineer expert in:
* **PlatformIO** & **Arduino Framework**.
* **ESP32-S3** Hardware (specifically the i8080 parallel display interface).
* **Industrial Protocols** (Modbus RTU).

## Workflow Steps (Sequential)

### Step 1: Environment Configuration (`platformio.ini`)
* **Board:** `lilygo-t-display-s3`
* **Monitor Speed:** 115200
* **Build Flags:** CRITICAL for the Display Driver (ST7789).
    * `-D TFT_PARALLEL_8_BIT`
    * `-D TFT_CS=6`, `-D TFT_DC=7`, `-D TFT_RST=5`, `-D TFT_WR=8`, `-D TFT_RD=9`
    * `-D TFT_D0=39` through `-D TFT_D7=48`
    * `-D TFT_BL=38` (Backlight)
    * `-D LOAD_GLCD`, `-D LOAD_FONT2`, `-D LOAD_FONT4` (Fonts)
* **Dependencies:** Add `lvgl` to `lib_deps` in `platformio.ini` for UI library support.
* **Board Library:** Add the library from https://github.com/Xinyuan-LilyGO/T-Display-S3.git to `lib_deps` in `platformio.ini` for board-specific support. The AI agent should use the examples from the GitHub repository as reference for implementation guidance. It may be best practice to add the whole GitHub repository into a sub directory (e.g., lib/T-Display-S3) for local access.
* **USB Mode:** Ensure `USB_CDC_ON_BOOT` is enabled so standard `Serial` works over USB while we use `Serial1` for RS485.

### Step 2: Modbus Driver Implementation
* Create a dedicated class `HuaweiInverter`.
* Use the ModbusMaster library for Modbus RTU communication.
* Use `HardwareSerial` (Serial1) on **Pins 43 (TX)** and **44 (RX)**.
* Baudrate: **9600**, Config: **8N1**.
* Implement a non-blocking `update()` method that polls:
    * Register 37760 (SOC)
    * Register 32064 (Input Power)
    * Register 37113 (Active Power - negative values indicate grid consumption, positive indicate feed-in to grid)
* Handle Modbus exceptions gracefully (do not crash on timeout).

### Step 3: UI Implementation
* Library: `LVGL` (with TFT_eSPI as display driver).
* Orientation: Landscape.
* **Design Philosophy:** "At-a-glance" readability.
    * **Center:** Huge Font for Battery %.
    * **Bottom Left:** PV Production (Yellow/Green).
    * **Bottom Right:** House Consumption (Blue/Red).
* Use LVGL widgets and buffering to handle updates efficiently.

### Step 4: WiFi and Web Server Implementation
* **Setup Routine:** Implement a setup routine where the device attempts to connect to saved WiFi credentials on startup. If connection fails or no credentials are saved, start an Access Point (AP) mode for web-based configuration, allowing users to input SSID and password via a captive portal or simple web page.
* **Web Server with Dashboard:** Set up an asynchronous web server (e.g., ESPAsyncWebServer) to serve a simple HTML dashboard displaying solar system data. Include basic styling for readability.
* **REST GET Endpoint for JSON Data:** Provide a GET endpoint (e.g., /api/data) that returns JSON with current SOC, input power, and active power values.
* **Compatibility with Boards without Display:** Ensure the firmware compiles and runs on ESP32 boards without display hardware by conditionally including display-related code.

## Quality Assurance Rules
* Never use blocking delays (`delay()`) in the main loop; use `millis()` timers.
* Comment strictly in English.
* Assume the user might disconnect the cable; handle reconnection automatically.
