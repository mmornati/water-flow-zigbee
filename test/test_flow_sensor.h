/*
 * Flow Sensor Tests
 * Tests for flow sensor functionality
 */

#ifndef TEST_FLOW_SENSOR_H
#define TEST_FLOW_SENSOR_H

#include <unity.h>
#include <Arduino.h>
#include "../include/config.h"

// Test suite declarations
void test_flow_sensor_pin_configuration(void);
void test_pulse_counting_logic(void);
void test_interrupt_handler_assignment(void);
void test_calibration_factor_range(void);

// Test suite runner
void FlowSensorTests(void);

#endif // TEST_FLOW_SENSOR_H

