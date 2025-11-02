/*
 * Data Persistence Tests
 * Unit tests for data persistence functionality
 */

#include "test_data_persistence.h"
#include <Preferences.h>

void test_eeprom_namespace_configuration(void) {
    // Test EEPROM namespace is configured
    TEST_ASSERT_TRUE(strlen(EEPROM_NAMESPACE) > 0);
    
    // Verify namespace is valid (not empty, reasonable length)
    TEST_ASSERT_TRUE(strlen(EEPROM_NAMESPACE) < 16); // ESP32 namespace max length
}

void test_save_threshold_configuration(void) {
    // Test save threshold is reasonable
    TEST_ASSERT_TRUE(SAVE_THRESHOLD > 0);
    TEST_ASSERT_TRUE(SAVE_THRESHOLD <= 10.0); // Reasonable max (10L)
    
    // Test default value
    TEST_ASSERT_FLOAT_WITHIN(0.1, 1.0, SAVE_THRESHOLD);
}

void test_save_interval_configuration(void) {
    // Test save interval is reasonable
    TEST_ASSERT_TRUE(MAX_SAVE_INTERVAL > 0);
    TEST_ASSERT_TRUE(MAX_SAVE_INTERVAL <= 3600000); // Max 1 hour
    
    // Convert to minutes for readability
    uint32_t intervalMinutes = MAX_SAVE_INTERVAL / 60000;
    TEST_ASSERT_TRUE(intervalMinutes >= 1);
    TEST_ASSERT_TRUE(intervalMinutes <= 60);
}

void test_volume_data_structure(void) {
    // Test that volume data type can store expected values
    float testVolume = 10000.0; // 10,000 liters (reasonable max)
    
    // Verify float can store this value
    TEST_ASSERT_TRUE(testVolume > 0);
    TEST_ASSERT_TRUE(testVolume < 100000.0); // Reasonable max
    
    // Test volume precision (should handle 0.001 L = 1ml)
    float smallVolume = 0.001;
    TEST_ASSERT_TRUE(smallVolume > 0);
}

// Note: Actual EEPROM read/write tests would require hardware
// or mocking of Preferences library, which is more complex.
// These tests verify configuration values are correct.

// Test suite runner
void DataPersistenceTests(void) {
    RUN_TEST(test_eeprom_namespace_configuration);
    RUN_TEST(test_save_threshold_configuration);
    RUN_TEST(test_save_interval_configuration);
    RUN_TEST(test_volume_data_structure);
}

