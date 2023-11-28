#include <IRremote.h>
#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4Motors motors;
Zumo32U4Encoders encoders;

#define code1 3910598400
#define code2 3860463360
#define code3 4061003520

//Variables for Speedometer and Meassure distance
long MeassureDistance = 0;
float meters;
Zumo32U4OLED display;

const long RECV_PIN = A4;
IRrecv irrecv(RECV_PIN);
unsigned long nowTime;
unsigned long considerTime;
unsigned long irNum;

void setup(){
	Serial.begin(9600);
	IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);

}

void loop(){
	SpeedometerAndMeassureDistance();
	nowTime = millis();
	if(IrReceiver.decode()){
	unsigned long irNum = IrReceiver.decodedIRData.decodedRawData;  
	Serial.println(irNum);
	switch (irNum)
	{
	case code1:
		motors.setSpeeds(200,200);
		break;

	case code2:
		motors.setSpeeds(0,0);
	break;

	case code3:
	Serial.println("3");
	break;
	
	default:
		break;
	}
	}
IrReceiver.resume();
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
		display.clear();
		display.gotoXY(0,0);
		display.print(meters);
		display.gotoXY(0,1);
		display.print(MeassureDistance);

      } // end if
}




  
