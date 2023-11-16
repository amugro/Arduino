#include <Wire.h>
#include <Zumo32U4.h>

// Variables for softwareBattery
long lastDistance = 0;
int8_t consumptionMeasure = 0;
int8_t timesCharged = 0;
int8_t batteryHealth = 2;

// Variables for showBatteryStatus()
unsigned long previousMillis = 0;
const long oninterval = 10000;
const long offinterval = 2000;
long displayTime = 0;
bool batteryDisplayed = false;


long batteryLevel = 123456789;
int speedReading = 32;
int distance = 11320;

Zumo32U4OLED display;
Zumo32U4ButtonA buttonA;

void setup(){
    // Wait for button A to be pressed and released.
    display.clear();
    display.print(F("Press A"));
    display.gotoXY(0, 1);
    display.print(F("to start"));
    buttonA.waitForButton();
    
}

void loop(){
    showBatteryStatus();
}

void showBatteryStatus(){
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= oninterval){
        display.clear();
        display.setLayout21x8();
        display.print(F("Battery level"));
        display.gotoXY(15,0);
        display.print(batteryLevel);
        display.gotoXY(0,2);
        display.print(F("Times Charged"));
        display.gotoXY(15,2);
        display.print(timesCharged);
        display.gotoXY(0,4);
        display.print(F("Battery Health"));
        display.gotoXY(15,4);
        display.print(batteryHealth);
        previousMillis = currentMillis;
        displayTime = currentMillis;
        batteryDisplayed = true;
    } // end if

    if ((currentMillis - displayTime >= offinterval) && (batteryDisplayed == true)){
        display.clear();
        display.setLayout11x4();
        display.print(F("Speed:"));
        display.gotoXY(0,1);
        display.print(speedReading);
        display.gotoXY(7,1);
        display.print(F("m/s"));
        display.gotoXY(0,2);
        display.print(F("Distance:"));
        display.gotoXY(0,3);
        display.print(distance);
        display.gotoXY(7,3);
        display.print(F("m"));
        batteryDisplayed = false;
    } // end if
}