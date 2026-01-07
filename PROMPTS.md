# Prompts for Kilo Code AI / AI Agents



## Prompt 1: Project Initialization
> "Act as a Senior Embedded Engineer. Keep @AGENTS.md in mind! Initialize the 'Huawei-Sun2000-Offline-HMI' project for the LilyGO T-Display-S3. 
> Git clone the official repository for the LilyGO T-Display-S3 into a subdirectory named 'offficial_repository' and use the lib directory from there.
> create a .gitignore
> 1. Create a `platformio.ini` file with the specific 8-bit parallel build flags defined in `AGENTS.md` (Step 1).
> 2. Create a `main.cpp` that initializes the TFT display, turns on the backlight (GPIO 38) and Power (GPIO 15), and displays a 'System Booting...' message to verify hardware."

## Prompt 2: Modbus Driver Implementation
> "Act as a Senior Embedded Engineer. Keep @AGENTS.md in mind! 
> "Create the HuaweiInverter class using ModbusMaster library on Serial1 (pins 43 TX, 44 RX), baud 9600, 8N1. Implement non-blocking update() method to poll registers 37760 (SOC), 32064 (Input Power), 37113 (Active Power). Handle exceptions gracefully."

## Prompt 3: UI Implementation
> "Act as a Senior Embedded Engineer. Keep @AGENTS.md in mind! 
> "Integrate LVGL with TFT_eSPI driver for landscape orientation. Design "at-a-glance" UI with huge battery % in center, PV production bottom left (yellow/green), house consumption bottom right (blue/red). Use efficient widgets and buffering."

## Prompt 4: WiFi and Web Server Implementation
> "Act as a Senior Embedded Engineer. Keep @AGENTS.md in mind! 
> Setup Routine: Implement a setup routine where the device attempts to connect to saved WiFi credentials on startup. If connection fails or no credentials are saved, start an Access Point (AP) mode for web-based configuration, allowing users to input SSID and password via a captive portal or simple web page.
> Web Server with Dashboard: Set up an asynchronous web server (e.g., ESPAsyncWebServer) to serve a simple HTML dashboard displaying solar system data. Include basic styling for readability.
> REST GET Endpoint for JSON Data: Provide a GET endpoint (e.g., /api/data) that returns JSON with current SOC, input power, and active power values.
> Compatibility with Boards without Display: Ensure the firmware compiles and runs on ESP32 boards without display hardware by conditionally including display-related code.


