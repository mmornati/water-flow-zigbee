/*
 * Battery Monitor Tests
 * Unit tests for battery monitoring (if enabled)
 */

#include "test_battery_monitor.h"

#if BATTERY_ENABLED

void test_battery_voltage_reading(void) {
    // Test voltage divider calculation
    // With 1:2 divider, ADC reads half of battery voltage
    // Battery = ADC * 2
    
    // Simulate ADC reading of 1925mV (half of 3.85V battery)
    uint32_t adcVoltage_mV = 1925;
    float batteryVoltage = (adcVoltage_mV / 1000.0) * 2.0;
    
    TEST_ASSERT_FLOAT_WITHIN(0.01, 3.85, batteryVoltage);
}

void test_battery_percentage_calculation(void) {
    // Test battery percentage calculation
    // Percentage = ((voltage - min) / (max - min)) * 100
    
    // Test: 4.2V should be 100%
    float voltage = BATTERY_MAX_VOLTAGE;
    float percentage = ((voltage - BATTERY_MIN_VOLTAGE) / 
                        (BATTERY_MAX_VOLTAGE - BATTERY_MIN_VOLTAGE)) * 100.0;
    
    TEST_ASSERT_FLOAT_WITHIN(0.1, 100.0, percentage);
    
    // Test: 3.6V should be 50%
    voltage = 3.6;
    percentage = ((voltage - BATTERY_MIN_VOLTAGE) / 
                  (BATTERY_MAX_VOLTAGE - BATTERY_MIN_VOLTAGE)) * 100.0;
    
    TEST_ASSERT_FLOAT_WITHIN(0.1, 50.0, percentage);
    
    // Test: 3.0V should be 0%
    voltage = BATTERY_MIN_VOLTAGE;
    percentage = ((voltage - BATTERY_MIN_VOLTAGE) / 
                  (BATTERY_MAX_VOLTAGE - BATTERY_MIN_VOLTAGE)) * 100.0;
    
    TEST_ASSERT_FLOAT_WITHIN(0.1, 0.0, percentage);
}

void test_battery_voltage_range(void) {
    // Test voltage range limits
    TEST_ASSERT_TRUE(BATTERY_MIN_VOLTAGE > 0);
    TEST_ASSERT_TRUE(BATTERY_MAX_VOLTAGE > BATTERY_MIN_VOLTAGE);
    TEST_ASSERT_TRUE(BATTERY_MAX_VOLTAGE <= 4.3); // Safety limit
    
    // Test typical values
    TEST_ASSERT_FLOAT_WITHIN(0.1, 3.0, BATTERY_MIN_VOLTAGE);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 4.2, BATTERY_MAX_VOLTAGE);
}

void test_battery_percentage_range(void) {
    // Test percentage clamping
    float voltage = BATTERY_MAX_VOLTAGE + 0.5; // Over max
    voltage = constrain(voltage, BATTERY_MIN_VOLTAGE, BATTERY_MAX_VOLTAGE);
    
    TEST_ASSERT_FLOAT_WITHIN(0.01, BATTERY_MAX_VOLTAGE, voltage);
    
    voltage = BATTERY_MIN_VOLTAGE - 0.5; // Below min
    voltage = constrain(voltage, BATTERY_MIN_VOLTAGE, BATTERY_MAX_VOLTAGE);
    
    TEST_ASSERT_FLOAT_WITHIN(0.01, BATTERY_MIN_VOLTAGE, voltage);
}

void test_battery_warning_levels(void) {
    // Test warning level thresholds
    TEST_ASSERT_TRUE(BATTERY_WARNING_LEVEL > BATTERY_CRITICAL_LEVEL);
    TEST_ASSERT_TRUE(BATTERY_WARNING_LEVEL <= 100);
    TEST_ASSERT_TRUE(BATTERY_CRITICAL_LEVEL >= 0);
    TEST_ASSERT_TRUE(BATTERY_CRITICAL_LEVEL < BATTERY_WARNING_LEVEL);
    
    // Test typical values
    TEST_ASSERT_EQUAL(25, BATTERY_WARNING_LEVEL);
    TEST_ASSERT_EQUAL(10, BATTERY_CRITICAL_LEVEL);
}

void test_voltage_divider_calculation(void) {
    // Test voltage divider ratio (1:2)
    // With 200kΩ + 200kΩ divider, ratio = 200k / (200k + 200k) = 0.5
    
    float dividerRatio = 0.5; // 1:2 voltage divider
    
    // Test: Battery 3.85V should give ADC reading of 1.925V
    float batteryVoltage = 3.85;
    float adcVoltage = batteryVoltage * dividerRatio;
    
    TEST_ASSERT_FLOAT_WITHIN(0.01, 1.925, adcVoltage);
    
    // Reverse: ADC 1.925V should give battery 3.85V
    float calculatedBattery = (adcVoltage / dividerRatio);
    TEST_ASSERT_FLOAT_WITHIN(0.01, batteryVoltage, calculatedBattery);
}

// Test suite runner
void BatteryMonitorTests(void) {
    RUN_TEST(test_battery_voltage_reading);
    RUN_TEST(test_battery_percentage_calculation);
    RUN_TEST(test_battery_voltage_range);
    RUN_TEST(test_battery_percentage_range);
    RUN_TEST(test_battery_warning_levels);
    RUN_TEST(test_voltage_divider_calculation);
}

#endif // BATTERY_ENABLED

