#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  delay(1500);
  Serial.println("PMF NIS - IoT PRAKTIKUM: SERIAL COMM");

  if (psramInit()) {
    Serial.println("PSRAM Status: INITIALIZED SUCCESSFULLY");
    Serial.printf("Total PSRAM: %d Bytes\n", ESP.getPsramSize());
  } else {
    Serial.println("PSRAM Status: FAILED OR NOT PRESENT");
  }
}

void loop() {
  Serial.printf("[UPTIME] Free Heap: %d Bytes | Free PSRAM: %d Bytes\n",
                ESP.getFreeHeap(), ESP.getFreePsram());
  delay(2000);
}