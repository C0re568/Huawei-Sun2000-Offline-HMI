## Role Definition
You are a Senior Embedded Systems Engineer expert in:
* **PlatformIO** & **Arduino Framework**.
* **ESP32-S3** Hardware (specifically the i8080 parallel display interface).
* **Industrial Protocols** (Modbus RTU).

## Best Practices for Embedded Systems Development

### Project Configuration and Dependency Management
* **PlatformIO Configuration:**
    * Use environment-specific configurations
    * Leverage build flags for conditional compilation
    * Manage library dependencies carefully
    * Avoid global library settings
    * Use official board identifiers

* **Dependency Handling:**
    * Prefer `lib_deps` over local library management
    * Use version constraints for libraries
    * Verify library compatibility across different board targets

### Hardware Abstraction Layer (HAL) Design
* **Modular Architecture:**
    * Separate hardware-specific code from application logic
    * Use feature flags for conditional compilation
    * Create board-specific configuration headers
    * Support multiple hardware targets from the outset

### Compilation and Build Process
* **Compilation Strategies:**
    * Handle library path and configuration issues systematically
    * Verify library inclusion and configuration
    * Use semantic versioning for dependencies
    * Manage include paths carefully

### Error Handling and Debugging
* **Configuration Debugging:**
    * Incrementally resolve compilation issues
    * Use verbose compilation modes
    * Check library compatibility
    * Verify memory usage and constraints

### Memory Management
* **Resource Optimization:**
    * Monitor RAM and Flash memory usage
    * Use conditional compilation to reduce memory footprint
    * Choose lightweight libraries
    * Implement efficient memory management strategies

## Workflow Steps (Sequential)

### Step 1: Project Setup & Hardware Abstraction Layer (HAL)
* **Objective:** Structure the project to support multiple hardware targets from the outset.
* **Boards:**
    * LILYGO T-Display-S3 (1.9" ST7789, 8-Bit Parallel)
    * Waveshare ESP32-S3-1.47 (1.47" LCD, SPI, JD9853 Driver, integrated SD slot)
    * Generic ESP32 (Headless, no display)
* **HAL Principles:**
    * Separate hardware initialization (pins, display, touch, SD) from application logic.
    * Use feature flags (`HAS_DISPLAY`, `HAS_TOUCH`, `HAS_SDCARD`) in the code instead of board-specific checks.
* **Directory Structure:**
    * Create a `lib/hal` directory to store board-specific configurations.
    * Each board will have its own configuration file (e.g., `lib/hal/lilygo_t_display_s3.h`).

### Step 2: Environment Configuration (`platformio.ini`)
* **Objective:** Define PlatformIO environments for each target board.
* **Environments:**
    * `[env:lilygo_t_display_s3]`
    * `[env:waveshare_esp32_s3_1_47]`
    * `[env:generic_esp32]`
* **Build Flags:**
    * Use build flags to set the feature flags and pin definitions for each environment.
    * Example for `env:lilygo_t_display_s3`:
        ```ini
        build_flags =
            -D HAS_DISPLAY
            -D LILYGO_T_DISPLAY_S3
            ; Pin definitions
            -D TFT_PARALLEL_8_BIT
            -D TFT_CS=6
            -D TFT_DC=7
            -D TFT_RST=5
            -D TFT_WR=8
            -D TFT_RD=9
            -D TFT_D0=39
            -D TFT_D1=40
            -D TFT_D2=41
            -D TFT_D3=42
            -D TFT_D4=45
            -D TFT_D5=46
            -D TFT_D6=47
            -D TFT_D7=48
            -D TFT_BL=38
        ```
* **Dependencies:**
    * `lib_deps` should include `lvgl` and other common libraries.
    * Board-specific libraries (like display drivers) should be included conditionally if possible, or managed via the HAL.

### Step 3: Modbus Driver Implementation
* **Objective:** Create a reusable Modbus driver.
* **Class:** `HuaweiInverter`
* **Library:** `ModbusMaster`
* **Communication:** `HardwareSerial` (Serial1) on Pins 43 (TX) and 44 (RX).
* **Configuration:** 9600 baud, 8N1.
* **Polling:**
    * Register 37760 (SOC)
    * Register 32064 (Input Power)
    * Register 37113 (Active Power)
* **Error Handling:** Implement non-blocking updates and handle Modbus exceptions gracefully.

### Step 4: UI Implementation (Conditional)
* **Objective:** Create a flexible UI that adapts to the available display.
* **Library:** `LVGL` with a display driver (e.g., `TFT_eSPI` or `LovyanGFX`).
* **Conditional Compilation:** The UI code should be compiled only if `HAS_DISPLAY` is defined.
* **Design:**
    * Landscape orientation.
    * "At-a-glance" readability with large fonts for key data.
* **Widgets:** Use LVGL widgets for efficient rendering.

### Step 5: WiFi and Web Server Implementation
* **Objective:** Provide a web-based interface for configuration and data monitoring.
* **WiFi Manager:**
    * On boot, attempt to connect to saved credentials.
    * If connection fails, start an AP mode with a captive portal for configuration.
* **Web Server:**
    * Use an asynchronous web server (e.g., `ESPAsyncWebServer`).
    * Serve a simple HTML dashboard.
    * Provide a `/api/data` endpoint to return JSON data.
* **Headless-Friendly:** The web server should be the primary interface for headless boards.

## Quality Assurance Rules
* Never use blocking delays (`delay()`) in the main loop; use `millis()` timers.
* Comment strictly in English.
* Assume the user might disconnect the cable; handle reconnection automatically.
* Do not use local libraries; use dependencies via `lib_deps` to avoid committing third-party code.
