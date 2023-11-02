#include <Arduino.h>
#include <SPI.h>
#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 10, /* data=*/ 9, /* cs=*/ 6, /* dc=*/ 7, /* reset=*/ 8);

const int potensiometer = A3;
int potVerdi;
const int sensorpin = A5;
int sensorverdi;

const int knapp = 5;
int knappVerdi = 0;
int buttonState = LOW;
long lastDebounceTime = 0;
long debounceDelay = 200;
int mode = 0;

unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 1000;

void setup(){
    Serial.begin(9600);
    pinMode(sensorpin, INPUT);
    pinMode(knapp, INPUT);
    pinMode(potensiometer, INPUT);
    startMillis = millis();
    u8g2.begin();
    }

void button(){
    int reading = digitalRead(knapp);
    if (reading != buttonState){
        if(reading == HIGH){
            mode++;
            Serial.print(mode);
            if (mode == 3){
                mode = 0;
            }
        }
    }
    buttonState = reading;
}

void potMeter(){
    potVerdi = analogRead(potensiometer);
    currentMillis = millis();
    if(currentMillis - startMillis >= period){
        Serial.println(potVerdi);
        startMillis = currentMillis;
    u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.setCursor(15,15);
    u8g2.print("Pot");
    u8g2.setCursor(15,50);
    u8g2.print(potVerdi);
  } while ( u8g2.nextPage() );
    }
}

void sensor(){
    sensorverdi = analogRead(sensorpin);
    currentMillis = millis();
    if (currentMillis - startMillis >= period){
        Serial.println(sensorverdi);
        startMillis = currentMillis;
        u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.setCursor(15,15);
    u8g2.print("Lys");
    u8g2.setCursor(15,50);
    u8g2.print(sensorverdi);
  } while ( u8g2.nextPage() );

    }
}

void loop(){
    button();
    if(mode == 0){
        sensor();
    }else if (mode == 1){
        potMeter();
    }else if (mode == 2){
        u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.setCursor(15,15);
    u8g2.print("Amund");
  } while ( u8g2.nextPage() );

    }
}
