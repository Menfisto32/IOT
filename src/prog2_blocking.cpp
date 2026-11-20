#include <Arduino.h>
#include <FastLED.h>

#define RGB_PIN 38
#define NUM_LEDS 1
CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<WS2812B, RGB_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(30);
  Serial.println("RGB LED initialized in blocking mode.");
}

void loop() {
  leds[0] = CRGB::Red;   FastLED.show(); delay(1000);
  leds[0] = CRGB::Green; FastLED.show(); delay(1000);
  leds[0] = CRGB::Blue;  FastLED.show(); delay(1000);
}