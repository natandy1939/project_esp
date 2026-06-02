# ESP32 128x64  Engine

A compact ESP32 game engine for a 128x64 SSD1306 OLED display.

## 🚀 What is included

*   `src/main.cpp` – a complete game engine for ESP32 in Arduino/PlatformIO style.
*   `platformio.ini` – PlatformIO project configuration for `esp32dev`.
*   `main.c` – placeholder note pointing to the actual source file.

## 🎮 Engine features

*   128x64 pixel display support with SSD1306.
*   Jump physics, ground collision, and obstacle movement.
*   Simple score tracking and game-over flow.
*   Menu, playing state, and restart behavior.

## 🔧 Default wiring

*   **OLED I2C**
    *   SDA -> GPIO 21
    *   SCL -> GPIO 22
*   **Button 1 (jump)** -> GPIO 4
*   **Button 2 (action/reset)** -> GPIO 15
*   Buttons should be wired from the GPIO pin to GND, with the code using `INPUT_PULLUP`.

## 📦 Build and flash

This project uses PlatformIO. From the repository root:

```bash
pio run
pio run -t upload
```

If you prefer Arduino IDE, copy the contents of `src/main.cpp` into a new sketch and install:

*   `Adafruit SSD1306`
*   `Adafruit GFX Library`

## 🛠 Notes

*   The engine is intentionally small and designed for low-latency refresh on ESP32.
*   You can extend it by adding more enemies, animated sprites, tile maps, and sound output.
---
