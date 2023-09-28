# 1 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\NeoPixel\\NeoPixel\\NeoPixel.ino"
# 2 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\NeoPixel\\NeoPixel\\NeoPixel.ino" 2




Adafruit_NeoPixel pixels = Adafruit_NeoPixel(32, 6, ((1 << 6) | (1 << 4) | (0 << 2) | (2)) /*|< Transmit as G,R,B*/ + 0x0000 /*|< 800 KHz data transmission*/);
int potensiometer = A5;
int delayval = 500;
int potValue = 0;



CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(9600);
  pinMode(potensiometer, 0x0);
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
      leds[i] = CHSV(i - (j * 2), 255 /* Control the saturation of your leds */, 255 /* Control the brightness of your leds */); /* The higher the value 4 the less fade there is and vice versa */
    }
    FastLED.show();
    delay(25); /* Change this to your hearts desire, the lower the value the faster your colors move (and vice versa) */
  }
    }
