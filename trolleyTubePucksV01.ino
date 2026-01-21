#include <FastLED.h>
#include <Adafruit_VCNL4010.h>

#define DATA_PIN 20
#define NUM_LEDS 30
CRGB leds[NUM_LEDS];

Adafruit_VCNL4010 vcnl;

float baseline;
float proxVal;
bool hit = false;

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  if (!vcnl.begin()){
    Serial.println("Sensor not found");
    while (1);
  }
  Serial.println("Found VCNL4010");
  baseline = vcnl.readProximity();
}

void showHit() {
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i].setRGB(255, 100, 0);
  }
  FastLED.show();
  delay(500);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(0, 0, 0);
  }
  FastLED.show();
  delay(500);
  hit = false;
}

void loop() {
  proxVal = vcnl.readProximity();
  if (!hit && proxVal > (baseline + 600)){
    hit = true;
    showHit();
  }
  Serial.println(proxVal);
  
  //Serial.println(hit);
}
