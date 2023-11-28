#include <Wire.h>
#include <Zumo32U4.h>
#include <IRremote.h>

/////////// NOTES ////////////
/*
- Add switchcase for display modes/ buzzer                              | DONE
- Add switchcase in softwareBattery for special functions               |
- Add lineFollower                                                      | DONE
- Add switchcase in line follower for turning, job etc.                 |
- Add Random based taxi job                                             | DONE (might need adjustment in payment calculation and random(LOW,HIGH))
- Fix speed and distance calculation. Use encoders.getCountsAndReset    |
- Add function to activate / deactivate hiddenfeature                   | DONE
- Add function to activate emergency charging                           | Skal vi bare aktivere den med tastetrykk på fjernkontrollen?
*/

Zumo32U4OLED display;
Zumo32U4Encoders encoders;
Zumo32U4Buzzer buzzer;
Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
Zumo32U4ButtonC buttonC;
Zumo32U4LineSensors lineSensors;
Zumo32U4IMU imu;

// Variables for softwareBattery
int8_t batteryLevel = 100;
long lastDistance = 0;
float consumptionMeasure = 0;
int8_t timesCharged = 0;
unsigned long batteryMillis = 0;

//variables for IR remote
#define code1 3910598400
#define code2 3860463360
#define code3 4061003520

const long RECV_PIN = A4;
IRrecv irrecv(RECV_PIN);
unsigned long nowTime;
unsigned long considerTime;
unsigned long irNum;
/////////////////////////////

// Variables for hiddenFeature()
bool hiddenActivated = false;
bool emergencyChargingUsed = false;
bool emergencyChargeMode = false;
bool countDownStarted = false;
unsigned long countDownStart = 0;
const long countDownInterval = 15000;

// Variables for showBatteryStatus()
unsigned long previousMillis = 0;
unsigned long refreshPreviousMillis = 0;
long displayTime = 0;
bool batteryDisplayed = false;



// Variables for regneDistanse()
long MeassureDistance = 0;

// Variables for taxiDriver()
int workCase = 0;
bool passengerFound = false;
bool onDuty = false;
unsigned long searchTime = 0;
unsigned long missionStart = 0;
unsigned long missionDistance = 0;
unsigned long startDistance = 0;
unsigned long passengerEnteredMillis = 0;
unsigned long previousWorkRequest = 0;
const long freeTimeInterval = 15000;

// Variables for followLine
unsigned int lineSensorValues[5];
int16_t lastError = 0;
const uint16_t maxSpeed = 200;

// Variables for charging
int missingAmount = 0;
int account = 100;
int debit = 0;


///////// TEST VARIABLES ////
int batteryHealth = 2;
float iAmSpeed = 0;



void setup(){
    Serial.begin(9600);
    lineSensors.initFiveSensors();

    // Wait for button A to be pressed and released.
    display.clear();
    display.print(F("Press A"));
    display.gotoXY(0, 1);
    display.print(F("to start"));
    buttonA.waitForButton();
    calibrateLineSensors();

    IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);

    
} // end setup

void loop(){
    softwareBattery();
    showBatteryStatus();
    SpeedometerAndMeassureDistance();
    followLine();
    taxiDriver();
} // end loop

void IrRemote(){
    nowTime = millis();
	if(IrReceiver.decode()){
	unsigned long irNum = IrReceiver.decodedIRData.decodedRawData; 
    Serial.println(irNum); 

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
        iAmSpeed = meters;
        MeassureDistance +=abs(meters)*0.2;
        lastDisplayTime = millis();

      } // end if
      Serial.println(MeassureDistance);
}// end voud SpeedometerAndMeassureDistance



void softwareBattery(){
    long currentMillis = millis();

    if (currentMillis - batteryMillis > 100){
    batteryMillis = currentMillis;
    consumptionMeasure += (abs(iAmSpeed)/30); // EKSEMPEL PÅ FUNKSJON, OPPDATER NÅR VI TESTER MED DATA
    Serial.print(consumptionMeasure);
    } // end if

    if (consumptionMeasure >= 10){
        batteryLevel -= 1;
        consumptionMeasure = 0;
    } // end if
} // end void

void hiddenFeature(){
    int currentMillis = millis();

    int8_t averageSpeed = iAmSpeed;
    int8_t distanceChange = MeassureDistance - lastDistance;



    // Function to turn on hiddenActivated
    if (imu.g.z > 5000){
        for (int i = 0; i < 10000; i++){
            if (imu.g.z < -5000){
                hiddenActivated = true;
            } // end if
        } // end for
    } // end if

    // Function to turn off hiddenActivated
    if ((hiddenActivated == true) && (countDownStarted == false)){
        countDownStart = currentMillis;
        countDownStarted = true;
    } // end if

    if (currentMillis - countDownStart > countDownInterval){
        hiddenActivated = false;
    } // end if

    // Function to turn on emergencyChargingMode
    


    if (hiddenActivated == true){ // FINN PÅ NOE SOM SKAL AKTIVERE FUNSKJONEN
      
        lastDistance = MeassureDistance;

        consumptionMeasure += (averageSpeed / distanceChange); // EKSEMPEL PÅ FUNKSJON, OPPDATER NÅR VI TESTER MED DATA

        if (consumptionMeasure <= -10){
            if ((emergencyChargeMode == true) && (emergencyChargingUsed = false)){
                batteryLevel += 20;
                emergencyChargingUsed = true;
            } // end if

            else{
                batteryLevel += 2;
            } // end else
        } // end else
    } // end if
} // end void

void showBatteryStatus(){  
    long onInterval;
    long offInterval;
    long refreshInterval;
    uint8_t batteryCase; 
  if((batteryLevel < 10) && (batteryLevel > 5)){
        batteryCase = 0;
    }else if((batteryLevel < 5) && (batteryLevel > 0)){
        batteryCase = 1;
    }else if(batteryLevel == 0){
        batteryCase = 2;
    }


    switch (batteryCase)
    {
    case 0:
        onInterval = 5000;
        offInterval = 2000;
        refreshInterval = 500;
        break;
    case 1:
        onInterval = 2000;
        offInterval = 1000;
        refreshInterval = 500;
        break;
    case 2:
        //batteryCase2(); skal være når batteriet er helt utladet. Her må vi kunne legge inn en hidden feature som
        //gjør at vi kan få litt strøm slik at den kommer seg til ladestasjonen.
        break;
    
    default:
        onInterval = 10000;
        offInterval = 2000;
        refreshInterval = 500;
        break;
    }



    unsigned long currentMillis = millis();



    if (batteryDisplayed == false){
        if (currentMillis - refreshPreviousMillis >= refreshInterval){
            //float speedReading = speedometer();
            display.clear();
            display.setLayout11x4();                        // Divide screen into 11 columns and 4 rows
            display.print(F("Speed:"));
            display.gotoXY(0,1);
            display.print(iAmSpeed);
            display.gotoXY(7,1);
            display.print(F("m/s"));
            display.gotoXY(0,2);
            display.print(F("Distance:"));
            display.gotoXY(0,3);
            display.print(MeassureDistance);
            display.gotoXY(7,3);
            display.print(F("cm"));
            refreshPreviousMillis = currentMillis;
        } // end if
    } // end if


    if (currentMillis - previousMillis >= onInterval){
        display.clear();
        display.setLayout21x8();                        // Divide screen into 21 columns and 8 rows
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
        if(batteryLevel < 10){
            display.gotoXY(0,6);
            display.print("Please recharge!");
        }
        previousMillis = currentMillis;
        displayTime = currentMillis;
        batteryDisplayed = true;                        // To make the next if sentence only run once after this text have been ran
    } // end if

    if ((currentMillis - displayTime >= offInterval) && (batteryDisplayed == true)){
        batteryDisplayed = false;
    } // end if

}//end void showBatteryStatus

void taxiDriver(){
    unsigned long currentMillis = millis();

    if (onDuty == false){
        if (currentMillis - previousWorkRequest > freeTimeInterval){
            motors.setSpeeds(0,0);
            display.clear();
            display.setLayout21x8();
            display.print(F("Want to work?"));
            display.gotoXY(0,3);
            display.print(F("Button A = Yes"));
            display.gotoXY(0,5);
            display.print(F("Button B = No"));
            while ((buttonA.isPressed() == 0) and (buttonB.isPressed() == 0)){
            } // end while
            if (buttonA.isPressed() == 1){
                delay(500);
                workCase = 1;
                onDuty = true;
            } // end if
            else if (buttonB.isPressed() == 1){
                delay(500);
                workCase = 0;
                onDuty = false;
            } // end else if
            previousWorkRequest = currentMillis;
        } // end if
    } // end if
    
    switch (workCase)
    {
    case 1:
        searchForPassenger();
        break;
    case 2:
        drivePassenger();
        break;
    default:
        break;
    } // end case
    
} // end void

void searchForPassenger(){
    unsigned long currentMillis = millis();

    if (passengerFound == false){
        searchTime = random(3000, 8000);
        passengerFound = true;
        missionStart = currentMillis;
    } // end if
    
    if (passengerFound == true){
        if (currentMillis - missionStart >= searchTime){
            passengerFound = false;

            missionDistance = random(200,400);
            motors.setSpeeds(0,0);
            display.clear();
            display.setLayout21x8();
            display.print(F("The passenger want to travel"));
            display.gotoXY(0,1);
            display.print(F("travel for"));
            display.gotoXY(12,1);
            display.print(missionDistance);
            display.gotoXY(18,1);
            display.print(F("cm"));
            display.gotoXY(0,3);
            display.print(F("Do you take the job?"));
            display.gotoXY(0,4);
            display.print(F("A = YES"));
            display.gotoXY(15,4);
            display.print(F("B = NO"));
            display.gotoXY(0,5);
            display.print(F("C = Off duty"));
            while ((buttonA.isPressed() == 0) and (buttonB.isPressed() == 0) and buttonC.isPressed() == 0){
            } // end while
            if (buttonA.isPressed() == 1){
                delay(500);
                startDistance = MeassureDistance;
                passengerEnteredMillis = currentMillis;
                workCase = 2;
            } // end if

            else if (buttonB.isPressed() == 1){
                delay(500);
                workCase = 1;
            } // end if

            else if (buttonC.isPressed() == 1){
                delay(500);
                onDuty = false;
                workCase = 0;
            } // end if     
        } // end if
    } // end if
} // end void

void drivePassenger(){
    unsigned long currentMillis = millis();
    if (MeassureDistance - startDistance >= missionDistance){
        motors.setSpeeds(0,0);
        unsigned long payment = ((missionDistance * 2000) / (currentMillis - passengerEnteredMillis));
        account +=  payment;
        display.clear();
        display.setLayout21x8();
        display.print(F("Passenger delivered"));
        display.gotoXY(0,2);
        display.print(F("Payment:"));
        display.gotoXY(13,2);
        display.print(payment);
        display.gotoXY(18,2);
        display.print(F("kr"));
        display.gotoXY(0,4);
        display.print(F("Continue Working?"));
        display.gotoXY(0,5);
        display.print(F("A = Search for client"));
        display.gotoXY(0,6);
        display.print(F("B = End work"));
        while ((buttonA.isPressed() == 0) and (buttonB.isPressed() == 0)){
        } // end while
        if (buttonA.isPressed() == 1){
            delay(500);
            workCase = 1;
        } // end if
        else if (buttonB.isPressed() == 1){
            delay(500);
            workCase = 0;
            onDuty = false;
        } // end if
    } // end if
} // end void

void followLine(){
    int16_t position = lineSensors.readLine(lineSensorValues);
    /*display.gotoXY(0,0);
    display.print(position);*/

    int16_t error = position - 2000;
    
    int16_t speedifference = error/4 + 6*(error-lastError);

    lastError = error;

    int leftSpeed = (int16_t)maxSpeed + speedifference;
    int rightSpeed = (int16_t)maxSpeed - speedifference;

 

    leftSpeed = constrain(leftSpeed,0,(int16_t)maxSpeed);
    rightSpeed = constrain(rightSpeed,0,(int16_t)maxSpeed);
    motors.setSpeeds(leftSpeed,rightSpeed);
} // end void

void calibrateLineSensors(){
     delay(1000);
  for(uint16_t i = 0; i < 100; i++)
  {
    motors.setSpeeds(-200, 200);
    lineSensors.calibrate();
    }
   
  motors.setSpeeds(0, 0);
  delay(2000);
}

void chargingMode(){
    unsigned long currentMillis = millis();
    if ((debit >= 0) && (account >= debit)){
        account = account - debit;
        debit = 0;
        display.clear();
        display.print("Debit account cleared");
    } // end if

    if ((currentMillis % 10) == 0){                           // Just a idea, test for reliability
    display.clear();
    display.print(F("Charging mode activated"));
    display.gotoXY(0,2);
    display.print(F("A = add 10%"));
    display.gotoXY(0,3);
    display.print(F("B = add 50%"));
    display.gotoXY(0,4);
    display.print(F("C = Fully Charged"));
    display.gotoXY(0,6);
    display.print(F("Bank account = "));
    display.gotoXY(15, 6);
    display.print(account);
    display.gotoXY(0, 7);
    display.print(F("Debit account = "));
    display.gotoXY(15, 7);
    display.print(debit);
    } // end if

    if (buttonA.isPressed() == 1){
        if (account >= 10){
            account -= 10;
            batteryLevel += 10;
            display.clear();
            display.print(F("Wait one second while charging"));
            delay(1000);
        } // end if
        
        else{
            missingAmount = 10 - account;
            display.clear();
            display.setLayout21x8();
            display.print(F("You are missing"));
            display.gotoXY(15, 0);
            display.print(missingAmount);
            display.gotoXY(18, 0);
            display.print(F("kr"));
            display.gotoXY(0, 1);
            display.print(F("Do you want to charge on debit?"));
            display.gotoXY(0,2);
            display.print(F("A = Yes"));
            display.gotoXY(12,2);
            display.print(F("B = Cancel"));

            while ((buttonA.isPressed() == 0) and (buttonB.isPressed() == 0)){
            } // end while
            if (buttonA.isPressed() == 1){
                debit += missingAmount;
                account = 0;
                chargingMode();
            } // end if

            if (buttonB.isPressed() == 1){
                // QUIT CHARGING MODE, Integrate with driving switch case?
            } // end if
        } // end else
    } // end if

    if (buttonB.isPressed() == 1){
        if (account >= 50){
            account -= 50;
            batteryLevel += 50;
            display.clear();
            display.print(F("Wait five second while charging"));
            delay(5000);
        } // end if
        
        else{
            missingAmount = 50 - account;
            display.clear();
            display.setLayout21x8();
            display.print(F("You are missing"));
            display.gotoXY(15, 0);
            display.print(missingAmount);
            display.gotoXY(18, 0);
            display.print(F("kr"));
            display.gotoXY(0, 1);
            display.print(F("Do you want to charge on debit?"));
            display.gotoXY(0,2);
            display.print(F("A = Yes"));
            display.gotoXY(12,2);
            display.print(F("B = Cancel"));

            while ((buttonA.isPressed() == 0) and (buttonB.isPressed() == 0)){
            } // end while
            if (buttonA.isPressed() == 1){
                debit += missingAmount;
                account = 0;
                chargingMode();
            } // end if

            if (buttonB.isPressed() == 1){
                // QUIT CHARGING MODE, Integrate with driving switch case?
            } // end if
        } // end else
    } // end if

    if (buttonC.isPressed() == 1){
        int percentageUntilFull = (100 - batteryLevel);
        if (account >= (percentageUntilFull)){
            account -= percentageUntilFull;
            batteryLevel += percentageUntilFull;
            display.clear();
            display.print(F("Wait ten second while charging"));
            delay(10000);
        } // end if
        
        else{
            missingAmount = percentageUntilFull - account;
            display.clear();
            display.setLayout21x8();
            display.print(F("You are missing"));
            display.gotoXY(15, 0);
            display.print(missingAmount);
            display.gotoXY(18, 0);
            display.print(F("kr"));
            display.gotoXY(0, 1);
            display.print(F("Do you want to charge on debit?"));
            display.gotoXY(0,2);
            display.print(F("A = Yes"));
            display.gotoXY(12,2);
            display.print(F("B = Cancel"));

            while ((buttonA.isPressed() == 0) and (buttonB.isPressed() == 0)){
            } // end while
            if (buttonA.isPressed() == 1){
                debit += missingAmount;
                account = 0;
                chargingMode();
            } // end if

            if (buttonB.isPressed() == 1){
                // QUIT CHARGING MODE, Integrate with driving switch case?
            } // end if
        } // end else
    } // end if
} // end void