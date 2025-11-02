/*
 * Integration Tests
 * Tests for system integration and component interaction
 */

#ifndef TEST_INTEGRATION_H
#define TEST_INTEGRATION_H

#include <unity.h>
#include <Arduino.h>
#include "../include/config.h"

// Test suite declarations
void test_configuration_consistency(void);
void test_zigbee_configuration(void);
void test_report_intervals(void);
void test_pin_conflicts(void);
void test_memory_usage_estimates(void);

// Test suite runner
void IntegrationTests(void);

#endif // TEST_INTEGRATION_H

