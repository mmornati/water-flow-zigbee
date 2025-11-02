/*
 * Water Flow Meter with Zigbee
 * Always-on water flow meter using XIAO ESP32C6 with Zigbee connectivity
 * 
 * Hardware:
 * - Seeed Studio XIAO ESP32C6
 * - YF-S201 Hall Effect Water Flow Sensor
 * - Optional: 3.7V Li-ion battery with voltage divider
 * 
 * Features:
 * - Always-on operation (no sleep modes)
 * - Real-time flow rate measurement (L/min)
 * - Cumulative volume tracking (L)
 * - Optional battery monitoring
 * - Zigbee communication for Home Assistant
 * - EEPROM data persistence
 * 
 * Author: Water Flow Meter Project
 * License: MIT
 */

#include <Arduino.h>
#include <Preferences.h>
#include "config.h"

// Note: Zigbee libraries are included via ESP32 board package
// Actual implementation depends on ESP32 Zigbee SDK version
// This is a template - adjust based on your ESP32 Zigbee SDK API

// ============================================================================
// Global Variables
// ============================================================================

// Flow Sensor
volatile uint32_t pulseCount = 0;
volatile unsigned long lastPulseTime = 0;
unsigned long lastFlowCheck = 0;

// Flow Data
float flowRate = 0.0;           // Current flow rate (L/min)
float totalVolume = 0.0;        // Cumulative volume (L)

// Battery (if enabled)
float batteryVoltage = 0.0;
uint8_t batteryPercent = 100;

// Zigbee
bool zigbeeInitialized = false;
bool zigbeeConnected = false;
uint16_t zigbeeShortAddr = 0xFFFF;

// Data Persistence
Preferences prefs;
float lastSavedVolume = 0.0;
unsigned long lastSaveTime = 0;

// System Status
unsigned long bootTime = 0;
uint32_t bootCount = 0;

// ============================================================================
// Flow Sensor Functions
// ============================================================================

/**
 * Interrupt handler for flow sensor pulses
 * MUST remain active at all times - never disable this interrupt
 */
void IRAM_ATTR pulseCounter() {
    // Use atomic increment to avoid volatile warning with C++14+
    pulseCount = pulseCount + 1;
    lastPulseTime = millis();
}

/**
 * Initialize flow sensor with interrupt
 */
void setupFlowSensor() {
    pinMode(FLOW_SENSOR_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), 
                    pulseCounter, RISING);
    
    if (DEBUG_ENABLED) {
        Serial.println("[Flow Sensor] Initialized on pin " + String(FLOW_SENSOR_PIN));
        Serial.println("[Flow Sensor] Interrupt attached - ALWAYS ACTIVE");
    }
}

/**
 * Calculate flow rate and update total volume
 * Called every second from main loop
 */
void calculateFlow() {
    static unsigned long lastCheck = 0;
    static uint32_t lastPulseCount = 0;
    
    unsigned long now = millis();
    
    if (now - lastCheck >= FLOW_CALC_INTERVAL) {
        // Calculate current flow rate
        uint32_t pulsesThisSecond = pulseCount - lastPulseCount;
        
        if (pulsesThisSecond > 0) {
            // Flow rate = (pulses/second) / calibration_factor * 60 (L/min)
            flowRate = (pulsesThisSecond / CALIBRATION_FACTOR) * 60.0;
            
            // Update total volume (each pulse = 1/calibration_factor liters)
            float volumeThisSecond = pulsesThisSecond / (CALIBRATION_FACTOR * 60.0);
            totalVolume += volumeThisSecond;
            
            if (DEBUG_ENABLED) {
                Serial.println("[Flow] Rate: " + String(flowRate, 2) + 
                              " L/min, Volume: " + String(totalVolume, 3) + " L");
            }
        } else {
            // Check if flow has stopped
            if ((now - lastPulseTime) > FLOW_IDLE_TIMEOUT) {
                flowRate = 0.0;
                if (DEBUG_ENABLED && flowRate > 0) {
                    Serial.println("[Flow] Flow stopped - rate set to 0");
                }
            }
        }
        
        // Update for next calculation
        lastPulseCount = pulseCount;
        lastCheck = now;
    }
}

// ============================================================================
// Battery Monitor Functions (Optional)
// ============================================================================

#if BATTERY_ENABLED

/**
 * Read battery voltage via voltage divider
 * Returns voltage in volts
 */
float readBatteryVoltage() {
    uint32_t voltage_sum = 0;
    
    // Average multiple readings for stability
    for(int i = 0; i < 16; i++) {
        voltage_sum += analogReadMilliVolts(BATTERY_PIN);
        delay(10);
    }
    
    // Average and compensate for 1:2 voltage divider
    float voltage = (voltage_sum / 16.0 / 1000.0) * 2.0;
    return voltage;
}

/**
 * Get battery percentage (0-100)
 */
uint8_t getBatteryPercentage() {
    float voltage = readBatteryVoltage();
    
    // Constrain to valid range
    voltage = constrain(voltage, BATTERY_MIN_VOLTAGE, BATTERY_MAX_VOLTAGE);
    
    // Calculate percentage
    float percentage = ((voltage - BATTERY_MIN_VOLTAGE) / 
                        (BATTERY_MAX_VOLTAGE - BATTERY_MIN_VOLTAGE)) * 100.0;
    
    return (uint8_t)percentage;
}

/**
 * Check battery level and handle warnings
 */
void checkBatteryLevel() {
    if (batteryPercent < BATTERY_CRITICAL_LEVEL) {
        Serial.println("[Battery] CRITICAL: Battery at " + 
                       String(batteryPercent) + "%");
        // TODO: Send critical alert via Zigbee
    } else if (batteryPercent < BATTERY_WARNING_LEVEL) {
        Serial.println("[Battery] WARNING: Battery at " + 
                       String(batteryPercent) + "%");
        // TODO: Send warning via Zigbee
    }
}

/**
 * Initialize battery monitoring
 */
void setupBatteryMonitor() {
    pinMode(BATTERY_PIN, INPUT);
    batteryVoltage = readBatteryVoltage();
    batteryPercent = getBatteryPercentage();
    
    if (DEBUG_ENABLED) {
        Serial.println("[Battery] Monitor initialized");
        Serial.println("[Battery] Voltage: " + String(batteryVoltage, 2) + 
                      "V, Percentage: " + String(batteryPercent) + "%");
    }
}

#endif // BATTERY_ENABLED

// ============================================================================
// Data Persistence Functions
// ============================================================================

/**
 * Load total volume from EEPROM
 */
void loadTotalVolume() {
    prefs.begin(EEPROM_NAMESPACE, true);  // Read-only
    
    totalVolume = prefs.getFloat("totalVolume", 0.0);
    uint64_t savedPulses = prefs.getULong64("totalPulses", 0);
    bootCount = prefs.getUInt("bootCount", 0);
    
    bootCount++;
    prefs.end();
    
    // Write back boot count
    prefs.begin(EEPROM_NAMESPACE, false);
    prefs.putUInt("bootCount", bootCount);
    prefs.end();
    
    if (DEBUG_ENABLED) {
        Serial.println("[EEPROM] Loaded total volume: " + 
                      String(totalVolume, 3) + " L");
        Serial.println("[EEPROM] Total pulses: " + String(savedPulses));
        Serial.println("[EEPROM] Boot count: " + String(bootCount));
    }
    
    lastSavedVolume = totalVolume;
}

/**
 * Save total volume to EEPROM
 */
void saveTotalVolume() {
    prefs.begin(EEPROM_NAMESPACE, false);
    
    prefs.putFloat("totalVolume", totalVolume);
    prefs.putULong64("totalPulses", (uint64_t)pulseCount);
    
    prefs.end();
    
    if (DEBUG_ENABLED) {
        Serial.println("[EEPROM] Saved total volume: " + 
                      String(totalVolume, 3) + " L");
    }
    
    lastSavedVolume = totalVolume;
    lastSaveTime = millis();
}

/**
 * Periodic save - saves data periodically to reduce EEPROM wear
 */
void periodicSave() {
    unsigned long now = millis();
    
    // Save if volume changed significantly
    if (abs(totalVolume - lastSavedVolume) >= SAVE_THRESHOLD) {
        saveTotalVolume();
    }
    
    // Or save periodically even if volume hasn't changed much
    if ((now - lastSaveTime) > MAX_SAVE_INTERVAL) {
        saveTotalVolume();
    }
}

// ============================================================================
// Zigbee Functions
// ============================================================================

/**
 * Initialize Zigbee stack
 * NOTE: This is a template - actual API depends on ESP32 Zigbee SDK version
 * You'll need to adjust based on your SDK:
 * - ESP-IDF Zigbee SDK
 * - Arduino ESP32 Zigbee library
 * - Espressif ESP-ZB library
 */
void setupZigbee() {
    if (DEBUG_ENABLED) {
        Serial.println("[Zigbee] Initializing Zigbee stack...");
    }
    
    // TODO: Initialize Zigbee stack based on your SDK
    // Example (conceptual - adjust for your SDK):
    // esp_zb_init();
    // esp_zb_set_channel(ZIGBEE_CHANNEL);
    // esp_zb_set_pan_id(ZIGBEE_PAN_ID);
    
    zigbeeInitialized = true;
    
    if (DEBUG_ENABLED) {
        Serial.println("[Zigbee] Stack initialized");
    }
}

/**
 * Join Zigbee network
 */
void joinZigbeeNetwork() {
    if (!zigbeeInitialized) {
        Serial.println("[Zigbee] ERROR: Stack not initialized!");
        return;
    }
    
    if (DEBUG_ENABLED) {
        Serial.println("[Zigbee] Joining network...");
        Serial.println("[Zigbee] Ensure coordinator is in pairing mode!");
    }
    
    // TODO: Join Zigbee network based on your SDK
    // Example (conceptual):
    // esp_zb_join();
    
    // Wait for join (with timeout)
    unsigned long joinStart = millis();
    const unsigned long JOIN_TIMEOUT = 60000;  // 60 seconds
    
    while (!zigbeeConnected && (millis() - joinStart < JOIN_TIMEOUT)) {
        // Process Zigbee events
        // esp_zb_process();
        delay(100);
        
        if (DEBUG_ENABLED && (millis() - joinStart) % 5000 < 100) {
            Serial.println("[Zigbee] Still joining network...");
        }
    }
    
    if (zigbeeConnected) {
        Serial.println("[Zigbee] Successfully joined network!");
        Serial.println("[Zigbee] Short Address: 0x" + 
                      String(zigbeeShortAddr, HEX));
    } else {
        Serial.println("[Zigbee] Failed to join network (timeout)");
        Serial.println("[Zigbee] Check coordinator is in pairing mode");
    }
}

/**
 * Send flow data report to Zigbee coordinator
 */
void sendFlowReport(float flowRate, float totalVolume, uint8_t batteryPercent) {
    if (!zigbeeConnected) {
        return;
    }
    
    if (DEBUG_ENABLED) {
        Serial.println("[Zigbee] Reporting flow data:");
        Serial.println("  Flow Rate: " + String(flowRate, 2) + " L/min");
        Serial.println("  Total Volume: " + String(totalVolume, 3) + " L");
        Serial.println("  Battery: " + String(batteryPercent) + "%");
    }
    
    // TODO: Send Zigbee report based on your SDK
    // Example (conceptual):
    // esp_zb_report_attribute(FLOW_ENDPOINT, FLOW_CLUSTER_ID, 
    //                         FLOW_RATE_ATTR, &flowRate, sizeof(float));
    // esp_zb_report_attribute(FLOW_ENDPOINT, FLOW_CLUSTER_ID,
    //                         VOLUME_ATTR, &totalVolume, sizeof(float));
    // 
    // if (BATTERY_ENABLED) {
    //     esp_zb_report_attribute(BATTERY_ENDPOINT, BATTERY_CLUSTER_ID,
    //                           BATTERY_PERCENT_ATTR, &batteryPercent, sizeof(uint8_t));
    // }
}

/**
 * Check if flow data should be reported
 * Reports periodically or on significant changes
 */
bool shouldReportFlow(float currentFlow, float currentVolume, uint8_t currentBattery) {
    static unsigned long lastReportTime = 0;
    static float lastReportedFlow = 0.0;
    static float lastReportedVolume = 0.0;
    static uint8_t lastReportedBattery = 0;
    
    unsigned long now = millis();
    bool shouldReport = false;
    
    // Report periodically
    if (now - lastReportTime > (FLOW_REPORT_INTERVAL * 1000)) {
        shouldReport = true;
    }
    
    // Report on significant flow rate change (>10%)
    if (abs(currentFlow - lastReportedFlow) > (lastReportedFlow * FLOW_RATE_CHANGE_THRESHOLD)) {
        shouldReport = true;
    }
    
    // Report on volume milestone (every 1L)
    if (abs(currentVolume - lastReportedVolume) >= VOLUME_MILESTONE) {
        shouldReport = true;
    }
    
    #if BATTERY_ENABLED
    // Report on battery change (>5%)
    if (abs(currentBattery - lastReportedBattery) >= BATTERY_CHANGE_THRESHOLD) {
        shouldReport = true;
    }
    #endif
    
    if (shouldReport) {
        sendFlowReport(currentFlow, currentVolume, currentBattery);
        lastReportedFlow = currentFlow;
        lastReportedVolume = currentVolume;
        lastReportedBattery = currentBattery;
        lastReportTime = now;
    }
    
    return shouldReport;
}

// ============================================================================
// System Functions
// ============================================================================

/**
 * Print system status
 */
void printSystemStatus() {
    Serial.println("\n========================================");
    Serial.println("Water Flow Meter - System Status");
    Serial.println("========================================");
    Serial.println("Boot #" + String(bootCount));
    Serial.println("Uptime: " + String((millis() - bootTime) / 1000) + " seconds");
    Serial.println();
    Serial.println("Flow Sensor:");
    Serial.println("  Flow Rate: " + String(flowRate, 2) + " L/min");
    Serial.println("  Total Volume: " + String(totalVolume, 3) + " L");
    Serial.println("  Total Pulses: " + String(pulseCount));
    Serial.println("  Status: " + String(flowRate > 0.1 ? "FLOWING" : "IDLE"));
    Serial.println();
    
    #if BATTERY_ENABLED
    Serial.println("Battery:");
    Serial.println("  Voltage: " + String(batteryVoltage, 2) + " V");
    Serial.println("  Percentage: " + String(batteryPercent) + " %");
    Serial.println();
    #endif
    
    Serial.println("Zigbee:");
    Serial.println("  Status: " + String(zigbeeConnected ? "CONNECTED" : "DISCONNECTED"));
    if (zigbeeConnected) {
        Serial.println("  Short Address: 0x" + String(zigbeeShortAddr, HEX));
    }
    Serial.println("========================================\n");
}

// ============================================================================
// Setup Function
// ============================================================================

void setup() {
    // Initialize serial
    Serial.begin(SERIAL_BAUD_RATE);
    delay(1000);
    
    Serial.println("\n\n========================================");
    Serial.println("Water Flow Meter Starting");
    Serial.println("========================================");
    
    bootTime = millis();
    
    // 1. Load persisted data from EEPROM
    loadTotalVolume();
    
    // 2. Initialize flow sensor with interrupt (ALWAYS ACTIVE)
    setupFlowSensor();
    
    // 3. Initialize battery monitoring (if enabled)
    #if BATTERY_ENABLED
    setupBatteryMonitor();
    #endif
    
    // 4. Initialize Zigbee stack
    setupZigbee();
    
    // 5. Join Zigbee network
    joinZigbeeNetwork();
    
    // 6. Initialize status LED
    pinMode(LED_PIN, OUTPUT);
    
    Serial.println("\n[System] Setup complete - System ready!");
    Serial.println("[System] Always-on operation - no sleep modes");
    Serial.println();
    
    // Print initial status
    printSystemStatus();
}

// ============================================================================
// Main Loop
// ============================================================================

void loop() {
    // 1. Calculate flow rate and volume (always running)
    calculateFlow();
    
    // 2. Save data periodically (reduce EEPROM wear)
    periodicSave();
    
    // 3. Check battery (if enabled, every minute)
    #if BATTERY_ENABLED
    static unsigned long lastBatteryCheck = 0;
    if (millis() - lastBatteryCheck > BATTERY_CHECK_INTERVAL) {
        batteryVoltage = readBatteryVoltage();
        batteryPercent = getBatteryPercentage();
        checkBatteryLevel();
        lastBatteryCheck = millis();
    }
    #endif
    
    // 4. Send Zigbee reports (periodically or on significant changes)
    if (zigbeeConnected) {
        shouldReportFlow(flowRate, totalVolume, batteryPercent);
        
        // Process Zigbee events
        // TODO: esp_zb_process();  // Uncomment when Zigbee SDK is configured
    }
    
    // 5. Status LED blinking (heartbeat)
    static unsigned long lastLedBlink = 0;
    if (millis() - lastLedBlink > STATUS_LED_INTERVAL) {
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
        lastLedBlink = millis();
    }
    
    // 6. Periodic status print (every 60 seconds)
    static unsigned long lastStatusPrint = 0;
    if (DEBUG_ENABLED && (millis() - lastStatusPrint > 60000)) {
        printSystemStatus();
        lastStatusPrint = millis();
    }
    
    // Small delay to prevent CPU spinning
    delay(10);
}

