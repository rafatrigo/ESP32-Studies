# ESP32 Wi-Fi Station (STA) Connection

This project demonstrates how to connect an ESP32 microcontroller to a Wi-Fi network in **Station Mode (STA)** using the **ESP-IDF** framework.

It implements an event-based architecture using **FreeRTOS Event Groups** to handle connection states, automatic retries, and success/failure logging.

## Features

* **Station Mode:** Connects to an existing Wi-Fi Access Point.
* **Kconfig Integration:** SSID and Password are configurable via `menuconfig` (no hardcoded credentials in source).
* **Robust Error Handling:** Implements a retry mechanism (up to 5 attempts) before giving up.
* **Event Driven:** Uses `esp_event` loop and FreeRTOS Event Groups for non-blocking operation.
* **NVS Support:** Initializes Non-Volatile Storage needed for Wi-Fi calibration.

## Hardware Required

* Any **ESP32** development board (ESP32-WROOM, ESP32-S3, ESP32-C3, etc.).
* USB Cable for programming and monitoring.
* A local Wi-Fi network (2.4GHz).

## Software Prerequisites

* **ESP-IDF** (v5.0 or later recommended).
* **CMake** and **Ninja** build tools.

## Project Structure

```text
├── CMakeLists.txt          # Project level CMake
├── README.md               # This file
└── main
    ├── CMakeLists.txt      # Component level CMake
    ├── Kconfig.projbuild   # Configuration menu definition
    └── wifi_connection.c   # Main source code
```

## Configuration

**Do not** edit the source code or the `sdkconfig` file to change your wifi credentials. Use the configuration menu:

**Open the configuration menu:**

```bash
idf.py menuconfig
```
Obs.: To be able to use this command you has to configure `ESP-IDF`.

- Navigate to **WiFi Configuration.**
- Enter your wifi ssid and wifi password.
- (Optional) Adjust the `maximum retry` count.
- Press `S`to save and `Q` to quit.

## Build

**Set the target**

```bash
idf.py set-target esp32
```

**Build, flash and open the serial monitor**

```bash
idf.py build flash monitor
```

- **Build:** Compiles the source code.
- **Flash:** Burns the binary to the ESP32.
- **Monitor:** Opens the serial output to view logs.