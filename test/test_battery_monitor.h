/*
 * Battery Monitor Tests
 * Tests for battery monitoring functionality (if enabled)
 */

#ifndef TEST_BATTERY_MONITOR_H
#define TEST_BATTERY_MONITOR_H

#include <unity.h>
#include <Arduino.h>
#include "../include/config.h"

#if BATTERY_ENABLED

// Test suite declarations
void test_battery_voltage_reading(void);
void test_battery_percentage_calculation(void);
void test_battery_voltage_range(void);
void test_battery_percentage_range(void);
void test_battery_warning_levels(void);
void test_voltage_divider_calculation(void);

// Test suite runner
void BatteryMonitorTests(void);

#endif // BATTERY_ENABLED

#endif // TEST_BATTERY_MONITOR_H

