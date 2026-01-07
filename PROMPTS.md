# Prompts for Kilo Code AI / AI Agents

## Prompt 1: Project Initialization
> "Act as a Senior Embedded Engineer. Initialize the 'Huawei-Sun2000-Offline-HMI' project for the LilyGO T-Display-S3. 
> 1. Create a `platformio.ini` file with the specific 8-bit parallel build flags defined in `AGENTS.md` (Step 1).
> 2. Create a `main.cpp` that initializes the TFT display, turns on the backlight (GPIO 38) and Power (GPIO 15), and displays a 'System Booting...' message to verify hardware."

## Prompt 2: Modbus Logic
> "Now implement the Modbus communication. Create a class `HuaweiInverter` in `lib/HuaweiInverter`.
> 1. Configure `Serial1` on Pins 43 (TX) and 44 (RX) at 9600 baud.
> 2. Use the `ModbusMaster` library.
> 3. Implement a function `bool update()` that reads Register 37760 (SOC), 32064 (PV Power), and 37113 (Grid Power).
> 4. Store these values in public variables within the class."

## Prompt 3: UI Implementation
> "Create the User Interface in `main.cpp` using LVGL.
> 1. Initialize LVGL with TFT_eSPI as the display driver.
> 2. Create LVGL widgets for the dashboard to ensure flicker-free updates with buffering.
> 3. Design the UI following the 'at-a-glance' philosophy: Center with huge font for Battery SOC (%), Bottom Left for PV Production (Yellow/Green), Bottom Right for House Consumption (Blue/Red).
> 4. In the `loop()`, update the Modbus data every 2 seconds and refresh the LVGL screen."

## Prompt 4: WiFi and Web Server Implementation
> Setup Routine: Implement a setup routine where the device attempts to connect to saved WiFi credentials on startup. If connection fails or no credentials are saved, start an Access Point (AP) mode for web-based configuration, allowing users to input SSID and password via a captive portal or simple web page.
> Web Server with Dashboard: Set up an asynchronous web server (e.g., ESPAsyncWebServer) to serve a simple HTML dashboard displaying solar system data. Include basic styling for readability.
> REST GET Endpoint for JSON Data: Provide a GET endpoint (e.g., /api/data) that returns JSON with current SOC, input power, and active power values.
> Compatibility with Boards without Display: Ensure the firmware compiles and runs on ESP32 boards without display hardware by conditionally including display-related code.

