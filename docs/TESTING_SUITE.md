# Test Suite Documentation

Comprehensive test suite for the water flow meter project using PlatformIO's Unity testing framework.

## 📋 Overview

The test suite includes:
- **Unit Tests** - Test individual functions and modules
- **Integration Tests** - Test component interaction
- **Configuration Tests** - Verify configuration values
- **Hardware Tests** - Test hardware interaction (requires hardware)

## 🧪 Test Structure

```
test/
├── test_main.cpp           # Main test runner
├── test_flow_sensor.h/cpp  # Flow sensor tests
├── test_flow_calculation.h/cpp  # Flow calculation tests
├── test_battery_monitor.h/cpp   # Battery monitor tests (if enabled)
├── test_data_persistence.h/cpp  # Data persistence tests
└── test_integration.h/cpp       # Integration tests
```

## 🚀 Running Tests

### ⚠️ Important Note

**PlatformIO embedded tests require hardware connection for execution.** Even with `--without-uploading`, PlatformIO will wait for a serial connection to run tests on the device. 

**Options:**
1. **Compile Tests Only** - Verify test code compiles without hardware
2. **Run Tests on Hardware** - Execute tests on connected ESP32 device

### Compile Tests (No Hardware Required)

Verify that tests compile correctly without requiring hardware:

```bash
# Compile test code only (no execution)
pio run -e test-compile

# Compile with verbose output
pio run -e test-compile -v
```

This verifies:
- ✅ Test code syntax is correct
- ✅ All includes and dependencies resolve
- ✅ Build flags are correct
- ❌ Does NOT execute tests (requires hardware)

### Run Tests on Hardware

Tests must run on the actual ESP32 hardware:

```bash
# Build and upload tests to device (requires hardware connected)
pio test -e test

# Monitor test output while running
pio test -e test -v

# Run specific test suite
pio test -e test -f test_flow_calculation

# Alternative: Upload then monitor separately
pio run -e test -t upload
pio device monitor
```

### Run Tests in IDE

**VS Code:**
1. Click **Test** in PlatformIO toolbar
2. Or use menu: **PlatformIO** → **Test**

**CLI:**
```bash
pio test
```

## 📝 Test Suites

### 1. Flow Sensor Tests (`test_flow_sensor.cpp`)

Tests for flow sensor functionality:

- ✅ `test_flow_sensor_pin_configuration` - Verify pin configuration
- ✅ `test_pulse_counting_logic` - Test pulse counting increment
- ✅ `test_interrupt_handler_assignment` - Verify interrupt setup
- ✅ `test_calibration_factor_range` - Verify calibration factor is valid

**Run:**
```bash
pio test -f test_flow_sensor
```

### 2. Flow Calculation Tests (`test_flow_calculation.cpp`)

Tests for flow rate and volume calculations:

- ✅ `test_flow_rate_calculation_basic` - Basic flow rate calculation
- ✅ `test_flow_rate_calculation_zero_pulses` - Zero flow rate
- ✅ `test_flow_rate_calculation_high_flow` - Maximum flow rate
- ✅ `test_volume_accumulation` - Volume accumulation logic
- ✅ `test_volume_accuracy` - Volume calculation accuracy
- ✅ `test_flow_stopped_detection` - Flow stopped detection

**Run:**
```bash
pio test -f test_flow_calculation
```

### 3. Battery Monitor Tests (`test_battery_monitor.cpp`)

Tests for battery monitoring (only if `BATTERY_ENABLED`):

- ✅ `test_battery_voltage_reading` - Voltage reading calculation
- ✅ `test_battery_percentage_calculation` - Percentage calculation
- ✅ `test_battery_voltage_range` - Voltage range limits
- ✅ `test_battery_percentage_range` - Percentage clamping
- ✅ `test_battery_warning_levels` - Warning level thresholds
- ✅ `test_voltage_divider_calculation` - Voltage divider math

**Run:**
```bash
pio test -f test_battery_monitor
```

### 4. Data Persistence Tests (`test_data_persistence.cpp`)

Tests for EEPROM/Preferences functionality:

- ✅ `test_eeprom_namespace_configuration` - Namespace configuration
- ✅ `test_save_threshold_configuration` - Save threshold values
- ✅ `test_save_interval_configuration` - Save interval values
- ✅ `test_volume_data_structure` - Data structure validation

**Run:**
```bash
pio test -f test_data_persistence
```

### 5. Integration Tests (`test_integration.cpp`)

Tests for system integration:

- ✅ `test_configuration_consistency` - Configuration value consistency
- ✅ `test_zigbee_configuration` - Zigbee configuration validation
- ✅ `test_report_intervals` - Report interval validation
- ✅ `test_pin_conflicts` - Pin conflict detection
- ✅ `test_memory_usage_estimates` - Memory usage validation

**Run:**
```bash
pio test -f test_integration
```

## ✅ Test Results

### Expected Output

Successful test run:

```
Water Flow Meter - Test Suite
========================================

test/test_flow_sensor.cpp:12:test_flow_sensor_pin_configuration      [PASS]
test/test_flow_sensor.cpp:19:test_pulse_counting_logic               [PASS]
...

---------------------------------------
All Tests Complete
========================================
Tests: 20 | Passing: 20 | Failing: 0 | Ignored: 0
```

## 🔧 Test Configuration

### PlatformIO Test Settings

In `platformio.ini`:

```ini
[env:test]
extends = env:xiao_esp32c6
test_framework = unity
test_build_src = yes
test_filter = *
```

### Test Filtering

Run specific tests:

```bash
# Run only flow sensor tests
pio test -f test_flow_sensor

# Run only battery tests (if enabled)
pio test -f test_battery_monitor

# Run integration tests
pio test -f test_integration
```

## 📊 Test Coverage

### Current Coverage

- ✅ Flow sensor configuration
- ✅ Flow rate calculations
- ✅ Volume accumulation
- ✅ Battery monitoring (if enabled)
- ✅ Data persistence configuration
- ✅ System integration
- ✅ Configuration validation

### Missing Tests (Require Hardware)

- ⚠️ Hardware interrupt testing (requires actual pulses)
- ⚠️ EEPROM read/write (requires hardware)
- ⚠️ Zigbee communication (requires coordinator)
- ⚠️ Real-time flow measurement (requires water flow)

## 🐛 Troubleshooting Tests

### Issue: Tests won't compile

**Solution:**
- Verify Unity framework is installed: `pio pkg list | grep unity`
- Check test files include correct headers
- Verify `test_framework = unity` in `platformio.ini`

### Issue: Tests fail

**Solution:**
- Check test output for specific failure messages
- Verify configuration values match expected
- Check test assertions are correct
- Run with verbose: `pio test -v`

### Issue: Tests wait for hardware connection

**Problem:** PlatformIO embedded tests require hardware even with `--without-uploading` flag.

**Solution:**
- Use `test-compile` environment to verify compilation: `pio run -e test-compile`
- Connect hardware to execute tests: `pio test -e test`
- Some tests can run without hardware (calculation tests), but execution still requires hardware connection
- Use examples for isolated hardware testing

## 📚 Writing New Tests

### Test Template

```cpp
// test_new_feature.h
#ifndef TEST_NEW_FEATURE_H
#define TEST_NEW_FEATURE_H

#include <unity.h>
#include "../include/config.h"

void test_feature_function(void);

void NewFeatureTests(void);

#endif

// test_new_feature.cpp
#include "test_new_feature.h"

void test_feature_function(void) {
    // Arrange
    int expected = 42;
    
    // Act
    int actual = functionUnderTest();
    
    // Assert
    TEST_ASSERT_EQUAL(expected, actual);
}

void NewFeatureTests(void) {
    RUN_TEST(test_feature_function);
}
```

### Test Assertions

Common Unity assertions:

```cpp
// Integer equality
TEST_ASSERT_EQUAL(expected, actual);

// Float equality (with tolerance)
TEST_ASSERT_FLOAT_WITHIN(tolerance, expected, actual);

// Boolean
TEST_ASSERT_TRUE(condition);
TEST_ASSERT_FALSE(condition);

// Pointers
TEST_ASSERT_NOT_NULL(pointer);
TEST_ASSERT_NULL(pointer);

// Strings
TEST_ASSERT_EQUAL_STRING(expected, actual);
```

## 📝 Test Checklist

Before committing code:

- [ ] All tests pass
- [ ] New features have tests
- [ ] Tests cover edge cases
- [ ] Hardware tests documented
- [ ] Test documentation updated

## 🔄 Continuous Integration

Tests can be run in CI/CD:

```yaml
# Example GitHub Actions
- name: Run Tests
  run: |
    pio test
```

## 📚 Additional Resources

- [Unity Test Framework](https://github.com/ThrowTheSwitch/Unity)
- [PlatformIO Testing](https://docs.platformio.org/en/latest/advanced/unit-testing/index.html)
- [ESP32 Testing Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/tools/idf-unit-test.html)

---

**Test suite complete!** Run `pio test` to verify your implementation!

