/*
 * Integration Tests
 * Tests for system integration and component interaction
 */

#include "test_integration.h"

void test_configuration_consistency(void) {
    // Test that all configuration values are consistent
    
    // Flow sensor pin should be valid
    TEST_ASSERT_TRUE(FLOW_SENSOR_PIN >= 0);
    
    // Battery pin should be different from flow sensor pin (if both enabled)
    #if BATTERY_ENABLED
    TEST_ASSERT_NOT_EQUAL(FLOW_SENSOR_PIN, BATTERY_PIN);
    #endif
    
    // Calibration factor should be positive
    TEST_ASSERT_TRUE(CALIBRATION_FACTOR > 0);
    
    // Intervals should be reasonable
    TEST_ASSERT_TRUE(FLOW_CALC_INTERVAL > 0);
    TEST_ASSERT_TRUE(FLOW_IDLE_TIMEOUT > 0);
}

void test_zigbee_configuration(void) {
    // Test Zigbee configuration values
    
    // Channel should be in valid range (11-26)
    TEST_ASSERT_TRUE(ZIGBEE_CHANNEL >= 11);
    TEST_ASSERT_TRUE(ZIGBEE_CHANNEL <= 26);
    
    // PAN ID should be valid (non-zero)
    TEST_ASSERT_TRUE(ZIGBEE_PAN_ID != 0);
    
    // Endpoints should be valid (1-240)
    TEST_ASSERT_TRUE(FLOW_ENDPOINT >= 1);
    TEST_ASSERT_TRUE(FLOW_ENDPOINT <= 240);
    
    #if BATTERY_ENABLED
    TEST_ASSERT_TRUE(BATTERY_ENDPOINT >= 1);
    TEST_ASSERT_TRUE(BATTERY_ENDPOINT <= 240);
    #endif
}

void test_report_intervals(void) {
    // Test report intervals are reasonable
    
    // Flow report interval should be reasonable (every 10s to 5min)
    TEST_ASSERT_TRUE(FLOW_REPORT_INTERVAL >= 10);
    TEST_ASSERT_TRUE(FLOW_REPORT_INTERVAL <= 300);
    
    #if BATTERY_ENABLED
    // Battery report interval should be reasonable (every 1min to 1hour)
    TEST_ASSERT_TRUE(BATTERY_REPORT_INTERVAL >= 60);
    TEST_ASSERT_TRUE(BATTERY_REPORT_INTERVAL <= 3600);
    #endif
}

void test_pin_conflicts(void) {
    // Test that pins don't conflict
    
    // Flow sensor and battery pins should be different
    #if BATTERY_ENABLED
    TEST_ASSERT_NOT_EQUAL(FLOW_SENSOR_PIN, BATTERY_PIN);
    #endif
    
    // LED pin should be valid
    TEST_ASSERT_TRUE(LED_PIN >= 0);
}

void test_memory_usage_estimates(void) {
    // Test that data structures are reasonable size
    
    // pulseCount (uint32_t) = 4 bytes
    TEST_ASSERT_EQUAL(4, sizeof(uint32_t));
    
    // flowRate (float) = 4 bytes
    TEST_ASSERT_EQUAL(4, sizeof(float));
    
    // totalVolume (float) = 4 bytes
    TEST_ASSERT_EQUAL(4, sizeof(float));
    
    // Total for flow data: ~16 bytes (reasonable)
    size_t flowDataSize = sizeof(volatile uint32_t) + sizeof(float) + sizeof(float);
    TEST_ASSERT_TRUE(flowDataSize < 100); // Should be small
}

// Test suite runner
void IntegrationTests(void) {
    RUN_TEST(test_configuration_consistency);
    RUN_TEST(test_zigbee_configuration);
    RUN_TEST(test_report_intervals);
    RUN_TEST(test_pin_conflicts);
    RUN_TEST(test_memory_usage_estimates);
}

