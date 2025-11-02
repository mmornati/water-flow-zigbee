/*
 * Flow Calculation Tests
 * Tests for flow rate and volume calculation functions
 */

#ifndef TEST_FLOW_CALCULATION_H
#define TEST_FLOW_CALCULATION_H

#include <unity.h>
#include <Arduino.h>
#include "../include/config.h"

// Test suite declarations
void test_flow_rate_calculation_basic(void);
void test_flow_rate_calculation_zero_pulses(void);
void test_flow_rate_calculation_high_flow(void);
void test_volume_accumulation(void);
void test_volume_accuracy(void);
void test_flow_stopped_detection(void);

// Test suite runner
void FlowCalculationTests(void);

#endif // TEST_FLOW_CALCULATION_H

