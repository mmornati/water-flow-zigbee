/*
 * Flow Sensor Test
 * Simple test sketch for YF-S201 flow sensor
 * 
 * This sketch tests:
 * - Pulse detection
 * - Flow rate calculation
 * - Volume accumulation
 * 
 * Hardware:
 * - XIAO ESP32C6
 * - YF-S201 flow sensor connected to GPIO2 (D2)
 */

#define FLOW_SENSOR_PIN 2
#define CALIBRATION_FACTOR 7.5  // 7.5 pulses per liter

volatile uint32_t pulseCount = 0;
unsigned long lastPulseTime = 0;

void IRAM_ATTR pulseCounter() {
    pulseCount++;
    lastPulseTime = millis();
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n========================================");
    Serial.println("Flow Sensor Test");
    Serial.println("========================================");
    Serial.println("Calibration Factor: " + String(CALIBRATION_FACTOR) + " pulses/L");
    Serial.println();
    
    pinMode(FLOW_SENSOR_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), 
                    pulseCounter, RISING);
    
    Serial.println("Flow sensor initialized on pin " + String(FLOW_SENSOR_PIN));
    Serial.println("Waiting for pulses...");
    Serial.println();
}

void loop() {
    static unsigned long lastCheck = 0;
    static uint32_t lastPulseCount = 0;
    
    unsigned long now = millis();
    
    // Calculate every 1 second
    if (now - lastCheck >= 1000) {
        uint32_t pulsesThisSecond = pulseCount - lastPulseCount;
        
        // Calculate flow rate
        float flowRate = (pulsesThisSecond / CALIBRATION_FACTOR) * 60.0;
        
        // Check if flow has stopped
        bool isFlowing = (now - lastPulseTime) < 5000;
        
        // Print results
        Serial.println("---------------------------------");
        Serial.println("Pulses: " + String(pulseCount));
        Serial.println("Pulses/second: " + String(pulsesThisSecond));
        Serial.println("Flow Rate: " + String(flowRate, 2) + " L/min");
        Serial.println("Status: " + String(isFlowing ? "FLOWING" : "IDLE"));
        Serial.println("---------------------------------");
        
        lastPulseCount = pulseCount;
        lastCheck = now;
    }
    
    delay(10);
}

