#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4Encoders encoders;
Zumo32U4Buzzer buzzer;
Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;
Zumo32U4ButtonC buttonC;

//Variables for Speedometer and Meassure distance
long MeassureDistance = 0;

Zumo32U4OLED display;

void setup()
{
Serial.begin(9600);
}


void loop()
{
  SpeedometerAndMeassureDistance();
if (buttonA.isPressed())
  {
    motors.setSpeeds(200, 200);
  }
else if (buttonC.isPressed())
  {
    motors.setSpeeds(-400, -400);
  }
else
  {
    motors.setSpeeds(0, 0);
  }
}


void SpeedometerAndMeassureDistance(){
  static uint8_t lastDisplayTime;
    if ((uint8_t)(millis() - lastDisplayTime) >= 200)
    {
        long countsLeft = encoders.getCountsAndResetLeft();
        long countsRight = encoders.getCountsAndResetRight();
        float avrage = (countsLeft+countsRight)/2;
        float distance = 75.81*12;
        float oneRound = 12.25221135;
        
        float meters = avrage/distance*oneRound*5;
        MeassureDistance +=abs(meters)*0.2;
        lastDisplayTime = millis();

      } // end if
      Serial.println(MeassureDistance);
}