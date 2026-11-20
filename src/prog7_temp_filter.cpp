#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 21

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float lastPrinted = -1000.0;   // nemoguca vrednost, pa se prvo merenje uvek ispise
const float THRESHOLD = 0.5;

void setup() {
  Serial.begin(115200);
  delay(1000);
  sensors.begin();

#ifdef ENABLE_SENSOR_LOG
  Serial.println("[DIAGNOSTIC] Sensor log enabled");
#endif
}

void loop() {
  sensors.requestTemperatures();
  float t = sensors.getTempCByIndex(0);

  if (t == DEVICE_DISCONNECTED_C) {
    Serial.println("Error: Could not read temperature data.");
  } else {
#ifdef ENABLE_SENSOR_LOG
    Serial.printf("[LOG] raw reading: %.2f C\n", t);
#endif
    if (fabs(t - lastPrinted) > THRESHOLD) {
      Serial.printf("Temperature: %.2f C\n", t);
      lastPrinted = t;
    }
  }
  delay(2000);
}