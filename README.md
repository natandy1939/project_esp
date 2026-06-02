# ESP32-C3 hand-held (Breadboard Edition)

A bare-bones, functional hardware prototype of a handheld simple game engine .
---

## 🚀 Current Status

*   **Display:** Working. It renders a pixel dinosaur, a cactus, and the UI.
*   **Game Logic:** Working. It handles the jump physics, collision detection, score tracking, and the "GAME OVER" state.
*   **Controls:** tow button .

---

## 🛠️ Hardware Setup


*   **Microcontroller:** ESP32-C3 Super Mini. Powered and flashed directly via the onboard USB-C port.
*   **Display:** Standard 4-pin I2C OLED Display (SSD1306). Wired directly to the ESP32-C3's I2C pins:
    *   GND -> GND
    *   VCC -> 3.3V
    *   SCL -> SCL pin
    *   SDA -> SDA pin
*   **Base:** 1x Mini 170-tie-point breadboard.
*   **Input:** 2x Tactile Push Button  
*   **Wiring:** Minimal solid-core jumper wires holding the screen and board together.

---

## 💻 Software Architecture

The software is lightweight, direct, and specifically written to run smoothly on this exact hardware setup.

*   **Language:** C / C++
*   **Environment:** Arduino IDE / PlatformIO
*   **Graphics & Display Drivers:** Utilizing lightweight libraries (like `Adafruit_SSD1306` and `Adafruit_GFX`, or `U8g2`) to push pixels to the I2C OLED over the wire without lagging the game loop.
---
