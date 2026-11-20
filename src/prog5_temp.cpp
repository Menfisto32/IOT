#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 21

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Initializing DS18B20 sensor...");
  sensors.begin();
}

void loop() {
  Serial.print("Requesting temperature conversion... ");
  unsigned long startTime = millis();
  sensors.requestTemperatures();
  unsigned long conversionTime = millis() - startTime;
  Serial.println("DONE");

  float temperatureC = sensors.getTempCByIndex(0);

  if (temperatureC != DEVICE_DISCONNECTED_C) {
    Serial.printf("Temperature: %.2f C\n", temperatureC);
    Serial.printf("Conversion time: %lu ms\n", conversionTime);
  } else {
    Serial.println("Error: Could not read temperature data.");
  }
  Serial.println("-----------------------------------");
  delay(2000);
}
