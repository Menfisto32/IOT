#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 21

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void asyncTemperatureTask(void *parameter) {
  sensors.setWaitForConversion(false);
  const TickType_t conversionDelay = pdMS_TO_TICKS(750);
  const TickType_t cycleDelay = pdMS_TO_TICKS(2250);  

  for (;;) {
    Serial.println("[TASK] Requesting hardware ADC ...");
    sensors.requestTemperatures();     
    vTaskDelay(conversionDelay);       

    float tempC = sensors.getTempCByIndex(0);
    if (tempC != DEVICE_DISCONNECTED_C) {
      Serial.printf("[TASK] Temp.: %.2f C\n", tempC);
    } else {
      Serial.println("[ALARM] Sensor comm breakdown!");
    }
    vTaskDelay(cycleDelay);
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  sensors.begin();
  Serial.println("FreeRTOS Async Sensor Mode ENABLED.");

  xTaskCreate(asyncTemperatureTask, "AsyncTempTask", 4096, NULL, 1, NULL);
}

void loop() {
  vTaskDelete(NULL);
}