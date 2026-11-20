#include <Arduino.h>
#include <FastLED.h>

#define RGB_PIN 38
#define NUM_LEDS 1
CRGB leds[NUM_LEDS];

unsigned long previousMillis = 0;
const unsigned long interval = 1000;
int colorState = 0;

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<WS2812B, RGB_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(30);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    colorState = (colorState + 1) % 3;

    if (colorState == 0)      { leds[0] = CRGB::Red;   Serial.println("LED Color: RED"); }
    else if (colorState == 1) { leds[0] = CRGB::Green; Serial.println("LED Color: GREEN"); }
    else                      { leds[0] = CRGB::Blue;  Serial.println("LED Color: BLUE"); }

    FastLED.show();
  }
}