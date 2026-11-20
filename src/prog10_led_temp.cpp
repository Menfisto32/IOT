#include <Arduino.h>
#include <WiFi.h>
#include <FastLED.h>

#define RGB_PIN  38
#define NUM_LEDS 1

const char* ssid = "IME_MREZE";
const char* password = "LOZINKA";

CRGB leds[NUM_LEDS];

void setLed(CRGB boja) {
  leds[0] = boja;
  FastLED.show();
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  FastLED.addLeds<WS2812B, RGB_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(30);
  setLed(CRGB::Blue);             

  Serial.printf("Spajam se na %s", ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  WiFi.setSleep(false);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Spojeno! IP adresa: ");
  Serial.println(WiFi.localIP());
  setLed(CRGB::Green);            
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    setLed(CRGB::Red);    
    Serial.println("Veza prekinuta, ESP32 se sam automatski ponovno spaja...");
  } else {
    setLed(CRGB::Green);             
  }
  
  delay(2000);
}