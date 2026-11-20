#include <Arduino.h>
#include <FastLED.h>

#define RGB_PIN 38
#define NUM_LEDS 1
CRGB leds[NUM_LEDS];

void rgbLedTask(void *parameter) {
  int state = 0;
  for (;;) {
    state = (state + 1) % 3;
    if (state == 0)      leds[0] = CRGB::Magenta;
    else if (state == 1) leds[0] = CRGB::Yellow;
    else                 leds[0] = CRGB::Cyan;

    FastLED.show();
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<WS2812B, RGB_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(30);

  xTaskCreate(rgbLedTask, "RGBLedTask", 4096, NULL, 1, NULL);
  Serial.println("FreeRTOS task for RGB LED created successfully.");
}

void loop() {
  vTaskDelay(pdMS_TO_TICKS(5000));
}