# Build Troubleshooting

Common build issues and solutions for PlatformIO.

## 🔧 Common Build Errors

### Issue: `ModuleNotFoundError: No module named 'yaml'`

**Symptoms:**
```
ModuleNotFoundError: No module named 'yaml':
  File ".../component_manager.py", line 14:
    import yaml
```

**Cause:**
The pioarduino platform requires PyYAML but it's not installed in PlatformIO's Python environment.

**Solution:**
Install PyYAML in PlatformIO's Python environment:

```bash
# Find PlatformIO Python path
pio system info | grep "Python Executable"

# Install PyYAML (replace path with your PlatformIO Python path)
/usr/local/Cellar/platformio/6.1.18_3/libexec/bin/python3 -m pip install pyyaml

# Or if PlatformIO is installed elsewhere:
~/.platformio/penv/bin/python3 -m pip install pyyaml
```

**Alternative:** Use the system Python if PlatformIO uses it:
```bash
python3 -m pip install --user pyyaml
```

---

### Issue: Warning: `++` expression of `volatile`-qualified type

**Symptoms:**
```
src/main.cpp:70:5: warning: '++' expression of 'volatile'-qualified type is deprecated [-Wvolatile]
   70 |     pulseCount++;
```

**Cause:**
C++14+ deprecates incrementing volatile variables directly.

**Solution:**
Use assignment instead of increment:
```cpp
// Instead of: pulseCount++;
pulseCount = pulseCount + 1;
```

Or use atomic operations if needed:
```cpp
#include <atomic>
std::atomic<uint32_t> pulseCount(0);

// In interrupt:
pulseCount++;
```

**Note:** This is just a warning, not an error. The code will still work.

---

### Issue: `esp_idf_size: error: unrecognized arguments: --ng`

**Symptoms:**
```
esp_idf_size: error: unrecognized arguments: --ng
Warning: esp-idf-size exited with code 2
```

**Cause:**
Version mismatch between esp-idf-size tool and the pioarduino platform. This is a non-critical warning.

**Solution:**
This warning doesn't affect the build. It's safe to ignore. If you want to suppress it:

Add to `platformio.ini`:
```ini
board_build.flash_size = 4MB
board_build.arduino.memory_type = qspi_opi
```

---

### Issue: Partition Table Not Found

**Symptoms:**
```
ERROR: Partition table file not found: partitions_zigbee.csv
```

**Solution:**
1. Verify `partitions_zigbee.csv` exists in project root
2. Check `platformio.ini` has: `board_build.partitions = partitions_zigbee.csv`
3. Verify CSV file path is correct

---

### Issue: Build Fails with Platform Not Found

**Symptoms:**
```
Error: Could not find a version that satisfies the requirement 
https://github.com/pioarduino/platform-espressif32/releases/download/stable/platform-espressif32.zip
```

**Solution:**
1. Check internet connection
2. Verify GitHub URL is accessible
3. Try updating PlatformIO: `pio upgrade`
4. Try reinstalling platform: `pio platform install https://github.com/pioarduino/platform-espressif32/releases/download/stable/platform-espressif32.zip`

---

### Issue: Board Not Found

**Symptoms:**
```
Error: Unknown board ID 'esp32-c6-devkitm-1'
```

**Solution:**
1. Verify platform is installed
2. Check board name spelling: `esp32-c6-devkitm-1` (not `esp32c6` or `xiao_esp32c6`)
3. List available boards: `pio boards esp32c6`
4. Try reinstalling platform

---

## 🐛 Build Warnings

### Volatile Increment Warning

**Warning:**
```
warning: '++' expression of 'volatile'-qualified type is deprecated
```

**Status:** Non-critical warning, code works fine

**Fix:** See solution above

---

### Unused Variable Warning

**Warning:**
```
warning: unused variable 'variable_name'
```

**Status:** Non-critical, code works fine

**Fix:** Remove unused variable or mark with `(void)variable_name;`

---

## ✅ Build Verification

### Successful Build Output

A successful build should show:
```
========================= [SUCCESS] Took X.XX seconds =========================
```

Memory usage:
```
RAM:   [          ]   4.5% (used XXXXX bytes from XXXXXXX bytes)
Flash: [==        ]  22.4% (used XXXXXX bytes from XXXXXXX bytes)
```

### Build Artifacts

After successful build, you should have:
- `.pio/build/xiao_esp32c6/firmware.bin` - Firmware binary
- `.pio/build/xiao_esp32c6/firmware.elf` - ELF file
- `.pio/build/xiao_esp32c6/partitions.bin` - Partition table

---

## 🔍 Debugging Build Issues

### Enable Verbose Output

```bash
pio run -v
```

### Clean Build

```bash
pio run -t clean
pio run
```

### Check Platform Status

```bash
pio platform list
pio platform show espressif32
```

### Check Installed Tools

```bash
pio pkg list
```

### Check Environment

```bash
pio system info
```

---

## 📝 Build Requirements Checklist

Before building, verify:

- [ ] PyYAML installed in PlatformIO Python environment
- [ ] PlatformIO updated to latest version: `pio upgrade`
- [ ] Platform installed correctly
- [ ] Partition table file exists
- [ ] Source files present (`src/main.cpp`)
- [ ] Include files present (`include/config.h`)
- [ ] `platformio.ini` configured correctly

---

## 🆘 Getting Help

If build issues persist:

1. **Check PlatformIO Version:**
   ```bash
   pio --version
   ```

2. **Update PlatformIO:**
   ```bash
   pio upgrade
   ```

3. **Reinstall Platform:**
   ```bash
   pio platform uninstall https://github.com/pioarduino/platform-espressif32/releases/download/stable/platform-espressif32.zip
   pio platform install https://github.com/pioarduino/platform-espressif32/releases/download/stable/platform-espressif32.zip
   ```

4. **Clean and Rebuild:**
   ```bash
   pio run -t clean
   rm -rf .pio
   pio run
   ```

5. **Check Logs:**
   - Review verbose output: `pio run -v > build.log 2>&1`
   - Check PlatformIO logs: `~/.platformio/logs/`

---

**Still having issues?** Check the main [Troubleshooting Guide](TROUBLESHOOTING.md) or open an issue on GitHub.

