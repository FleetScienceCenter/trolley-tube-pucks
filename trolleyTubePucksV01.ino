#include <FastLED.h>
#include <Adafruit_VCNL4010.h>

#define DATA_PIN 41
#define NUM_LEDS 30
CRGB leds[NUM_LEDS];

Adafruit_VCNL4010 vcnl;

float baseline;
float proxVal;
bool hit = false;
int rVal = 255;
int gVal = 255;
int bVal = 255;

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  if (!vcnl.begin()){
    Serial.println("Sensor not found");
    while (1);
  }
  Serial.println("Found VCNL4010");
  baseline = vcnl.readProximity();
  FastLED.showColor(CRGB(255, 0, 0));
}

void showHit() {
  gVal = 255;
  bVal = 255;
  FastLED.showColor(CRGB(255, gVal, bVal));
  delay(500);
  while(gVal > 0 && bVal > 0){
    gVal--;
    bVal--;
    FastLED.showColor(CRGB(255, gVal, bVal));
  }
  FastLED.showColor(CRGB(255, 0, 0));
  hit = false;
  Serial.println("hit");
}

void loop() {
  proxVal = vcnl.readProximity();
  if (!hit && proxVal > (baseline + 600)){
    hit = true;
    showHit();
  }
  Serial.println(proxVal);
}
