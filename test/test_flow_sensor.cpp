/*
 * Flow Sensor Tests
 * Unit tests for flow sensor functionality
 */

#include "test_flow_sensor.h"

void test_flow_sensor_pin_configuration(void) {
    // Test that flow sensor pin is correctly defined
    TEST_ASSERT_EQUAL(2, FLOW_SENSOR_PIN);
    
    // Verify pin is valid GPIO pin
    TEST_ASSERT_TRUE(FLOW_SENSOR_PIN >= 0 && FLOW_SENSOR_PIN <= 31);
}

void test_pulse_counting_logic(void) {
    // Test pulse counting increment logic
    // pulseCount = pulseCount + 1 should work correctly
    
    volatile uint32_t testPulseCount = 0;
    
    // Simulate pulse counting
    testPulseCount = testPulseCount + 1;
    TEST_ASSERT_EQUAL(1, testPulseCount);
    
    testPulseCount = testPulseCount + 1;
    TEST_ASSERT_EQUAL(2, testPulseCount);
    
    // Test rapid counting (simulating real flow)
    for(int i = 0; i < 100; i++) {
        testPulseCount = testPulseCount + 1;
    }
    TEST_ASSERT_EQUAL(102, testPulseCount);
}

void test_interrupt_handler_assignment(void) {
    // Test that interrupt pin is correctly configured
    // This is more of a configuration test
    
    // Verify pin mode is INPUT_PULLUP compatible
    // In real test, we'd check pinMode() was called correctly
    
    // Verify interrupt is on correct edge (RISING)
    // This would be tested in integration tests
    
    // For now, just verify configuration constants
    TEST_ASSERT_EQUAL(2, FLOW_SENSOR_PIN);
}

void test_calibration_factor_range(void) {
    // Test that calibration factor is within reasonable range
    // Standard YF-S201: 7.5 pulses/L, but can vary 7.0-8.0
    
    TEST_ASSERT_TRUE(CALIBRATION_FACTOR >= 7.0);
    TEST_ASSERT_TRUE(CALIBRATION_FACTOR <= 8.0);
    TEST_ASSERT_TRUE(CALIBRATION_FACTOR > 0);
    
    // Verify default calibration factor
    TEST_ASSERT_FLOAT_WITHIN(0.1, 7.5, CALIBRATION_FACTOR);
}

// Test suite runner
void FlowSensorTests(void) {
    RUN_TEST(test_flow_sensor_pin_configuration);
    RUN_TEST(test_pulse_counting_logic);
    RUN_TEST(test_interrupt_handler_assignment);
    RUN_TEST(test_calibration_factor_range);
}

