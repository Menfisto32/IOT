#include <Arduino.h>
#include <FastLED.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define RGB_PIN      38
#define NUM_LEDS     1
#define ONE_WIRE_BUS 21

CRGB leds[NUM_LEDS];
CRGB lastColor = CRGB::White;
bool ledOn = false;

OneWire oneWire(ONE_WIRE_BUS);     
DallasTemperature sensors(&oneWire);

void setLed(CRGB c) {
  leds[0] = c;
  FastLED.show();
}

void setColor(CRGB c, const char* name) {
  lastColor = c;
  ledOn = true;
  setLed(c);
  Serial.printf("LED: %s\n", name);
}

void tempTask(void *parameter) {
  sensors.setWaitForConversion(false);
  for (;;) {
    sensors.requestTemperatures();      
    vTaskDelay(pdMS_TO_TICKS(750));    
    float t = sensors.getTempCByIndex(0);
    if (t != DEVICE_DISCONNECTED_C) {
      Serial.printf("Temperature: %.2f C\n", t);
    } else {
      Serial.println("Error: Could not read temperature data.");
    }
    vTaskDelay(pdMS_TO_TICKS(1250));     
  }
}

void setup() {
  Serial.begin(115200);
  delay(1500);
  FastLED.addLeds<WS2812B, RGB_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(30);
  setLed(CRGB::Black);

  sensors.begin();
  xTaskCreate(tempTask, "TempTask", 4096, NULL, 1, NULL);

  Serial.println("Spremno. Komande: on, off, red, green, blue, white, rgb R G B, bright N");
}

void loop() {
  if (!Serial.available()) { vTaskDelay(pdMS_TO_TICKS(10)); return; }

  String cmd = Serial.readStringUntil('\n');
  cmd.trim();
  cmd.toLowerCase();

  if (cmd == "on")          { ledOn = true;  setLed(lastColor);   Serial.println("LED: ON"); }
  else if (cmd == "off")    { ledOn = false; setLed(CRGB::Black); Serial.println("LED: OFF"); }
  else if (cmd == "red")    setColor(CRGB::Red,   "RED");
  else if (cmd == "green")  setColor(CRGB::Green, "GREEN");
  else if (cmd == "blue")   setColor(CRGB::Blue,  "BLUE");
  else if (cmd == "white")  setColor(CRGB::White, "WHITE");
  else if (cmd.startsWith("rgb ")) {
    int r, g, b;
    if (sscanf(cmd.c_str(), "rgb %d %d %d", &r, &g, &b) == 3) {
      setColor(CRGB(constrain(r, 0, 255), constrain(g, 0, 255), constrain(b, 0, 255)), "RGB");
    } else {
      Serial.println("Format: rgb R G B  (0-255)");
    }
  }
  else if (cmd.startsWith("bright ")) {
    FastLED.setBrightness(constrain(cmd.substring(7).toInt(), 0, 255));
    if (ledOn) setLed(lastColor);
    Serial.println("Svetlina promenjena");
  }
  else if (cmd.length() > 0) {
    Serial.println("Nepoznata komanda");
  }
}