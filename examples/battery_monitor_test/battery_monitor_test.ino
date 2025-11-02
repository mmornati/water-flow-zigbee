/*
 * Battery Monitor Test
 * Test sketch for battery voltage monitoring
 * 
 * This sketch tests:
 * - Battery voltage reading
 * - Battery percentage calculation
 * - Voltage divider calibration
 * 
 * Hardware:
 * - XIAO ESP32C6
 * - Voltage divider on A0 (GPIO4)
 * - 3.7V Li-ion battery
 */

#define BATTERY_PIN A0
#define BATTERY_MIN_VOLTAGE 3.0
#define BATTERY_MAX_VOLTAGE 4.2

float readBatteryVoltage() {
    uint32_t voltage_sum = 0;
    
    // Average 16 readings for stability
    for(int i = 0; i < 16; i++) {
        voltage_sum += analogReadMilliVolts(BATTERY_PIN);
        delay(10);
    }
    
    // Average and compensate for 1:2 voltage divider
    float voltage = (voltage_sum / 16.0 / 1000.0) * 2.0;
    return voltage;
}

uint8_t getBatteryPercentage() {
    float voltage = readBatteryVoltage();
    
    // Constrain to valid range
    voltage = constrain(voltage, BATTERY_MIN_VOLTAGE, BATTERY_MAX_VOLTAGE);
    
    // Calculate percentage
    float percentage = ((voltage - BATTERY_MIN_VOLTAGE) / 
                        (BATTERY_MAX_VOLTAGE - BATTERY_MIN_VOLTAGE)) * 100.0;
    
    return (uint8_t)percentage;
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n========================================");
    Serial.println("Battery Monitor Test");
    Serial.println("========================================");
    Serial.println("Battery Range: " + String(BATTERY_MIN_VOLTAGE) + 
                  "V - " + String(BATTERY_MAX_VOLTAGE) + "V");
    Serial.println();
    
    pinMode(BATTERY_PIN, INPUT);
    
    Serial.println("Battery monitor initialized on pin " + String(BATTERY_PIN));
    Serial.println();
}

void loop() {
    // Read raw ADC value
    uint32_t rawADC = analogRead(BATTERY_PIN);
    uint32_t adcVoltage = analogReadMilliVolts(BATTERY_PIN);
    
    // Calculate actual battery voltage
    float batteryVoltage = readBatteryVoltage();
    uint8_t batteryPercent = getBatteryPercentage();
    
    // Print results
    Serial.println("---------------------------------");
    Serial.println("Raw ADC: " + String(rawADC));
    Serial.println("ADC Voltage: " + String(adcVoltage) + " mV");
    Serial.println("ADC Voltage: " + String(adcVoltage / 1000.0, 3) + " V");
    Serial.println("Battery Voltage: " + String(batteryVoltage, 3) + " V");
    Serial.println("Battery Percentage: " + String(batteryPercent) + " %");
    
    // Battery level indicator
    if (batteryPercent >= 75) {
        Serial.println("Status: FULL");
    } else if (batteryPercent >= 50) {
        Serial.println("Status: GOOD");
    } else if (batteryPercent >= 25) {
        Serial.println("Status: LOW");
    } else if (batteryPercent >= 10) {
        Serial.println("Status: CRITICAL");
    } else {
        Serial.println("Status: EMPTY");
    }
    
    Serial.println("---------------------------------");
    Serial.println();
    
    delay(2000);
}

