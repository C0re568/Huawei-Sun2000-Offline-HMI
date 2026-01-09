# Huawei Sun2000 Offline HMI

## 🌞 Project Overview

The Huawei Sun2000 Offline HMI is an advanced, flexible monitoring solution for Huawei Solar Inverters, designed to provide real-time insights into solar energy production and system status. This project offers a versatile, embedded system approach to inverter monitoring across multiple hardware platforms.

### Key Features

- 📊 Real-time solar inverter data monitoring
- 🔌 Multi-board support with hardware abstraction
- 📡 Modbus RTU communication
- 🌐 Optional WiFi web interface
- 📱 Adaptive UI for different display sizes

### Supported Huawei Inverter Models

- Huawei Sun2000-L1 Series
- Huawei Sun2000-L2 Series
- Huawei Sun2000-L3 Series

## 🖥️ Supported Hardware

### Boards

1. **LILYGO T-Display-S3**
   - 1.9" ST7789 Display
   - 8-Bit Parallel Interface
   - Full feature support

2. **Waveshare ESP32-S3-1.47**
   - 1.47" LCD
   - SPI Interface
   - Integrated SD Card Slot

3. **Generic ESP32**
   - Headless Configuration
   - Web Interface Only
   - Modbus Communication

### Feature Matrix

| Feature       | T-Display-S3 | Waveshare ESP32-S3 | Generic ESP32 |
|--------------|--------------|-------------------|---------------|
| Display      | ✅ Full      | ✅ Full           | ❌ None       |
| Touch        | ❌ No        | ❌ No             | ❌ No         |
| SD Card      | ❌ No        | ✅ Yes            | ❌ No         |
| Web Interface| ✅ Yes       | ✅ Yes            | ✅ Yes        |
| Modbus       | ✅ Yes       | ✅ Yes            | ✅ Yes        |

## 🗂️ Project Structure

```
Huawei-Sun2000-Offline-HMI/
│
├── lib/
│   └── hal/                   # Hardware Abstraction Layer
│       ├── generic_esp32.h
│       ├── lilygo_t_display_s3.h
│       └── waveshare_esp32_s3_1_47.h
│
├── src/                       # Main application source
│   ├── main.cpp
│   ├── modbus_driver.cpp
│   └── ui/
│
├── platformio.ini             # PlatformIO Configuration
└── README.md
```

### Hardware Abstraction Layer (HAL)

The HAL provides a flexible, feature-flag-based approach to hardware configuration:

- Separate hardware initialization from application logic
- Use feature flags like `HAS_DISPLAY`, `HAS_TOUCH`
- Board-specific pin definitions and configurations

## 🚀 Getting Started

### Prerequisites

- [PlatformIO IDE](https://platformio.org/install/ide) or [PlatformIO Core](https://docs.platformio.org/en/latest/core/installation/index.html)
- [Visual Studio Code](https://code.visualstudio.com/) (recommended)
- USB-to-Serial Adapter (for programming)

### Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/Huawei-Sun2000-Offline-HMI.git
   cd Huawei-Sun2000-Offline-HMI
   ```

2. Open the project in PlatformIO

### Board Selection and Compilation

Select your target board in `platformio.ini`:

```ini
[env:lilygo-t-display-s3]
platform = espressif32
board = esp32-s3-devkit
framework = arduino
build_flags =
    -D HAS_DISPLAY
    -D LILYGO_T_DISPLAY_S3
    ; Pin definitions...
```

Compile for your board:
```bash
pio run -e lilygo-t-display-s3
```

## ⚙️ Configuration

### Build Flags

Key build flags control feature inclusion:

- `HAS_DISPLAY`: Enable display rendering
- `HAS_TOUCH`: Enable touch input
- `HAS_SDCARD`: Enable SD card functionality

### Modbus Communication

Configure Modbus settings in `src/modbus_driver.cpp`:

```cpp
#define MODBUS_BAUD 9600
#define MODBUS_RX_PIN 43
#define MODBUS_TX_PIN 44
```

## 🔧 Troubleshooting

### Common Issues

1. **No Modbus Communication**
   - Verify serial connection settings
   - Check inverter address
   - Ensure correct RX/TX pins

2. **Display Not Working**
   - Verify build flags
   - Check pin definitions
   - Confirm display driver initialization

### Debugging Tips

- Use serial monitor: `pio device monitor`
- Enable verbose output: `pio run -v`
- Check build flags and HAL configuration

## 📜 License

[Insert License Information]

## 🤝 Contributing

Contributions are welcome! Please read the contributing guidelines before getting started.
