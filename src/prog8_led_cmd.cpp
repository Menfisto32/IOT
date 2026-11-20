#include <Arduino.h>
#include <FastLED.h>

#define RGB_PIN  38
#define NUM_LEDS 1

CRGB leds[NUM_LEDS];
CRGB lastColor = CRGB::White;
bool ledOn = false;

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

void setup() {
  Serial.begin(115200);
  delay(1500);
  FastLED.addLeds<WS2812B, RGB_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(30);
  setLed(CRGB::Black);
  Serial.println("LED kontrola spremna. Komande: on, off, red, green, blue, white,");
  Serial.println("yellow, cyan, magenta, orange, rgb R G B, hue H, bright N");
}

void loop() {
  if (!Serial.available()) return;

  String cmd = Serial.readStringUntil('\n');
  cmd.trim();
  cmd.toLowerCase();

  if (cmd == "on")           { ledOn = true;  setLed(lastColor);   Serial.println("LED: ON"); }
  else if (cmd == "off")     { ledOn = false; setLed(CRGB::Black); Serial.println("LED: OFF"); }
  else if (cmd == "red")     setColor(CRGB::Red,     "RED");
  else if (cmd == "green")   setColor(CRGB::Green,   "GREEN");
  else if (cmd == "blue")    setColor(CRGB::Blue,    "BLUE");
  else if (cmd == "white")   setColor(CRGB::White,   "WHITE");
  else if (cmd == "yellow")  setColor(CRGB::Yellow,  "YELLOW");
  else if (cmd == "cyan")    setColor(CRGB::Cyan,    "CYAN");
  else if (cmd == "magenta") setColor(CRGB::Magenta, "MAGENTA");
  else if (cmd == "orange")  setColor(CRGB::Orange,  "ORANGE");
  else if (cmd.startsWith("rgb ")) {
    int r, g, b;
    if (sscanf(cmd.c_str(), "rgb %d %d %d", &r, &g, &b) == 3) {
      setColor(CRGB(constrain(r, 0, 255), constrain(g, 0, 255), constrain(b, 0, 255)), "RGB");
    } else {
      Serial.println("Format: rgb R G B  (0-255)");
    }
  }
  else if (cmd.startsWith("hue ")) {
    int h = constrain(cmd.substring(4).toInt(), 0, 255);
    setColor(CHSV(h, 255, 255), "HUE");
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