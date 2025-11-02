/*
 * Water Flow Meter - Test Suite
 * Main test runner using Unity framework
 */

#include <unity.h>
#include <Arduino.h>

// Include test modules
#include "test_flow_sensor.h"
#include "test_flow_calculation.h"
#include "test_data_persistence.h"
#include "test_integration.h"

#if BATTERY_ENABLED
#include "test_battery_monitor.h"
#endif

void setUp(void) {
    // Set up test environment before each test
    // This function runs before every test
}

void tearDown(void) {
    // Clean up after each test
    // This function runs after every test
}

void setup() {
    // Wait for serial monitor to connect (2 seconds)
    delay(2000);
    
    UNITY_BEGIN();    // Start Unity test framework
    
    Serial.println("\n\n========================================");
    Serial.println("Water Flow Meter - Test Suite");
    Serial.println("========================================");
    Serial.println();
    
    // Run test suites
    FlowSensorTests();
    FlowCalculationTests();
    
    #if BATTERY_ENABLED
    BatteryMonitorTests();
    #endif
    
    DataPersistenceTests();
    IntegrationTests();
    
    UNITY_END();    // End Unity test framework
    
    Serial.println("\n========================================");
    Serial.println("All Tests Complete");
    Serial.println("========================================");
}

void loop() {
    // Tests run once in setup()
    // Keep loop empty or add continuous monitoring if needed
    delay(1000);
}

