#include <Wire.h>
#include <Zumo32U4.h>

const uint8_t sensorThreshold = 4;

Zumo32U4OLED display;
Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;
Zumo32U4ProximitySensors proxSensors;

unsigned long currentMillis;
unsigned long previousMillis;
const unsigned long interval = 2;
int proximitySensor = A1;

void proximity(){
    proxSensors.read();
    uint8_t leftValue = proxSensors.countsFrontWithLeftLeds();
    uint8_t rightValue = proxSensors.countsFrontWithRightLeds();
    for(int i = 0; i <= 100; i++){
        motors.setSpeeds(i,i);
    }
   
    bool objectSeen = leftValue >= sensorThreshold || rightValue >= sensorThreshold;
    if(objectSeen){
        if(leftValue < rightValue){
            svingHoyre();
        }else if(rightValue < leftValue){
            svingVenstre();
        }else{
            motors.setSpeeds(0,0);
        }
    }
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
    proxSensors.read();
    frem();
}

void frem(){
    for(int speed= 0; speed <= 200; speed++){
        motors.setLeftSpeed(speed);
        motors.setRightSpeed(speed);
    }
    delay(1000);
    
    for(int speed = 200; speed >= 0; speed--){
        motors.setLeftSpeed(speed);
        motors.setRightSpeed(speed);
    }
    delay(1000);

}

void bak(){
    for(int speed= 0; speed <= 200; speed++){
        motors.setLeftSpeed(-speed);
        motors.setRightSpeed(-speed);
    }
    delay(1000);
    
    for(int speed = 200; speed >= 0; speed--){
        motors.setLeftSpeed(-speed);
        motors.setRightSpeed(-speed);
    }
    delay(1000);

}

void svingHoyre(){
    for(int speed = 0; speed <= 100; speed++){//178, er 90 grader ish
        motors.setSpeeds(speed, -speed);
        delay(2);
    }
    for(int speed = 100; speed >= 0 ; speed--){ //178, er 90 grader ish
        motors.setSpeeds(speed, -speed);
        delay(2);
    }
}

void svingVenstre(){
    for(int speed = 0; speed <= 100; speed++){//178, er 90 grader ish
        motors.setSpeeds(-speed, speed);
        delay(2);
    }
    for(int speed =100; speed >= 0 ; speed--){ //178, er 90 grader ish
        motors.setSpeeds(-speed, speed);
        delay(2);
    }
}

void snu(){
        for(int speed = 0; speed <= 180; speed++){
        motors.setLeftSpeed(speed);
        motors.setRightSpeed(-speed);
        delay(4);
    }
    for(int speed = 180; speed >= 0; speed--){
        motors.setLeftSpeed(speed);
        motors.setRightSpeed(-speed);
        delay(4);
    }

}