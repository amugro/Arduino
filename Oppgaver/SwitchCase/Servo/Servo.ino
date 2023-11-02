#include <Servo.h>

const int servoPin = 9;
const int servoPin2 = 10;
const int sensor1 = A1;
const int sensor2 = A0;
const int sensor3 = A2;
int potVerdi = 0;
int servoOutput = 0;
int sensorVerdi1 = 0;
int sensorVerdi2 = 0;
int sensorVerdi3 = 0;
int i = 90;
int x = 90;


const int interval = 100;
int previousMillis;


Servo servo;
Servo servo2;


void setup(){
    Serial.begin(9600);
    servo.attach(servoPin);
    servo2.attach(servoPin2);
    pinMode(sensor1, INPUT);
    pinMode(sensor2, INPUT);
    pinMode(sensor3, INPUT);
}

void potensiometer(){
    int currentMillis = millis();
    if (currentMillis-previousMillis >= interval){
        previousMillis = currentMillis;
        sensorVerdi1 = analogRead(sensor1);
        sensorVerdi2 = analogRead(sensor2);
        sensorVerdi3 = analogRead(sensor3);
            if(sensorVerdi1>sensorVerdi2){
                i = i+3;
                servo.write(i);
            }else{
                i = i-3;
                servo.write(i);
            }
            if(sensorVerdi3>sensorVerdi1){
                x = x+3;
                servo2.write(x);
            }else{
                x = x-3;
                servo2.write(x);
            }
            

        
   
      /*  if(reading[readindex]-reading[readindex - 1]){
            i= i -1;
            servo.write(i);
            Serial.print(i);
        }else{
            i= i+1;
            servo.write(i);
            Serial.print(i);
        }
        if(i==0|| i==180){
            i = 90;
        }
        servo.write(i);
        if (readindex >= numReadings){
            readindex = 0;
        }*/
    }

}

void fremOgTilbake(){
    servoOutput = map(potVerdi,0,1023,0,180);
    servo.write(servoOutput);
}

void loop(){
    potensiometer();
    //fremOgTilbake();
}