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

## Prompt 3: The Dashboard UI
> "Create the User Interface in `main.cpp`.
> 1. Create a `TFT_eSprite` for the background to ensure flicker-free updates.
> 2. Draw a dashboard with the Battery SOC (%) in the center using a large font.
> 3. Display PV Power (Watts) and Grid Power (Watts) at the bottom.
> 4. In the `loop()`, update the Modbus data every 2 seconds and redraw the sprite."
