# ESP32 TCP Socket Client (LwIP)

This project demonstrates how to implement a **TCP Client** on the ESP32 using the **LwIP (Lightweight IP)** stack and standard **BSD Sockets**.

Instead of using the high-level `esp_http_client`, this example interacts directly with the transport layer. It resolves a DNS hostname, establishes a TCP handshake, sends a raw HTTP GET request, and reads the response in chunks.

## Features

- **DNS Resolution:** resolving `httpbin.org` to an IP address using `getaddrinfo`.
- **Raw TCP Sockets:** using `socket()`, `connect()`, `send()`, and `recv()`.
- **Memory Efficiency:** reads the HTTP response in small chunks to save RAM.
- **Error Handling:** basic retry logic for connection failures.

## Hardware Required

- **ESP32 Development Board** (e.g., ESP32-WROOM-32, ESP32-S3, etc.)
- USB Cable (Data capable)

## Software Requirements

- **ESP-IDF Framework** (v5.x or later recommended)
* **CMake** and **Ninja** build tools.

## Project Structure


```
├── CMakeLists.txt
├── components
│   └── wifi_connect      # Custom component for Wi-Fi logic
├── main
│   ├── CMakeLists.txt
│   └── tcp_client.c      # Main application logic (The provided code)
└── README.md
```

## Why use LwIP / BSD Sockets directly?

While `esp_http_client` is easier for general web requests, using LwIP sockets is essential for:

1. **Custom Protocols:** Creating proprietary protocols over TCP (e.g., for industrial automation).
2. **Performance:** Lower overhead by removing HTTP parsing logic when it's not needed.
3. **Portability:** BSD Socket code is nearly identical on Linux, making it easier to port legacy C code to ESP32.
## How to Build and Flash

### 1. Setup the Environment

Open your terminal and export the ESP-IDF paths (if you haven't already added this to your `.bashrc`):

```
. $HOME/esp/esp-idf/export.sh
```

### 2. Configure the Project

**Do not** edit the source code or the `sdkconfig` file to change your wifi credentials. Use the configuration menu.

You need to set your Wi-Fi SSID and Password:

```
idf.py menuconfig
```

- Navigate to **WiFi Configuration.**
- Enter your wifi ssid and wifi password.
- (Optional) Adjust the `maximum retry` count.
- Press `S`to save and `Q` to quit.

### 3. Build the Firmware

Compile the project. This may take a minute on the first run.

```
idf.py build
```

### 4. Flash and Monitor

Flash the firmware to the ESP32 and open the serial monitor to view the logs. Replace `/dev/ttyUSB0` with your actual port if different.

```
idf.py -p /dev/ttyUSB0 flash monitor
```

## ⚠️ Important Notes

- **HTTP vs HTTPS:** This example uses port `80` (Plain HTTP). The data is sent in clear text. For secure communications, **mbedTLS** or `esp-tls` wrappers must be used on top of the socket.
- **Blocking:** The `socket` calls in this example are blocking. In a complex application, ensure this task is prioritized correctly so it doesn't starve other critical tasks.