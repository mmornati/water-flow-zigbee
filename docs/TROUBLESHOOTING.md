# Troubleshooting Guide

Comprehensive troubleshooting guide for common issues with the water flow meter project.

## 📋 Table of Contents

- [Hardware Issues](#hardware-issues)
- [Software Issues](#software-issues)
- [Flow Sensor Issues](#flow-sensor-issues)
- [Zigbee Issues](#zigbee-issues)
- [Home Assistant Issues](#home-assistant-issues)
- [Power Issues](#power-issues)
- [Data Issues](#data-issues)

## 🔧 Hardware Issues

### Issue: XIAO ESP32C6 Won't Power On

**Symptoms:**
- No LED illumination
- No serial output
- Device appears dead

**Possible Causes:**
1. Power supply not connected
2. Power supply voltage incorrect
3. USB cable issue (charge-only cable)
4. Hardware damage

**Solutions:**

1. **Check Power Supply:**
   - Verify power supply provides 5V ±0.25V
   - Measure voltage with multimeter
   - Ensure current rating > 500mA

2. **Check USB Cable:**
   - Use data cable (not charge-only)
   - Try different USB cable
   - Check USB port on computer

3. **Check Connections:**
   - Verify VIN/GND connections
   - Check for loose connections
   - Inspect for damage

4. **Reset Device:**
   - Press RESET button
   - Hold BOOT button while pressing RESET
   - Release BOOT button

**Still Not Working?**
- Try different power supply
- Check hardware for physical damage
- Verify XIAO ESP32C6 is not defective

---

### Issue: Flow Sensor Not Detected

**Symptoms:**
- No pulses detected
- Flow rate always zero
- Pulse count not increasing

**Possible Causes:**
1. Sensor wiring incorrect
2. Sensor not receiving power
3. No water flowing through sensor
4. Sensor installed backwards
5. Sensor faulty

**Solutions:**

1. **Check Wiring:**
   - RED wire → 5V (VIN)
   - BLACK wire → GND
   - YELLOW wire → D2 (GPIO2)
   - Verify connections with multimeter

2. **Check Power:**
   - Measure voltage at sensor (should be 5V)
   - Check sensor power LED (if present)
   - Verify power supply adequate

3. **Check Water Flow:**
   - Verify water actually flowing
   - Check sensor installation
   - Verify sensor arrow direction

4. **Check Sensor:**
   - Test sensor with multimeter (should pulse 0-5V)
   - Verify sensor is not faulty
   - Check sensor installation in pipe

5. **Test Sensor Directly:**
   ```cpp
   // Simple test: read pin directly
   void loop() {
       int value = digitalRead(2);
       Serial.println(value);
       delay(10);
   }
   // Should pulse HIGH/LOW during flow
   ```

**Still Not Working?**
- Try different sensor
- Verify sensor model (should be YF-S201)
- Check sensor specifications

---

### Issue: Battery Voltage Reads 0 or Incorrect

**Symptoms:**
- Battery voltage reads 0V
- Battery percentage incorrect
- Voltage readings don't match multimeter

**Possible Causes:**
1. Voltage divider not installed
2. Resistor values incorrect
3. Connections incorrect
4. Battery not connected
5. ADC pin issue

**Solutions:**

1. **Check Voltage Divider:**
   - Verify resistors are 200kΩ each
   - Measure resistance with multimeter
   - Check voltage divider ratio (~0.5)

2. **Check Connections:**
   - Battery+ → Resistor1 → A0 → Resistor2 → GND
   - Verify connections with multimeter
   - Check A0 pin connection

3. **Check Battery:**
   - Verify battery connected
   - Measure battery voltage directly
   - Check battery polarity

4. **Calibrate Voltage Divider:**
   ```cpp
   // Test voltage divider ratio
   float testRatio() {
       float battery = readBatteryVoltage();  // Direct measurement
       float adc = analogReadMilliVolts(A0) / 1000.0;
       return adc / battery;  // Should be ~0.5
   }
   ```

**Still Not Working?**
- Verify battery is working
- Check voltage divider circuit
- Test ADC pin with known voltage

---

## 💻 Software Issues

### Issue: Interrupt Not Working

**Symptoms:**
- Pulse count not increasing
- Interrupt handler never called
- Flow rate always zero

**Possible Causes:**
1. Interrupt not attached
2. Wrong pin configured
3. Interrupt disabled accidentally
4. GPIO conflict
5. Code bug

**Solutions:**

1. **Verify Interrupt Setup:**
   ```cpp
   // Check interrupt is attached in setup()
   void setup() {
       pinMode(FLOW_SENSOR_PIN, INPUT_PULLUP);
       attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), 
                       pulseCounter, RISING);
       Serial.println("Interrupt attached on pin " + String(FLOW_SENSOR_PIN));
   }
   ```

2. **Check Pin Configuration:**
   - Verify FLOW_SENSOR_PIN is correct (should be 2)
   - Check pin is INPUT_PULLUP
   - Verify interrupt is RISING edge

3. **Test Interrupt:**
   ```cpp
   // Simple interrupt test
   volatile int testCount = 0;
   void IRAM_ATTR testInterrupt() {
       testCount++;
   }
   
   void setup() {
       attachInterrupt(digitalPinToInterrupt(2), testInterrupt, RISING);
   }
   
   void loop() {
       Serial.println("Interrupt count: " + String(testCount));
       delay(1000);
   }
   ```

4. **Check for Interrupt Disabling:**
   - Verify no `detachInterrupt()` calls
   - Check for sleep modes (should be none)
   - Verify interrupt remains attached

**Still Not Working?**
- Check GPIO conflicts
- Verify pin number correct
- Test with different pin

---

### Issue: Flow Rate Calculation Incorrect

**Symptoms:**
- Flow rate doesn't match actual
- Flow rate always zero or very high
- Inconsistent readings

**Possible Causes:**
1. Calibration factor incorrect
2. Pulse counting wrong
3. Calculation error
4. Timing issue

**Solutions:**

1. **Recalibrate Sensor:**
   - Flow exactly 10L through sensor
   - Count total pulses
   - Calculate calibration factor
   - Update CALIBRATION_FACTOR in code

2. **Verify Pulse Counting:**
   ```cpp
   // Test pulse counting
   void testPulseCounting() {
       uint32_t startPulses = pulseCount;
       delay(5000);  // Wait 5 seconds
       uint32_t pulsesIn5Sec = pulseCount - startPulses;
       Serial.println("Pulses in 5 seconds: " + String(pulsesIn5Sec));
       // Should be ~37-38 pulses at 10 L/min
   }
   ```

3. **Check Calculation:**
   ```cpp
   // Verify calculation formula
   // Flow rate = (pulses/second) / calibration_factor * 60
   float verifyFlowRate(uint32_t pulses, float calFactor) {
       float frequency = pulses;  // pulses per second
       return (frequency / calFactor) * 60.0;  // L/min
   }
   ```

4. **Test with Known Flow Rate:**
   - Set known flow rate (flow regulator)
   - Measure actual rate (timed container)
   - Compare with sensor reading

**Still Not Working?**
- Recalibrate sensor
- Check sensor installation
- Verify calibration factor

---

### Issue: Data Not Persisting

**Symptoms:**
- Total volume resets to 0
- Data lost after power cycle
- EEPROM not saving

**Possible Causes:**
1. EEPROM not initialized
2. Save function not called
3. EEPROM write failed
4. Data not loaded on startup

**Solutions:**

1. **Check EEPROM Initialization:**
   ```cpp
   // Verify Preferences library initialized
   void loadTotalVolume() {
       prefs.begin("flowmeter", true);  // Read-only
       totalVolume = prefs.getFloat("totalVolume", 0.0);
       Serial.println("Loaded volume: " + String(totalVolume));
       prefs.end();
   }
   ```

2. **Verify Save Function Called:**
   - Check `saveTotalVolume()` is called
   - Verify save triggers (volume change or time)
   - Test save manually

3. **Test EEPROM Write:**
   ```cpp
   // Test EEPROM write
   void testEEPROM() {
       prefs.begin("test", false);
       prefs.putFloat("test_value", 123.45);
       float readValue = prefs.getFloat("test_value", 0.0);
       Serial.println("Test value: " + String(readValue));
       // Should be 123.45
       prefs.end();
   }
   ```

4. **Check Save Frequency:**
   - Verify save called periodically
   - Check save threshold (should save every 1L)
   - Verify save interval (should save every 5 min)

**Still Not Working?**
- Check EEPROM size/partition
- Verify Preferences library works
- Test with minimal example

---

## 📡 Flow Sensor Issues

### Issue: Missed Pulses

**Symptoms:**
- Total volume less than actual
- Flow rate lower than expected
- Pulse count doesn't match actual

**Possible Causes:**
1. Interrupt disabled (sleep mode)
2. Interrupt too slow
3. Pulse frequency too high
4. GPIO conflict
5. Hardware issue

**Solutions:**

1. **Verify Always-On Operation:**
   - Ensure no sleep modes
   - Verify interrupt always active
   - Check interrupt never disabled

2. **Check Interrupt Handler Speed:**
   ```cpp
   // Interrupt handler should be fast
   void IRAM_ATTR pulseCounter() {
       pulseCount++;  // Minimal code
       // No delays, no Serial, no complex operations
   }
   ```

3. **Test Pulse Frequency:**
   - Calculate max pulse frequency
   - Verify interrupt can handle rate
   - Test at maximum flow rate (30 L/min)

4. **Check for Interference:**
   - Electrical interference
   - Long signal wires
   - Power supply noise

**Still Not Working?**
- Verify sensor installation
- Check for timing issues
- Test interrupt handler speed

---

### Issue: False Pulses

**Symptoms:**
- Pulses counted when no flow
- Volume increasing when idle
- Flow rate non-zero when idle

**Possible Causes:**
1. Electrical interference
2. Loose connections
3. Sensor faulty
4. Pull-up/pull-down missing

**Solutions:**

1. **Add Pull-Up Resistor:**
   ```cpp
   // Use INPUT_PULLUP (internal pull-up)
   pinMode(FLOW_SENSOR_PIN, INPUT_PULLUP);
   ```

2. **Check Connections:**
   - Verify all connections secure
   - Check for loose wires
   - Shield signal wire if needed

3. **Test Sensor:**
   - Disconnect sensor
   - Verify no pulses when disconnected
   - Test with different sensor

4. **Filter Pulses:**
   ```cpp
   // Add debounce if needed
   volatile unsigned long lastPulseTime = 0;
   void IRAM_ATTR pulseCounter() {
       unsigned long now = micros();
       if (now - lastPulseTime > 1000) {  // Minimum 1ms between pulses
           pulseCount++;
           lastPulseTime = now;
       }
   }
   ```

**Still Not Working?**
- Check for electrical interference
- Verify sensor is not faulty
- Try different sensor

---

## 📶 Zigbee Issues

### Issue: Device Won't Join Network

**Symptoms:**
- Device not joining Zigbee network
- Join timeout
- Connection failed

**Possible Causes:**
1. Coordinator not in pairing mode
2. Zigbee channel mismatch
3. PAN ID mismatch
4. Signal strength too weak
5. Coordinator not reachable

**Solutions:**

1. **Check Coordinator:**
   - Ensure coordinator in pairing mode
   - Verify coordinator is discoverable
   - Check coordinator logs

2. **Verify Network Configuration:**
   - Check Zigbee channel matches
   - Verify PAN ID matches
   - Check network security settings

3. **Check Signal Strength:**
   - Reduce distance to coordinator
   - Check for interference
   - Verify antenna connection (if external)

4. **Reset Device:**
   - Hold BOOT button, press RESET
   - Release RESET, then release BOOT
   - Try joining again

**Still Not Working?**
- Check coordinator compatibility
- Verify Zigbee SDK version
- Test with different coordinator

---

### Issue: Reports Not Received

**Symptoms:**
- Device connected but no data
- Home Assistant not receiving data
- Reports sent but not received

**Possible Causes:**
1. Endpoint ID incorrect
2. Cluster ID incorrect
3. Attribute ID incorrect
4. MQTT configuration wrong (if Zigbee2MQTT)
5. Coordinator not forwarding

**Solutions:**

1. **Verify Endpoint IDs:**
   - Check endpoint IDs match coordinator
   - Verify cluster IDs correct
   - Check attribute IDs match

2. **Test Reporting:**
   ```cpp
   // Test report manually
   void testReport() {
       float testFlowRate = 5.0;  // Test value
       sendFlowReport(testFlowRate, 100.0, 80);
       Serial.println("Test report sent");
   }
   ```

3. **Check Coordinator Logs:**
   - Review Zigbee coordinator logs
   - Check for error messages
   - Verify reports received

4. **Test MQTT (if Zigbee2MQTT):**
   - Check MQTT topics
   - Verify MQTT broker connection
   - Test MQTT publish/subscribe

**Still Not Working?**
- Verify Zigbee configuration
- Check coordinator settings
- Test with Zigbee sniffer

---

### Issue: Frequent Disconnections

**Symptoms:**
- Device disconnects frequently
- Unreliable communication
- Connection unstable

**Possible Causes:**
1. Signal strength too weak
2. Interference
3. Power supply unstable
4. Distance too far
5. Antenna issue

**Solutions:**

1. **Check Signal Strength:**
   - Reduce distance to coordinator
   - Check RSSI value
   - Verify signal strength adequate

2. **Check Power Supply:**
   - Verify power supply stable
   - Check voltage fluctuations
   - Ensure adequate current

3. **Check Interference:**
   - Avoid WiFi interference (different channel)
   - Check for 2.4GHz interference
   - Shield device if needed

4. **Check Antenna:**
   - Verify antenna connected (if external)
   - Check antenna orientation
   - Test with different antenna

**Still Not Working?**
- Check Zigbee network stability
- Verify coordinator health
- Test with different location

---

## 🏠 Home Assistant Issues

### Issue: Device Not Appearing in Home Assistant

**Symptoms:**
- Device not in device list
- No entities created
- Device not discovered

**Possible Causes:**
1. Zigbee pairing failed
2. MQTT not configured (if Zigbee2MQTT)
3. ZHA not configured
4. Device not discovered

**Solutions:**

1. **Verify Zigbee Pairing:**
   - Ensure device paired successfully
   - Check Zigbee coordinator
   - Verify device shows as "Online"

2. **Check MQTT (if Zigbee2MQTT):**
   - Verify MQTT broker running
   - Check MQTT integration configured
   - Verify MQTT topics

3. **Check ZHA:**
   - Verify ZHA integration enabled
   - Check device in ZHA devices
   - Verify device recognized

4. **Restart Home Assistant:**
   - Restart Home Assistant
   - Restart Zigbee coordinator
   - Check logs for errors

**Still Not Working?**
- Check Home Assistant logs
- Verify integration configuration
- Test with manual device add

---

### Issue: Entities Not Updating

**Symptoms:**
- Entities show static values
- Data not updating
- Values don't change

**Possible Causes:**
1. Device not reporting
2. MQTT not working (if Zigbee2MQTT)
3. ZHA not receiving (if ZHA)
4. Entity configuration wrong

**Solutions:**

1. **Verify Device Reporting:**
   - Check device is sending reports
   - Verify report interval
   - Check Zigbee logs

2. **Check MQTT (if Zigbee2MQTT):**
   - Verify MQTT topics
   - Check MQTT publish
   - Test MQTT subscribe

3. **Check ZHA (if ZHA):**
   - Verify ZHA receiving data
   - Check entity configuration
   - Verify cluster bindings

4. **Test Entity Updates:**
   - Manually trigger report
   - Check entity state updates
   - Verify entity attributes

**Still Not Working?**
- Check Home Assistant logs
- Verify entity configuration
- Test with MQTT explorer (if Zigbee2MQTT)

---

## ⚡ Power Issues

### Issue: Power Supply Inadequate

**Symptoms:**
- Device resets frequently
- Unstable operation
- Voltage drops

**Possible Causes:**
1. Power supply current rating too low
2. Power supply voltage incorrect
3. Cable resistance too high
4. Power supply faulty

**Solutions:**

1. **Check Power Supply:**
   - Verify current rating > 500mA
   - Check voltage (should be 5V ±0.25V)
   - Test with different power supply

2. **Check Cable:**
   - Use shorter cable
   - Use thicker gauge wire
   - Check cable resistance

3. **Measure Power Consumption:**
   - Measure actual current draw
   - Verify within expected range
   - Check for power spikes

**Still Not Working?**
- Use higher current power supply
- Verify power supply quality
- Check for power supply issues

---

## 📊 Data Issues

### Issue: Inaccurate Measurements

**Symptoms:**
- Flow rate incorrect
- Volume doesn't match actual
- Measurements inconsistent

**Possible Causes:**
1. Calibration factor incorrect
2. Sensor installation wrong
3. Flow path issues
4. Sensor faulty

**Solutions:**

1. **Recalibrate Sensor:**
   - Flow known volume (10L)
   - Count pulses
   - Calculate calibration factor
   - Update code

2. **Check Sensor Installation:**
   - Verify sensor orientation
   - Check sensor arrow direction
   - Verify sensor in pipe correctly

3. **Check Flow Path:**
   - Verify no air bubbles
   - Check for blockages
   - Ensure flow path clear

4. **Test Sensor:**
   - Test with different volumes
   - Verify consistent results
   - Check for sensor issues

**Still Not Working?**
- Replace sensor
- Verify sensor model correct
- Check sensor specifications

---

## 📞 Getting Help

### Before Asking for Help

1. **Review Documentation:**
   - Check relevant step guides
   - Review troubleshooting guide
   - Search for similar issues

2. **Check Logs:**
   - Serial output
   - Zigbee coordinator logs
   - Home Assistant logs

3. **Test Components:**
   - Test hardware individually
   - Test software components
   - Verify configurations

### Reporting Issues

When reporting issues, include:

1. **Problem Description:**
   - What's happening
   - What should happen
   - Error messages

2. **System Information:**
   - Hardware version
   - Firmware version
   - Software versions

3. **Steps to Reproduce:**
   - Detailed steps
   - Test conditions
   - Expected vs actual results

4. **Logs:**
   - Serial output
   - Zigbee logs
   - Home Assistant logs

5. **Configuration:**
   - Relevant config files
   - Settings
   - Network configuration

### Resources

- [GitHub Issues](https://github.com/yourusername/water-flow-zigbee/issues)
- [Discussions](https://github.com/yourusername/water-flow-zigbee/discussions)
- [Wiki](https://github.com/yourusername/water-flow-zigbee/wiki)

---

**Still having issues?** Open an issue on GitHub with the information above!

