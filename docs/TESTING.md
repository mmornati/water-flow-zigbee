# Comprehensive Testing Guide

Complete testing procedures for the water flow meter project.

## 📋 Table of Contents

- [Introduction](#introduction)
- [Hardware Testing](#hardware-testing)
- [Software Testing](#software-testing)
- [Integration Testing](#integration-testing)
- [Performance Testing](#performance-testing)
- [Calibration Procedures](#calibration-procedures)
- [Test Reports](#test-reports)

## 🎯 Introduction

This guide provides comprehensive testing procedures to verify:
- Hardware functionality
- Software correctness
- System integration
- Measurement accuracy
- Long-term stability

## 🔧 Hardware Testing

### Test 1: Power Supply Verification

**Objective:** Verify power supply provides stable 5V

**Procedure:**
1. Connect multimeter to XIAO ESP32C6 VIN and GND
2. Measure voltage (should be 5.0V ±0.25V)
3. Measure current draw (should be 50-150mA)
4. Verify voltage remains stable under load

**Expected Results:**
- ✅ Voltage: 4.75V - 5.25V
- ✅ Current: 50-150mA typical
- ✅ Voltage stable under load
- ✅ No voltage drop

**Pass Criteria:** All checks pass

### Test 2: Flow Sensor Wiring

**Objective:** Verify flow sensor wiring is correct

**Procedure:**
1. Measure continuity: RED wire → 5V
2. Measure continuity: BLACK wire → GND
3. Measure continuity: YELLOW wire → D2
4. Test signal voltage during flow (should pulse 0-5V)

**Expected Results:**
- ✅ RED wire connected to 5V
- ✅ BLACK wire connected to GND
- ✅ YELLOW wire connected to D2
- ✅ Signal pulses during flow

**Pass Criteria:** All connections correct

### Test 3: Battery Monitoring (if enabled)

**Objective:** Verify battery monitoring circuit

**Procedure:**
1. Measure battery voltage directly (multimeter)
2. Measure voltage at A0 pin (should be half)
3. Verify voltage divider ratio (~0.5)
4. Test battery percentage calculation

**Expected Results:**
- ✅ Voltage divider ratio: 0.49-0.51
- ✅ Battery voltage reads correctly
- ✅ Percentage calculates properly

**Pass Criteria:** All measurements accurate within 5%

### Test 4: GPIO Testing

**Objective:** Verify all GPIO pins work correctly

**Procedure:**
1. Test flow sensor pin (D2) with interrupt
2. Test battery pin (A0) with ADC
3. Test LED pin (if present)
4. Verify no GPIO conflicts

**Expected Results:**
- ✅ All pins configured correctly
- ✅ Interrupts work
- ✅ ADC reads correctly
- ✅ No conflicts

**Pass Criteria:** All GPIO functional

## 💻 Software Testing

### Test 1: Interrupt Handler

**Objective:** Verify interrupt handler counts pulses correctly

**Procedure:**
1. Enable interrupt on D2
2. Generate known number of pulses
3. Verify pulse count matches

**Expected Results:**
- ✅ Interrupt triggers on every pulse
- ✅ Pulse count matches actual
- ✅ No missed pulses
- ✅ No false pulses

**Pass Criteria:** 100% pulse accuracy

### Test 2: Flow Rate Calculation

**Objective:** Verify flow rate calculation accuracy

**Procedure:**
1. Flow water at known rate
2. Measure flow rate with sensor
3. Compare with actual rate
4. Calculate error

**Expected Results:**
- ✅ Flow rate within 5% of actual
- ✅ Flow rate updates every second
- ✅ Zero flow rate when idle

**Pass Criteria:** Error < 5%

### Test 3: Volume Accumulation

**Objective:** Verify volume accumulates correctly

**Procedure:**
1. Flow known volume (10L)
2. Compare sensor reading with actual
3. Calculate error

**Expected Results:**
- ✅ Volume within 2% of actual
- ✅ Volume accumulates correctly
- ✅ No volume when idle

**Pass Criteria:** Error < 2%

### Test 4: Data Persistence

**Objective:** Verify data survives power cycles

**Procedure:**
1. Note total volume
2. Cut power
3. Restore power
4. Verify volume restored

**Expected Results:**
- ✅ Volume restored correctly
- ✅ No data loss
- ✅ EEPROM works correctly

**Pass Criteria:** 100% data recovery

### Test 5: Zigbee Communication

**Objective:** Verify Zigbee communication

**Procedure:**
1. Join Zigbee network
2. Send test reports
3. Verify coordinator receives data
4. Test connection stability

**Expected Results:**
- ✅ Device joins network
- ✅ Reports sent successfully
- ✅ Coordinator receives data
- ✅ Connection remains stable

**Pass Criteria:** All communication works

## 🔗 Integration Testing

### Test 1: Flow Sensor + Zigbee

**Objective:** Verify flow sensor data sent via Zigbee

**Procedure:**
1. Flow water
2. Monitor Zigbee reports
3. Verify data matches sensor reading

**Expected Results:**
- ✅ Flow rate reported correctly
- ✅ Volume reported correctly
- ✅ Reports sent periodically
- ✅ Data matches sensor

**Pass Criteria:** Data matches within 1%

### Test 2: Battery + Zigbee

**Objective:** Verify battery data sent via Zigbee

**Procedure:**
1. Monitor battery level
2. Send battery reports
3. Verify coordinator receives data

**Expected Results:**
- ✅ Battery voltage reported
- ✅ Battery percentage reported
- ✅ Reports sent periodically

**Pass Criteria:** All battery data sent

### Test 3: Complete System

**Objective:** Verify complete system end-to-end

**Procedure:**
1. Flow known volume (10L)
2. Monitor sensor → Zigbee → Home Assistant
3. Verify data accuracy at each stage

**Expected Results:**
- ✅ Sensor counts correctly
- ✅ Zigbee reports correctly
- ✅ Home Assistant displays correctly
- ✅ End-to-end accuracy maintained

**Pass Criteria:** Error < 2% end-to-end

## ⚡ Performance Testing

### Test 1: Maximum Flow Rate

**Objective:** Verify system handles maximum flow rate

**Procedure:**
1. Test with maximum flow rate (30 L/min)
2. Monitor for missed pulses
3. Verify accuracy maintained

**Expected Results:**
- ✅ No missed pulses
- ✅ Accuracy maintained
- ✅ System stable

**Pass Criteria:** No pulse loss, accuracy < 5%

### Test 2: Long-Term Stability

**Objective:** Verify system stable over extended period

**Procedure:**
1. Run system for 24+ hours
2. Monitor for:
   - Missed pulses
   - Memory leaks
   - Zigbee disconnections
   - Data corruption

**Expected Results:**
- ✅ No missed pulses
- ✅ No memory issues
- ✅ Stable connection
- ✅ Accurate data

**Pass Criteria:** 24 hours stable operation

### Test 3: Power Consumption

**Objective:** Verify power consumption within expected range

**Procedure:**
1. Measure current draw (multimeter)
2. Verify within expected range
3. Check power supply capacity

**Expected Results:**
- ✅ Current: 50-150mA typical
- ✅ Power: 250-750mW
- ✅ Power supply adequate

**Pass Criteria:** Power consumption acceptable

## 📏 Calibration Procedures

### Calibration Test 1: Basic Calibration

**Objective:** Determine calibration factor

**Procedure:**
1. Flow exactly 10.0L through sensor
2. Count total pulses
3. Calculate calibration factor

**Expected Results:**
- ✅ Calibration factor determined
- ✅ Factor within expected range (7-8 pulses/L)
- ✅ Multiple tests consistent

**Pass Criteria:** Factor consistent across tests

### Calibration Test 2: Accuracy Verification

**Objective:** Verify calibration accuracy

**Procedure:**
1. Test with multiple volumes (5L, 10L, 20L)
2. Calculate error for each
3. Verify error < 2%

**Expected Results:**
- ✅ Error < 2% for all volumes
- ✅ Consistent accuracy
- ✅ Calibration verified

**Pass Criteria:** Error < 2% for all tests

### Calibration Test 3: Flow Rate Calibration

**Objective:** Verify flow rate calculation

**Procedure:**
1. Set known flow rate
2. Measure with sensor
3. Compare with actual
4. Verify accuracy

**Expected Results:**
- ✅ Flow rate accurate
- ✅ Error < 5%
- ✅ Consistent across rates

**Pass Criteria:** Error < 5%

## 📊 Test Reports

### Test Report Template

```
=================================
Water Flow Meter Test Report
=================================

Test Date: __________
Tester: __________
Firmware Version: __________

Hardware:
- XIAO ESP32C6: OK / FAIL
- Flow Sensor: OK / FAIL
- Battery Monitor: OK / FAIL / N/A
- Power Supply: OK / FAIL

Software Tests:
- Interrupt Handler: PASS / FAIL
- Flow Rate Calculation: PASS / FAIL
- Volume Accumulation: PASS / FAIL
- Data Persistence: PASS / FAIL
- Zigbee Communication: PASS / FAIL

Integration Tests:
- Flow Sensor + Zigbee: PASS / FAIL
- Battery + Zigbee: PASS / FAIL
- Complete System: PASS / FAIL

Performance Tests:
- Maximum Flow Rate: PASS / FAIL
- Long-Term Stability: PASS / FAIL
- Power Consumption: PASS / FAIL

Calibration:
- Calibration Factor: _____ pulses/L
- Accuracy: _____ % error
- Verification: PASS / FAIL

Overall Result: PASS / FAIL

Notes: __________
```

## ✅ Testing Checklist

Complete all tests:

- [ ] Power supply verification
- [ ] Flow sensor wiring
- [ ] Battery monitoring (if enabled)
- [ ] GPIO testing
- [ ] Interrupt handler
- [ ] Flow rate calculation
- [ ] Volume accumulation
- [ ] Data persistence
- [ ] Zigbee communication
- [ ] Integration tests
- [ ] Performance tests
- [ ] Calibration tests

## 🐛 Troubleshooting During Testing

### Issue: Test Fails

**Steps:**
1. Review test procedure
2. Check hardware connections
3. Verify software configuration
4. Review logs/error messages
5. Consult troubleshooting guide

### Issue: Inconsistent Results

**Steps:**
1. Repeat test multiple times
2. Check for environmental factors
3. Verify sensor installation
4. Check for interference
5. Verify power supply stability

## 📚 Additional Resources

- [Step 8: Testing and Calibration](STEP_08_TESTING_CALIBRATION.md)
- [Troubleshooting Guide](TROUBLESHOOTING.md)
- [Calibration Procedure](../tests/calibration_procedure.md)

---

**Testing complete?** Proceed to deployment!

