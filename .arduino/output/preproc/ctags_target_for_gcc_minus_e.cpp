# 1 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"

# 3 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 2
# 4 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 2
# 5 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 2

/////////// NOTES ////////////
/*

- Add switchcase for display modes/ buzzer

- Add switchcase in softwareBattery for special functions

- Add lineFollower

- Add switchcase in line follower for turning, job etc.

- Add Random based taxi job

- 

*/
# 16 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
Zumo32U4OLED display;
Zumo32U4Encoders encoders;
Zumo32U4Buzzer buzzer;
Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
Zumo32U4ButtonC buttonC;
Zumo32U4LineSensors lineSensors;

//variables for IR remote




const long RECV_PIN = A4;
IRrecv irrecv(RECV_PIN);
unsigned long nowTime;
unsigned long considerTime;
unsigned long irNum;
/////////////////////////////

// Variables for softwareBattery
int8_t batteryLevel = 100;
long lastDistance = 0;
float consumptionMeasure = 0;
int8_t timesCharged = 0;
unsigned long batteryMillis = 0;

// Variables for hiddenFeature()
bool hiddenActivated = false;
bool emergencyChargingUsed = false;
bool emergencyChargeMode = false;

// Variables for showBatteryStatus()
unsigned long previousMillis = 0;
unsigned long refreshPreviousMillis = 0;
long displayTime = 0;
bool batteryDisplayed = false;


// Variables for regneDistanse()
long MeassureDistance = 0;

// Variables for taxiDriver()
bool passengerFound = false;
long searchTime = 0;
long missionStart = 0;
int missionDistance = 0;
int workCase = 0;
int startDistance = 0;
int passengerEntered = 0;

// Variables for followLine
unsigned int lineSensorValues[5];
int16_t lastError = 0;
const uint16_t maxSpeed = 200;



///////// TEST VARIABLES ////
int batteryHealth = 2;
float iAmSpeed = 0;
int bankAccount = 100;



void setup(){
    Serial.begin(9600);
    lineSensors.initFiveSensors();

    // Wait for button A to be pressed and released.
    display.clear();
    display.print((reinterpret_cast<const __FlashStringHelper *>(
# 88 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 88 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                 "Press A"
# 88 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                 ); &__c[0];}))
# 88 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                 )));
    display.gotoXY(0, 1);
    display.print((reinterpret_cast<const __FlashStringHelper *>(
# 90 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 90 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                 "to start"
# 90 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                 ); &__c[0];}))
# 90 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                 )));
    buttonA.waitForButton();
    calibrateLineSensors();

    IrReceiver.begin(RECV_PIN, true);

} // end setup

void loop(){
    IrRemote();
    SpeedometerAndMeassureDistance();
    followLine();
    softwareBattery();
    showBatteryStatus();
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
        MeassureDistance +=((meters)>0?(meters):-(meters))*0.2;
        lastDisplayTime = millis();

      } // end if
      Serial.println(MeassureDistance);
}


void softwareBattery(){
    long currentMillis = millis();

    if (currentMillis - batteryMillis > 100){
    batteryMillis = currentMillis;
    consumptionMeasure += (((iAmSpeed)>0?(iAmSpeed):-(iAmSpeed))/30); // EKSEMPEL PÅ FUNKSJON, OPPDATER NÅR VI TESTER MED DATA
    Serial.print(consumptionMeasure);
    } // end if

    if (consumptionMeasure >= 10){
        batteryLevel -= 1;
        consumptionMeasure = 0;
    } // end if
} // end void

void carNeedCharging(){
    if (batteryLevel < 10){ // Gir forvarsel med Lyd, lys og display

    } // end if

    if (batteryLevel < 5){ // Stopper hvert 15 sekund og piper

    } // end if
} // end void

void hiddenFeature(){
    //int8_t averageSpeed = speedometer();
    int8_t distanceChange = MeassureDistance - lastDistance;



    // Function to turn on hiddenActivated

    // Function to turn of hiddenActivated

    // Function to turn on emergencyChargingMode



    if (hiddenActivated == true){ // FINN PÅ NOE SOM SKAL AKTIVERE FUNSKJONEN

        lastDistance = MeassureDistance;

        consumptionMeasure += (iAmSpeed / distanceChange); // EKSEMPEL PÅ FUNKSJON, OPPDATER NÅR VI TESTER MED DATA

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
            display.setLayout11x4(); // Divide screen into 11 columns and 4 rows
            display.print((reinterpret_cast<const __FlashStringHelper *>(
# 243 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 243 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         "Speed:"
# 243 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         ); &__c[0];}))
# 243 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         )));
            display.gotoXY(0,1);
            display.print(iAmSpeed);
            display.gotoXY(7,1);
            display.print((reinterpret_cast<const __FlashStringHelper *>(
# 247 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 247 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         "m/s"
# 247 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         ); &__c[0];}))
# 247 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         )));
            display.gotoXY(0,2);
            display.print((reinterpret_cast<const __FlashStringHelper *>(
# 249 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 249 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         "Distance:"
# 249 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         ); &__c[0];}))
# 249 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         )));
            display.gotoXY(0,3);
            display.print(MeassureDistance);
            display.gotoXY(7,3);
            display.print((reinterpret_cast<const __FlashStringHelper *>(
# 253 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 253 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         "cm"
# 253 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         ); &__c[0];}))
# 253 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         )));
            refreshPreviousMillis = currentMillis;
        } // end if
    } // end if


    if (currentMillis - previousMillis >= onInterval){
        display.clear();
        display.setLayout21x8(); // Divide screen into 21 columns and 8 rows
        display.print((reinterpret_cast<const __FlashStringHelper *>(
# 262 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 262 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                     "Battery level"
# 262 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                     ); &__c[0];}))
# 262 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                     )));
        display.gotoXY(15,0);
        display.print(batteryLevel);
        display.gotoXY(0,2);
        display.print((reinterpret_cast<const __FlashStringHelper *>(
# 266 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 266 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                     "Times Charged"
# 266 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                     ); &__c[0];}))
# 266 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                     )));
        display.gotoXY(15,2);
        display.print(timesCharged);
        display.gotoXY(0,4);
        display.print((reinterpret_cast<const __FlashStringHelper *>(
# 270 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 270 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                     "Battery Health"
# 270 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                     ); &__c[0];}))
# 270 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                     )));
        display.gotoXY(15,4);
        display.print(batteryHealth);
        if(batteryLevel < 10){
            display.gotoXY(0,6);
            display.print("Please recharge!");
        }
        previousMillis = currentMillis;
        displayTime = currentMillis;
        batteryDisplayed = true; // To make the next if sentence only run once after this text have been ran
    } // end if

    if ((currentMillis - displayTime >= offInterval) && (batteryDisplayed == true)){
        batteryDisplayed = false;
    } // end if

}//end void showBatteryStatus

void searchForPassenger(){

    int currentMillis = millis();

    if (passengerFound = false){
        searchTime = random(1000, 5000);
        passengerFound = true;
    } // end if

    else if (passengerFound == true){
        if (currentMillis - missionStart > searchTime){
            missionDistance = random(100,300);
            motors.setSpeeds(0,0);
            display.clear();
            display.setLayout21x8();
            display.print((reinterpret_cast<const __FlashStringHelper *>(
# 303 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 303 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         "The passenger want to travel"
# 303 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         ); &__c[0];}))
# 303 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         )));
            display.gotoXY(0,1);
            display.print((reinterpret_cast<const __FlashStringHelper *>(
# 305 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 305 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         "travel for"
# 305 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         ); &__c[0];}))
# 305 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         )));
            display.gotoXY(12,1);
            display.print(missionDistance);
            display.gotoXY(18,1);
            display.print((reinterpret_cast<const __FlashStringHelper *>(
# 309 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 309 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         "cm"
# 309 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         ); &__c[0];}))
# 309 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         )));
            display.gotoXY(0,3);
            display.print((reinterpret_cast<const __FlashStringHelper *>(
# 311 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 311 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         "Do you take the job?"
# 311 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         ); &__c[0];}))
# 311 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         )));
            display.gotoXY(0,4);
            display.print((reinterpret_cast<const __FlashStringHelper *>(
# 313 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 313 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         "A = YES"
# 313 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         ); &__c[0];}))
# 313 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         )));
            display.gotoXY(15,4);
            display.print((reinterpret_cast<const __FlashStringHelper *>(
# 315 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 315 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         "B = NO"
# 315 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         ); &__c[0];}))
# 315 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         )));
            display.gotoXY(0,5);
            display.print((reinterpret_cast<const __FlashStringHelper *>(
# 317 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 317 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         "C = Off duty"
# 317 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         ); &__c[0];}))
# 317 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         )));
            while ((buttonA.isPressed() == 0) and (buttonB.isPressed() == 0) and buttonC.isPressed() == 0){
            } // end while
            if (buttonA.isPressed() == 1){
                passengerEntered = currentMillis;
                startDistance = MeassureDistance;
                workCase = 2;
                passengerFound = true;
            } // end if

            else if (buttonB.isPressed() == 1){
                workCase = 1;
            } // end if

            else if (buttonC.isPressed() == 1){
                workCase = 0;
            } // end if     
        } // end if
    } // end if
} // end void

void drivePassenger(){
    int currentMillis = millis();
    if (MeassureDistance - startDistance > missionDistance){
        motors.setSpeeds(0,0);
        int payment = (missionDistance / (currentMillis - missionStart)) * 1000;
        bankAccount += payment;
        display.clear();
        display.setLayout21x8();
        display.print((reinterpret_cast<const __FlashStringHelper *>(
# 346 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 346 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                     "Passanger delivered"
# 346 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                     ); &__c[0];}))
# 346 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                     )));
        display.gotoXY(0,2);
        display.print((reinterpret_cast<const __FlashStringHelper *>(
# 348 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 348 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                     "Payment:"
# 348 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                     ); &__c[0];}))
# 348 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                     )));
        display.gotoXY(13,2);
        display.print(payment);
        display.gotoXY(18,2);
        display.print((reinterpret_cast<const __FlashStringHelper *>(
# 352 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 352 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                     "kr"
# 352 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                     ); &__c[0];}))
# 352 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                     )));
        display.gotoXY(0,4);
        display.print((reinterpret_cast<const __FlashStringHelper *>(
# 354 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 354 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                     "Continue Working?"
# 354 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                     ); &__c[0];}))
# 354 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                     )));
        display.gotoXY(0,5);
        display.print((reinterpret_cast<const __FlashStringHelper *>(
# 356 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 356 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                     "A = Search for client"
# 356 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                     ); &__c[0];}))
# 356 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                     )));
        display.gotoXY(0,6);
        display.print((reinterpret_cast<const __FlashStringHelper *>(
# 358 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 358 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                     "B = End work"
# 358 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                     ); &__c[0];}))
# 358 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                     )));
        while ((buttonA.isPressed() == 0) and (buttonB.isPressed() == 0)){
        } // end while
        if (buttonA.isPressed() == 1){
            workCase = 1;
        } // end if
        else if (buttonB.isPressed() == 1){
            workCase = 0;
        } // end if
    } // end if
} // end void

void taxiDriver(){
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
    }
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



    leftSpeed = ((leftSpeed)<(0)?(0):((leftSpeed)>((int16_t)maxSpeed)?((int16_t)maxSpeed):(leftSpeed)));
    rightSpeed = ((rightSpeed)<(0)?(0):((rightSpeed)>((int16_t)maxSpeed)?((int16_t)maxSpeed):(rightSpeed)));
    if(batteryLevel > 0){
    motors.setSpeeds(leftSpeed,rightSpeed);
    }else{
    motors.setSpeeds(0,0);
    }
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

void calculateAverage(){
    int currentMillis = millis();
    int interval = 60000;
    uint8_t previousMillis;
    if(currentMillis - previousMillis > interval){

    }

}// end void calculateAverage
