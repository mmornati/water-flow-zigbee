/*
 * Data Persistence Tests
 * Tests for EEPROM/Preferences data persistence
 */

#ifndef TEST_DATA_PERSISTENCE_H
#define TEST_DATA_PERSISTENCE_H

#include <unity.h>
#include <Arduino.h>
#include "../include/config.h"

// Test suite declarations
void test_eeprom_namespace_configuration(void);
void test_save_threshold_configuration(void);
void test_save_interval_configuration(void);
void test_volume_data_structure(void);

// Test suite runner
void DataPersistenceTests(void);

#endif // TEST_DATA_PERSISTENCE_H

