# Installation Guide

Complete installation guide for the water flow meter project.

## 📋 Prerequisites

- XIAO ESP32C6 microcontroller
- YF-S201 Hall Effect Water Flow Sensor
- 5V USB-C power supply or external 5V adapter
- USB-C cable for programming
- Computer with Arduino IDE
- Optional: 3.7V Li-ion battery for UPS functionality

## 🔧 Hardware Setup

### Wiring Diagram

```
YF-S201 Flow Sensor    XIAO ESP32C6
─────────────────     ─────────────
    RED (VCC)   ──────>  5V (VIN)
    BLACK (GND) ──────>  GND
    YELLOW (OUT)──────>  D2 (GPIO2)
```

**Important:**
- Flow sensor must be installed in water line
- Sensor arrow indicates flow direction
- Ensure water-tight seals

### Power Connection

Connect power supply:
- **Option 1:** USB-C cable to XIAO ESP32C6 USB port
- **Option 2:** External 5V adapter to VIN and GND pins

### Optional: Battery Backup

If using battery backup:
1. Connect battery positive to BAT+ pad (bottom of board)
2. Connect battery negative to BAT- pad (bottom of board)
3. Install voltage divider circuit for battery monitoring (see main documentation)

## 💻 Software Setup

### Choose Your IDE

You can use either **PlatformIO** (recommended) or **Arduino IDE**:

- **PlatformIO** - Better for development, library management, and CI/CD
- **Arduino IDE** - Simpler, good for beginners

### Option 1: PlatformIO Setup (Recommended)

See [PlatformIO Setup Guide](PLATFORMIO.md) for complete instructions.

**Quick Start:**
1. Install PlatformIO (VS Code extension or CLI)
2. Open project folder in PlatformIO
3. Build and upload: `pio run -t upload`

### Option 2: Arduino IDE Setup

#### Step 1: Install Arduino IDE

1. Download Arduino IDE from [arduino.cc](https://www.arduino.cc/en/software)
2. Install Arduino IDE
3. Launch Arduino IDE

#### Step 2: Install ESP32 Board Support

1. Open **File** → **Preferences**
2. Add this URL to **Additional Board Manager URLs**:
   ```
   https://espressif.github.io/arduino-esp32/package_esp32_index.json
   ```
3. Open **Tools** → **Board** → **Boards Manager**
4. Search for **"esp32"**
5. Install **"esp32 by Espressif Systems"** (version 3.0.0 or higher)

#### Step 3: Configure Board

1. Select board: **Tools** → **Board** → **esp32** → **XIAO_ESP32C6**
2. Select port: **Tools** → **Port** → Choose your XIAO ESP32C6 port
3. Configure upload speed: **Tools** → **Upload Speed** → **921600**

#### Step 4: Install Required Libraries

The following libraries are included with ESP32 board support:
- `Preferences` (for EEPROM)
- `Zigbee` (included with ESP32 package v3.0.0+)

No additional library installation needed!

**Note:** For Arduino IDE, you'll need to rename `src/main.cpp` to `src/main.ino` or create a `.ino` file that includes the necessary headers.

## 📦 Firmware Installation

### Step 1: Open Project

1. Download or clone this repository
2. **PlatformIO:** Open project folder in PlatformIO
   **Arduino IDE:** You'll need to convert `src/main.cpp` to Arduino format

### Step 2: Configure Settings

Edit `include/config.h` (PlatformIO) or `src/config.h` (Arduino IDE) to configure your device:

```cpp
// Flow Sensor Calibration
#define CALIBRATION_FACTOR 7.5  // Adjust after calibration

// Battery (if enabled)
#define BATTERY_ENABLED false   // Set to true if battery installed

// Zigbee Network
#define ZIGBEE_CHANNEL 11        // Match your coordinator channel
#define ZIGBEE_PAN_ID 0x1A62    // Match your network PAN ID
```

### Step 3: Configure Zigbee

**Important:** The Zigbee implementation in `src/main.cpp` is a template. You need to:

1. Identify your ESP32 Zigbee SDK version
2. Update Zigbee functions in `src/main.cpp` with actual API calls
3. Configure based on your SDK:
   - ESP-IDF Zigbee SDK
   - Arduino ESP32 Zigbee library
   - Espressif ESP-ZB library

### Step 4: Upload Firmware

**PlatformIO:**
1. Connect XIAO ESP32C6 via USB-C
2. Click **Upload** in PlatformIO toolbar
3. Or run: `pio run -t upload`

**Arduino IDE:**
1. Connect XIAO ESP32C6 via USB-C
2. Select correct port in Arduino IDE
3. Click **Upload** button (→ icon)
4. Wait for upload to complete

### Step 5: Verify Upload

Open Serial Monitor (115200 baud) to verify:
- **PlatformIO:** Click **Monitor** or `pio device monitor`
- **Arduino IDE:** **Tools** → **Serial Monitor**

## 🧪 Testing

### Test Flow Sensor

1. Open `examples/flow_sensor_test/flow_sensor_test.ino`
2. Upload to XIAO ESP32C6
3. Start water flow
4. Verify pulses are counted correctly

### Calibrate Sensor

1. Open `examples/calibration_test/calibration_test.ino`
2. Upload to XIAO ESP32C6
3. Flow exactly 10.0L through sensor
4. Note calibration factor from results
5. Update `CALIBRATION_FACTOR` in `config.h`

### Test Battery (if enabled)

1. Open `examples/battery_monitor_test/battery_monitor_test.ino`
2. Upload to XIAO ESP32C6
3. Verify battery voltage reads correctly
4. Compare with multimeter reading

## 🔗 Zigbee Setup

### Pairing with Coordinator

1. **Put coordinator in pairing mode:**
   - Zigbee2MQTT: Click "Permit join" button
   - ZHA: Add device in Home Assistant

2. **Power on device:**
   - XIAO ESP32C6 will automatically attempt to join network
   - Check Serial Monitor for join status

3. **Verify pairing:**
   - Device should appear in coordinator
   - Device status should show "Online"
   - Short address should be assigned

### Configure Home Assistant

See [Home Assistant Integration](HOME_ASSISTANT.md) for detailed setup.

## ⚙️ Configuration

### Flow Sensor Calibration

After initial setup, calibrate sensor for accuracy:

1. Use `calibration_test.ino` example
2. Flow known volume (10L recommended)
3. Calculate calibration factor
4. Update `CALIBRATION_FACTOR` in `config.h`
5. Re-upload firmware

### Zigbee Network Settings

Ensure Zigbee settings match your coordinator:

- **Channel:** Must match coordinator channel (11-26)
- **PAN ID:** Must match network PAN ID
- **Network Security:** Must match coordinator settings

## 🔍 Verification

### Serial Monitor Output

After upload, you should see:

```
========================================
Water Flow Meter Starting
========================================
[EEPROM] Loaded total volume: 0.000 L
[Flow Sensor] Initialized on pin 2
[Zigbee] Initializing Zigbee stack...
[Zigbee] Joining network...
[Zigbee] Successfully joined network!
[System] Setup complete - System ready!
```

### System Status

Periodically check system status:

- Flow rate should update every second
- Total volume should accumulate
- Zigbee should remain connected
- Battery level (if enabled) should update

## 🐛 Troubleshooting

### Device won't upload

**Solutions:**
- Hold BOOT button while clicking Upload
- Try different USB cable (must be data cable)
- Check USB port on computer
- Lower upload speed to 115200

### Flow sensor not working

**Solutions:**
- Check wiring (RED→5V, BLACK→GND, YELLOW→D2)
- Verify sensor has power (5V)
- Test sensor with multimeter (should pulse 0-5V)
- Verify water is flowing through sensor

### Zigbee won't join network

**Solutions:**
- Ensure coordinator is in pairing mode
- Check Zigbee channel matches coordinator
- Verify PAN ID matches network
- Check signal strength (reduce distance)

### Battery not reading correctly

**Solutions:**
- Verify voltage divider installed correctly
- Check resistor values (200kΩ each)
- Measure battery directly with multimeter
- Verify voltage divider ratio (~0.5)

For more troubleshooting help, see [TROUBLESHOOTING.md](TROUBLESHOOTING.md).

## ✅ Next Steps

After installation:

1. **Calibrate sensor** - Use calibration test example
2. **Setup Home Assistant** - See [HOME_ASSISTANT.md](HOME_ASSISTANT.md)
3. **Monitor operation** - Check Serial Monitor for status
4. **Verify accuracy** - Test with known volumes

## 📚 Additional Resources

- [Main Documentation](../README.md)
- [Troubleshooting Guide](TROUBLESHOOTING.md)
- [Home Assistant Setup](HOME_ASSISTANT.md)
- [Testing Guide](TESTING.md)

---

**Installation complete?** Proceed to Home Assistant setup!

