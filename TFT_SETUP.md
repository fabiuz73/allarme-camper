# TFT Display Setup Guide

## Library Required
The code uses the TFT_eSPI library. Install it through Arduino IDE Library Manager or PlatformIO.

## Display Configuration
The TFT_eSPI library needs to be configured for your specific display. This is typically done in one of these ways:

### Method 1: User_Setup.h file
Edit the `User_Setup.h` file in the TFT_eSPI library folder to match your display and ESP32 pinout.

### Method 2: platformio.ini (if using PlatformIO)
Add build flags to specify your display type and pins.

## Common ESP32 + TFT Configurations

### For ILI9341 240x320 Display:
```cpp
#define ILI9341_DRIVER
#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   15  // Chip select control pin
#define TFT_DC    2  // Data Command control pin
#define TFT_RST   4  // Reset pin (could connect to RST pin)
```

### For ST7735 128x160 Display:
```cpp
#define ST7735_DRIVER
#define TFT_CS   15
#define TFT_DC   2
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_RST  4
```

## Pinout Used in This Project
- RGB LED Red: GPIO 12
- RGB LED Green: GPIO 13  
- RGB LED Blue: GPIO 14
- Relay: GPIO 27 (moved from 14 to avoid conflict)
- ADXL335: GPIO 34, 35, 32 (analog inputs)
- TFT Display: Uses standard SPI pins (configure in TFT_eSPI)

## Display Messages
The alarm shows these messages:
1. `ALLARME CAMPER NON CONNESSO` - when Bluetooth is not connected
2. `ALLARME CONNESSO` - when Bluetooth is connected but alarm is off
3. `ALLARME INSERITO` - when alarm is activated

## RGB LED States
1. OFF - when Bluetooth is disconnected
2. GREEN - when Bluetooth is connected
3. RED blinking (1 second intervals) - when alarm is active