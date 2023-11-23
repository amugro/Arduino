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

// Variables for speedometer
int16_t previousCountLeft = 0;
int16_t previousCountRight = 0;

// Variables for regneDistanse()
int16_t lastAverage = 0;
long distance = 0;
unsigned long distanceMillis = 0;

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
# 93 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 93 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                 "Press A"
# 93 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                 ); &__c[0];}))
# 93 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                 )));
    display.gotoXY(0, 1);
    display.print((reinterpret_cast<const __FlashStringHelper *>(
# 95 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 95 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                 "to start"
# 95 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                 ); &__c[0];}))
# 95 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                 )));
    buttonA.waitForButton();
    calibrateLineSensors();

    IrReceiver.begin(RECV_PIN, true);

} // end setup

void loop(){
    IrRemote();
    meassureDistance();
    speedometer();
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

float speedometer(){
    static uint8_t lastDisplayTime;

    long countsLeft = encoders.getCountsLeft();
    long countsRight = encoders.getCountsRight();
    if ((uint8_t)(millis() - lastDisplayTime) >= 100)
    {
        int16_t newCountLeft = countsLeft - previousCountLeft;
        int16_t newCountRight = countsRight - previousCountRight;
        int16_t avrage = (newCountLeft+newCountRight)/2;
        float distanse = 75.81*12;
        float oneRound = 122.5221135;
        float meters = avrage/distanse*oneRound;
        //Serial.println(meters);
        previousCountLeft = countsLeft;
        previousCountRight = countsRight;
        //display.clear();
        //display.print(meters);
        lastDisplayTime = millis();
        iAmSpeed = meters;
        return meters;
    } // end if
} // end void

void meassureDistance(){
    int currentMillis = millis();

    if (currentMillis - distanceMillis > 100){
        // Millis funksjon
        long countsLeft = encoders.getCountsLeft(); // Get amount of encoder readings
        long countsRight = encoders.getCountsRight();

        long average = (countsLeft + countsRight)/2; // Uses the average of the encoder readings

        float round = 75.81*12; // Calculation of wheelrotation
        int diffAverage = ((average-lastAverage)>0?(average-lastAverage):-(average-lastAverage)); // use the absolute value to count distance both forward and backward
        distance += (diffAverage/round)*12.5221135;

        lastAverage = average; // Make current readings as reffrence for next run's calculations
        distanceMillis = currentMillis;
    } // end if
} // end void


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
    int8_t averageSpeed = speedometer();
    int8_t distanceChange = distance - lastDistance;



    // Function to turn on hiddenActivated

    // Function to turn of hiddenActivated

    // Function to turn on emergencyChargingMode



    if (hiddenActivated == true){ // FINN PÅ NOE SOM SKAL AKTIVERE FUNSKJONEN

        lastDistance = distance;

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
            display.setLayout11x4(); // Divide screen into 11 columns and 4 rows
            display.print((reinterpret_cast<const __FlashStringHelper *>(
# 273 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 273 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         "Speed:"
# 273 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         ); &__c[0];}))
# 273 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         )));
            display.gotoXY(0,1);
            display.print(iAmSpeed);
            display.gotoXY(7,1);
            display.print((reinterpret_cast<const __FlashStringHelper *>(
# 277 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 277 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         "m/s"
# 277 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         ); &__c[0];}))
# 277 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         )));
            display.gotoXY(0,2);
            display.print((reinterpret_cast<const __FlashStringHelper *>(
# 279 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 279 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         "Distance:"
# 279 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         ); &__c[0];}))
# 279 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         )));
            display.gotoXY(0,3);
            display.print(distance);
            display.gotoXY(7,3);
            display.print((reinterpret_cast<const __FlashStringHelper *>(
# 283 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 283 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         "cm"
# 283 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         ); &__c[0];}))
# 283 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         )));
            refreshPreviousMillis = currentMillis;
        } // end if
    } // end if


    if (currentMillis - previousMillis >= onInterval){
        display.clear();
        display.setLayout21x8(); // Divide screen into 21 columns and 8 rows
        display.print((reinterpret_cast<const __FlashStringHelper *>(
# 292 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 292 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                     "Battery level"
# 292 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                     ); &__c[0];}))
# 292 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                     )));
        display.gotoXY(15,0);
        display.print(batteryLevel);
        display.gotoXY(0,2);
        display.print((reinterpret_cast<const __FlashStringHelper *>(
# 296 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 296 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                     "Times Charged"
# 296 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                     ); &__c[0];}))
# 296 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                     )));
        display.gotoXY(15,2);
        display.print(timesCharged);
        display.gotoXY(0,4);
        display.print((reinterpret_cast<const __FlashStringHelper *>(
# 300 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 300 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                     "Battery Health"
# 300 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                     ); &__c[0];}))
# 300 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
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
# 333 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 333 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         "The passenger want to travel"
# 333 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         ); &__c[0];}))
# 333 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         )));
            display.gotoXY(0,1);
            display.print((reinterpret_cast<const __FlashStringHelper *>(
# 335 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 335 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         "travel for"
# 335 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         ); &__c[0];}))
# 335 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         )));
            display.gotoXY(12,1);
            display.print(missionDistance);
            display.gotoXY(18,1);
            display.print((reinterpret_cast<const __FlashStringHelper *>(
# 339 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 339 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         "cm"
# 339 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         ); &__c[0];}))
# 339 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         )));
            display.gotoXY(0,3);
            display.print((reinterpret_cast<const __FlashStringHelper *>(
# 341 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 341 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         "Do you take the job?"
# 341 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         ); &__c[0];}))
# 341 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         )));
            display.gotoXY(0,4);
            display.print((reinterpret_cast<const __FlashStringHelper *>(
# 343 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 343 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         "A = YES"
# 343 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         ); &__c[0];}))
# 343 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         )));
            display.gotoXY(15,4);
            display.print((reinterpret_cast<const __FlashStringHelper *>(
# 345 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 345 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         "B = NO"
# 345 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         ); &__c[0];}))
# 345 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         )));
            display.gotoXY(0,5);
            display.print((reinterpret_cast<const __FlashStringHelper *>(
# 347 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 347 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         "C = Off duty"
# 347 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                         ); &__c[0];}))
# 347 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                         )));
            while ((buttonA.isPressed() == 0) and (buttonB.isPressed() == 0) and buttonC.isPressed() == 0){
            } // end while
            if (buttonA.isPressed() == 1){
                passengerEntered = currentMillis;
                startDistance = distance;
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
    if (distance - startDistance > missionDistance){
        motors.setSpeeds(0,0);
        int payment = (missionDistance / (currentMillis - missionStart)) * 1000;
        bankAccount += payment;
        display.clear();
        display.setLayout21x8();
        display.print((reinterpret_cast<const __FlashStringHelper *>(
# 376 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 376 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                     "Passanger delivered"
# 376 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                     ); &__c[0];}))
# 376 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                     )));
        display.gotoXY(0,2);
        display.print((reinterpret_cast<const __FlashStringHelper *>(
# 378 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 378 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                     "Payment:"
# 378 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                     ); &__c[0];}))
# 378 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                     )));
        display.gotoXY(13,2);
        display.print(payment);
        display.gotoXY(18,2);
        display.print((reinterpret_cast<const __FlashStringHelper *>(
# 382 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 382 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                     "kr"
# 382 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                     ); &__c[0];}))
# 382 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                     )));
        display.gotoXY(0,4);
        display.print((reinterpret_cast<const __FlashStringHelper *>(
# 384 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 384 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                     "Continue Working?"
# 384 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                     ); &__c[0];}))
# 384 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                     )));
        display.gotoXY(0,5);
        display.print((reinterpret_cast<const __FlashStringHelper *>(
# 386 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 386 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                     "A = Search for client"
# 386 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                     ); &__c[0];}))
# 386 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                     )));
        display.gotoXY(0,6);
        display.print((reinterpret_cast<const __FlashStringHelper *>(
# 388 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 388 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
                     "B = End work"
# 388 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino" 3
                     ); &__c[0];}))
# 388 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\Zumo\\softwarebattery\\softwarebattery.ino"
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
