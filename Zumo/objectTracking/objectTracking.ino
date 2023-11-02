#include <Wire.h>
#include <Zumo32U4.h>

const uint8_t sensorThreshold = 5;

Zumo32U4OLED display;
Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;
Zumo32U4ProximitySensors proxSensors;

unsigned long currentMillis;
unsigned long previousMillis;
const unsigned long interval = 3;
int proximitySensor = A1;

void proximity(){
    proxSensors.read();
    uint8_t leftValue = proxSensors.countsFrontWithLeftLeds();
    uint8_t rightValue = proxSensors.countsFrontWithRightLeds();

   
    bool objectSeen = leftValue >= sensorThreshold || rightValue >= sensorThreshold;
    if(objectSeen){
        delay(100);
        if(leftValue < rightValue){
            motors.setSpeeds(100,-100);
        }else if(rightValue < leftValue){
            motors.setSpeeds(-100,100);
        }else if(rightValue == leftValue){
            motors.setSpeeds(0,0);
        }
    }else{
        motors.setSpeeds(-100,100);
        }

}

void read(){
    proxSensors.read();
    uint8_t leftValue = proxSensors.countsFrontWithLeftLeds();
    uint8_t rightValue = proxSensors.countsFrontWithRightLeds();

    display.gotoXY(0,0);
    display.print(leftValue);
    display.print(' ');
    display.gotoXY(0,1);
    display.print(rightValue);
}

void setup()
{
    proxSensors.initFrontSensor();
    buttonA.waitForButton();
    delay(1000);
}

void loop(){
    read();
    proximity();
}


