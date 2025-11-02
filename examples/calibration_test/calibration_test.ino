/*
 * Calibration Test
 * Test sketch for calibrating YF-S201 flow sensor
 * 
 * This sketch:
 * - Measures known volume of water
 * - Calculates calibration factor
 * - Verifies accuracy
 * 
 * Procedure:
 * 1. Flow exactly 10.0L through sensor
 * 2. Press any key to start measurement
 * 3. Flow water through sensor
 * 4. Results will be displayed
 */

#define FLOW_SENSOR_PIN 2
#define TEST_VOLUME_LITERS 10.0  // Known test volume

volatile uint32_t totalPulses = 0;
bool testRunning = false;
unsigned long testStartTime = 0;

void IRAM_ATTR pulseCounter() {
    if (testRunning) {
        totalPulses++;
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n========================================");
    Serial.println("Flow Sensor Calibration Test");
    Serial.println("========================================");
    Serial.println("Test Volume: " + String(TEST_VOLUME_LITERS) + " L");
    Serial.println();
    
    pinMode(FLOW_SENSOR_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), 
                    pulseCounter, RISING);
    
    Serial.println("Ready to start calibration!");
    Serial.println("Instructions:");
    Serial.println("1. Prepare exactly " + String(TEST_VOLUME_LITERS) + "L of water");
    Serial.println("2. Press any key to start test");
    Serial.println("3. Flow water through sensor");
    Serial.println("4. Results will be displayed when test completes");
    Serial.println();
    Serial.println("Press any key to start...");
    
    while (!Serial.available()) {
        delay(100);
    }
    Serial.read();  // Clear buffer
    
    Serial.println();
    Serial.println("Starting calibration test...");
    Serial.println("Flow " + String(TEST_VOLUME_LITERS) + " L of water now!");
    Serial.println();
    
    testRunning = true;
    testStartTime = millis();
    totalPulses = 0;
}

void loop() {
    if (testRunning) {
        // Calculate estimated volume with current calibration (7.5)
        float estimatedVolume = totalPulses / 7.5;
        
        // Print progress
        Serial.print("Pulses: " + String(totalPulses));
        Serial.print(" | Time: " + String((millis() - testStartTime) / 1000) + "s");
        Serial.print(" | Estimated: " + String(estimatedVolume, 2) + " L");
        Serial.println();
        
        // Stop test when volume reached (with margin)
        if (estimatedVolume >= TEST_VOLUME_LITERS * 0.95) {
            testRunning = false;
            
            // Calculate calibration factor
            float calibrationFactor = totalPulses / TEST_VOLUME_LITERS;
            float error = ((estimatedVolume - TEST_VOLUME_LITERS) / TEST_VOLUME_LITERS) * 100.0;
            
            Serial.println();
            Serial.println("========================================");
            Serial.println("Calibration Results");
            Serial.println("========================================");
            Serial.println("Test Volume: " + String(TEST_VOLUME_LITERS) + " L");
            Serial.println("Total Pulses: " + String(totalPulses));
            Serial.println("Measured Volume: " + String(estimatedVolume, 2) + " L");
            Serial.println();
            Serial.println("Calibration Factor: " + String(calibrationFactor, 2) + " pulses/L");
            Serial.println("Error: " + String(error, 2) + "%");
            Serial.println();
            Serial.println("========================================");
            Serial.println("Update CALIBRATION_FACTOR in config.h:");
            Serial.println("#define CALIBRATION_FACTOR " + String(calibrationFactor, 2));
            Serial.println("========================================");
            Serial.println();
            
            delay(10000);  // Wait before restarting
        }
        
        delay(500);
    }
}

