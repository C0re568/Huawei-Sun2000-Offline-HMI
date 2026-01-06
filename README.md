# Huawei SUN2000 Offline HMI (Resilience Edition)

> [!WARNING]
> **DRAFT PROJECT:** This repository is currently a concept and development draft. It is **not yet a finished, implemented, or hardware-tested project**. Use the code and wiring diagrams at your own risk. Hardware verification is still in progress.

![License](https://img.shields.io/badge/license-MIT-blue.svg) ![Platform](https://img.shields.io/badge/platform-ESP32--S3-green) ![Framework](https://img.shields.io/badge/framework-Arduino%20%7C%20PlatformIO-orange)

A standalone, network-independent hardware monitor for Huawei SUN2000 Inverters and Luna2000 Battery Storage systems.

**Key Feature:** Direct RS485 (Modbus RTU) communication ensures real-time system visibility (SOC, PV Yield, House Load) even during network outages or in grid-independent "Island Mode," without relying on Wi-Fi, the FusionSolar App, or Cloud servers.

---

## 📋 Table of Contents
- [About the Project](#about-the-project)
- [Hardware Requirements](#hardware-requirements)
- [Wiring & Pinout](#wiring--pinout)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
- [Usage](#usage)
- [Roadmap](#roadmap)
- [Contributing](#contributing)
- [License](#license)
- [Acknowledgments](#acknowledgments)

---

## 🧐 About the Project
Standard monitoring solutions for Huawei inverters rely on cloud connectivity. In a power outage (blackout) or internet failure, users often lose visibility into their battery status just when they need it most. 

This project solves that by reading data directly from the inverter's COM port using the **Modbus RTU** protocol. It visualizes this data on a low-power **LilyGO T-Display-S3**, providing a robust "Resilience Dashboard."

**Monitored Metrics:**
* **Battery SOC:** Luna2000 State of Charge (%)
* **PV Input:** Current solar production (Watts)
* **Grid Status:** Active power to/from grid (Watts)

## 🛠 Hardware Requirements
* **Microcontroller:** [LILYGO® T-Display-S3](https://github.com/Xinyuan-LilyGO/T-Display-S3) (ESP32-S3 with 1.9" ST7789 LCD).
* **Communication:** RS485-to-TTL Adapter (3.3V Logic).
* **Cabling:** Twisted pair cable (CAT5e/6 recommended).
* **Power:** USB-C (5V) or LiPo Battery (3.7V).

## 🔌 Wiring & Pinout
The T-Display-S3 uses an 8-bit parallel interface for the screen. RS485 communication is mapped to the side headers.

### ESP32-S3 to RS485 Adapter
| ESP32-S3 Pin | RS485 Adapter | Function |
| :--- | :--- | :--- |
| **3.3V / 5V** | VCC | Power for Adapter |
| **GND** | GND | Common Ground |
| **GPIO 43** | DI (TX) | Data Transmit |
| **GPIO 44** | RO (RX) | Data Receive |
| *Auto* | DE/RE | (Use Auto-Flow adapter or tie high/low) |

### RS485 Adapter to Huawei COM Connector
| RS485 Adapter | Huawei COM Pin | Signal |
| :--- | :--- | :--- |
| **A (+)** | Pin 1 (or 3) | RS485 A (Data +) |
| **B (-)** | Pin 2 (or 4) | RS485 B (Data -) |
| **GND** | Pin 5 (optional) | Shield/Ground |

> **Note:** Consult your specific Huawei SUN2000 manual. Pins 1/2 are usually dominant, but cascaded systems may use 3/4.

---

## 🚀 Getting Started

### Prerequisites
* **VS Code** with **PlatformIO** extension installed.
* **Git** for cloning the repository.
* USB-C Data Cable.

### Installation
1.  **Clone the repository:**
    ```bash
    git clone [https://github.com/YourUsername/Huawei-Sun2000-Offline-HMI.git](https://github.com/YourUsername/Huawei-Sun2000-Offline-HMI.git)
    ```
2.  **Open in PlatformIO:**
    Open the project folder in VS Code. PlatformIO should automatically detect the `platformio.ini` file.
3.  **Install Dependencies:**
    PlatformIO will automatically download `TFT_eSPI` and `ModbusMaster` libraries upon first build.
4.  **Build & Upload:**
    Connect your T-Display-S3 via USB and click the **Upload** (Arrow) button in the PlatformIO toolbar.

## 🖥 Usage
Once powered on, the device will attempt to connect via Modbus ID 1 (default). 
* **Startup:** Shows a "Connecting..." screen.
* **Normal Operation:** Updates values every 2 seconds.
* **Error State:** If communication fails (timeout > 5s), the values will turn **Orange/Red** to indicate stale data.

## 🗺 Roadmap
- [ ] Implement Touchscreen support for page switching (Detail View).
- [ ] MQTT optional uplink for Home Assistant integration when Wi-Fi is available to not need Hawei Cloud connection all the time.

## 🤝 Contributing
Contributions are what make the open-source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

1.  Fork the Project
2.  Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3.  Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4.  Push to the Branch (`git push origin feature/AmazingFeature`)
5.  Open a Pull Request

## 📄 License
Distributed under the MIT License. See `LICENSE` for more information.

## 👏 Acknowledgments
* **LilyGO** for the excellent hardware documentation.
* **Huawei** for the transparent Modbus Interface Definitions.
* Generated with the assistance of **Google Gemini** & **Kilo Code AI**.
