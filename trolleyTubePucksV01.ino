#include <FastLED.h>
#include <Adafruit_VCNL4010.h>
#include <SD.h>
#include <Audio.h>

#define DATA_PIN 41
#define NUM_LEDS 30
CRGB leds[NUM_LEDS];

Adafruit_VCNL4010 vcnl;

AudioPlaySdWav playWav1;
AudioOutputI2S audioOutput;

AudioConnection patchCord1(playWav1, 0, audioOutput, 0);
AudioConnection patchCord2(playWav1, 1, audioOutput, 1);
AudioControlSGTL5000 sgtl5000_1;

#define SDCARD_CS_PIN    BUILTIN_SDCARD
#define SDCARD_MOSI_PIN  11
#define SDCARD_SCK_PIN   13

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

  AudioMemory(8);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    // stop here, but print a message repetitively
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
}

void showHit() {
  gVal = 255;
  bVal = 255;
  playWav1.play("F#4.WAV");
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
