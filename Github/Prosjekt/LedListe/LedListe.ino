#include <Adafruit_NeoPixel.h>

#define PIN 6 // input pin Neopixel is attached to

#define NUMPIXELS      32 // number of neopixels in strip

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int sensorVerdi = 0;
int long ranDelay = 0;
float realTime;
int knapp = 2;


void setup(){
    pinMode(knapp, INPUT_PULLUP);
    pixels.begin();
    Serial.begin(9600);
}

void farger(){
    
}

void trykket(){
    int trykket = digitalRead(knapp);
    Serial.println(trykket);
    if (trykket == 1){
        for (int i = 0; i < 32; i ++){
        pixels.setPixelColor(i, pixels.Color(150, 150, 0));
        delay(100);
        pixels.setPixelColor(i-1, pixels.Color(0, 0, 0));
        if(i=31){
            i = 0;
        }
        }
    }


}
    
void loop(){
    farger();
    pixels.show();
    trykket();
}
