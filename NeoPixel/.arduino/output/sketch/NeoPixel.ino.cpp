#include <Arduino.h>
#line 1 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\NeoPixel\\NeoPixel\\NeoPixel.ino"
#include <Adafruit_NeoPixel.h>

#define PIN            6
#define NUMPIXELS      32

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int potensiometer = A5;
int delayval = 500;
int potValue = 0;
#define BRIGHTNESS 255   /* Control the brightness of your leds */
#define SATURATION 255   /* Control the saturation of your leds */

CRGB leds[NUM_LEDS];

#line 15 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\NeoPixel\\NeoPixel\\NeoPixel.ino"
void setup();
#line 21 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\NeoPixel\\NeoPixel\\NeoPixel.ino"
void hastighet();
#line 28 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\NeoPixel\\NeoPixel\\NeoPixel.ino"
void loop();
#line 15 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\NeoPixel\\NeoPixel\\NeoPixel.ino"
void setup() {
  Serial.begin(9600);
  pinMode(potensiometer, INPUT);
  pixels.begin(); // This initializes the NeoPixel library.
}

void hastighet(){
    potValue = analogRead(potensiometer);
    Serial.print(potValue);
    delay(delayval);
    FastLED.addLeds<LED_TYPE, DATA_PIN>(leds, NUM_LEDS);
}

void loop() {
 for (int j = 0; j < 255; j++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(i - (j * 2), SATURATION, BRIGHTNESS); /* The higher the value 4 the less fade there is and vice versa */ 
    }
    FastLED.show();
    delay(25); /* Change this to your hearts desire, the lower the value the faster your colors move (and vice versa) */
  }
    }
