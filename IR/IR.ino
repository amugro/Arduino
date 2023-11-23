#include <IRremote.h>
#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4Motors motors;

#define code1 3910598400
#define code2 3860463360
#define code3 4061003520

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




  
