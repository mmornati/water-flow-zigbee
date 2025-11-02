# PlatformIO Setup

This guide covers setting up and building the water flow meter project with PlatformIO.

## 📋 Prerequisites

- [ ] PlatformIO installed (VS Code extension or standalone CLI)
- [ ] XIAO ESP32C6 hardware
- [ ] USB-C cable for programming
- [ ] Hardware setup completed (see [Installation Guide](INSTALLATION.md))

## 🔧 PlatformIO Installation

### Option 1: VS Code Extension (Recommended)

1. **Install VS Code:**
   - Download from [code.visualstudio.com](https://code.visualstudio.com/)

2. **Install PlatformIO Extension:**
   - Open VS Code
   - Go to Extensions (Ctrl+Shift+X / Cmd+Shift+X)
   - Search for "PlatformIO IDE"
   - Click Install

3. **Verify Installation:**
   - Open PlatformIO Home (PlatformIO icon in sidebar)
   - Verify PlatformIO is installed

### Option 2: Standalone CLI

1. **Install Python 3.7+:**
   - Download from [python.org](https://www.python.org/)

2. **Install PlatformIO:**
   ```bash
   pip install platformio
   ```

3. **Verify Installation:**
   ```bash
   pio --version
   ```

## 📦 Project Setup

### Open Project in PlatformIO

1. **Open Project:**
   - VS Code: **File** → **Open Folder** → Select `water-flow-zigbee` directory
   - CLI: `cd water-flow-zigbee`

2. **Verify PlatformIO.ini:**
   - PlatformIO will detect `platformio.ini` automatically
   - Project should appear in PlatformIO sidebar

### Project Structure

PlatformIO uses this structure:

```
water-flow-zigbee/
├── platformio.ini              # PlatformIO configuration
├── src/                         # Source code
│   └── main.cpp                # Main application
├── include/                     # Header files
│   └── config.h                # Configuration constants
├── lib/                         # Custom libraries (optional)
├── test/                        # Test files (optional)
└── examples/                    # Example sketches
```

## ⚙️ Configuration

### PlatformIO.ini

The `platformio.ini` file configures the project:

```ini
[env:xiao_esp32c6]
; Platform Arduino 3.x compatible C6 (pioarduino)
platform = https://github.com/pioarduino/platform-espressif32/releases/download/stable/platform-espressif32.zip
board = esp32-c6-devkitm-1    ; Compatible with XIAO ESP32C6
framework = arduino

; Serial Monitor
monitor_speed = 115200

; Partition table (required for Zigbee)
board_build.partitions = partitions_zigbee.csv

; Upload options
upload_speed = 921600

; Build flags
build_flags = 
    -DCORE_DEBUG_LEVEL=3
```

**Note:** This project uses the `pioarduino` platform fork for better ESP32C6 + Arduino 3.x support, which is recommended for Zigbee development on ESP32C6.

### Build Environments

Multiple build environments are available:

- **xiao_esp32c6** - Default environment (release build)
- **dev** - Development environment (with debug flags)
- **release** - Production environment (optimized)

Select environment in PlatformIO:
- VS Code: Click environment name in status bar
- CLI: Use `-e` flag: `pio run -e dev`

## 🔨 Building the Project

### Build with VS Code

1. **Open PlatformIO:**
   - Click PlatformIO icon in VS Code sidebar

2. **Build Project:**
   - Click **Build** in PlatformIO toolbar
   - Or press `Ctrl+Alt+B` / `Cmd+Alt+B`
   - Or use menu: **PlatformIO** → **Build**

3. **Monitor Output:**
   - Build output appears in terminal
   - Check for errors/warnings

### Build with CLI

```bash
# Build default environment
pio run

# Build specific environment
pio run -e dev

# Build with verbose output
pio run -v
```

## 📤 Uploading Firmware

### Upload with VS Code

1. **Connect Device:**
   - Connect XIAO ESP32C6 via USB-C
   - Verify port is detected (check status bar)

2. **Upload Firmware:**
   - Click **Upload** in PlatformIO toolbar
   - Or press `Ctrl+Alt+U` / `Cmd+Alt+U`
   - Or use menu: **PlatformIO** → **Upload**

3. **Monitor Upload:**
   - Upload progress appears in terminal
   - Verify successful upload

### Upload with CLI

```bash
# Upload firmware
pio run -t upload

# Upload and monitor serial
pio run -t upload && pio device monitor
```

## 📊 Serial Monitor

### Open Serial Monitor

**VS Code:**
1. Click **Monitor** in PlatformIO toolbar
2. Or press `Ctrl+Alt+S` / `Cmd+Alt+S`
3. Or use menu: **PlatformIO** → **Serial Monitor**

**CLI:**
```bash
# Open serial monitor
pio device monitor

# Monitor with custom baud rate
pio device monitor -b 115200
```

### Serial Monitor Configuration

Configure in `platformio.ini`:

```ini
monitor_speed = 115200
monitor_filters = 
    colorize
    default
```

## 🧪 Running Tests

### ⚠️ Note: Tests Require Hardware

PlatformIO embedded tests require a connected ESP32 device for execution. Use `test-compile` environment to verify compilation without hardware.

### Compile Tests Only (No Hardware)

```bash
# Verify tests compile correctly (no hardware required)
pio run -e test-compile

# With verbose output
pio run -e test-compile -v
```

### Run Tests on Hardware

```bash
# Run all tests (requires connected device)
pio test -e test

# Run specific test
pio test -e test -f test_specific

# Run with verbose output
pio test -e test -v
```

See [Testing Suite Documentation](TESTING_SUITE.md) for more details.

## 🔧 Advanced Configuration

### Custom Build Flags

Edit `platformio.ini`:

```ini
build_flags = 
    -DDEBUG_ENABLED
    -DCUSTOM_FLAG=value
    -I${env:PIO_SRC_DIR}/include
```

### Library Dependencies

Add libraries in `platformio.ini`:

```ini
lib_deps = 
    library_name@^version
    library_name
    ; From registry
    ; From Git
    ; From local path
```

### Environment-Specific Settings

```ini
[env:dev]
extends = env:xiao_esp32c6
build_flags = 
    ${env:xiao_esp32c6.build_flags}
    -DDEBUG_ENABLED
```

## 🐛 Troubleshooting

### Issue: Build fails with `ModuleNotFoundError: No module named 'yaml'`

**Solution:**
Install PyYAML in PlatformIO's Python environment:
```bash
# Find PlatformIO Python path
pio system info | grep "Python Executable"

# Install PyYAML (use your PlatformIO Python path)
/usr/local/Cellar/platformio/6.1.18_3/libexec/bin/python3 -m pip install pyyaml
```

See [Build Troubleshooting](BUILD_TROUBLESHOOTING.md) for more details.

### Issue: PlatformIO can't find board

**Solution:**
- Verify `platformio.ini` has correct board name (`esp32-c6-devkitm-1`)
- Check platform is installed: `pio platform list`
- Verify platform URL is accessible (pioarduino fork)
- Try reinstalling platform: `pio platform uninstall https://github.com/pioarduino/platform-espressif32/releases/download/stable/platform-espressif32.zip` then rebuild

### Issue: Upload fails

**Solution:**
- Hold BOOT button during upload
- Check USB cable (must be data cable)
- Verify correct port selected
- Lower upload speed: `upload_speed = 115200`

### Issue: Build errors

**Solution:**
- Verify all dependencies installed
- Check `include/config.h` exists
- Clean build: `pio run -t clean`
- Rebuild: `pio run`

### Issue: Serial monitor not working

**Solution:**
- Check baud rate matches code (115200)
- Verify correct port selected
- Close other serial monitor instances
- Check device drivers installed

## 📚 PlatformIO Commands

### Common Commands

```bash
# Build project
pio run

# Upload firmware
pio run -t upload

# Clean build
pio run -t clean

# Monitor serial
pio device monitor

# List devices
pio device list

# Update platforms
pio platform update

# Install libraries
pio lib install library_name
```

### Useful Commands

```bash
# Show project info
pio project config

# Show library dependencies
pio lib list

# Check library updates
pio lib update

# Show build information
pio run -v
```

## ✅ Verification Checklist

Before proceeding, verify:

- [ ] PlatformIO installed
- [ ] Project opens correctly
- [ ] Board detected (XIAO ESP32C6)
- [ ] Project builds without errors
- [ ] Firmware uploads successfully
- [ ] Serial monitor works
- [ ] Device initializes correctly

## 📖 Additional Resources

- [PlatformIO Documentation](https://docs.platformio.org/)
- [ESP32 Platform Documentation](https://docs.platformio.org/en/latest/platforms/espressif32.html)
- [PlatformIO VS Code Extension](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide)

## 🆚 PlatformIO vs Arduino IDE

### Advantages of PlatformIO

- ✅ Better library management
- ✅ Multiple build environments
- ✅ Built-in unit testing
- ✅ Better IDE integration (VS Code)
- ✅ CI/CD integration support
- ✅ Dependency management
- ✅ Better debugging tools

### When to Use Arduino IDE

- Simple projects
- Learning Arduino
- Quick prototyping
- Prefer Arduino IDE workflow

---

**PlatformIO setup complete?** Proceed to [Installation Guide](INSTALLATION.md) for hardware setup!

