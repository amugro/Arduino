const int analogInPin = A1;
const int buttonPin = 2; 
const int digitalOutPin = 1;
const int digitalOutRed = 6;
const int digitalOutGreen = 5;
const int digitalOutBlue = 4;
int ledState = LOW;
 
int buttonPushCounter = 0;  
int buttonState = 0;       
int lastButtonState = 0;   
int interval;
int previousMillis = 0;


int buttonValue = 0;
int sensorvalue = 0;
int outPutValue= 0;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 0;

void setup(){
    Serial.begin(9600);
  	pinMode(digitalOutRed, OUTPUT);
 	pinMode(digitalOutBlue, OUTPUT);
  	pinMode(digitalOutGreen, OUTPUT);
    pinMode(buttonPin, INPUT);
    
}

void knapp(){
    int currentMillis = millis();
    buttonState = digitalRead(buttonPin);
    if (buttonState != lastButtonState) {
        previousMillis = currentMillis;
        if (buttonState == HIGH) {   
        buttonPushCounter++;
        Serial.println(buttonPushCounter);
        }

    lastButtonState = buttonState;

    if(buttonPushCounter == 3){
        buttonPushCounter = 1;
        }
    }

}

void potensiometer(){
    sensorvalue = analogRead(analogInPin);
    outPutValue = map(sensorvalue, 0, 1023, 0, 255);

  	analogWrite(digitalOutRed, outPutValue);
  	analogWrite(digitalOutBlue, outPutValue);
  	analogWrite(digitalOutGreen, outPutValue);
}

void blink(){
	int currentMillis = millis();
  if (currentMillis - previousMillis >= interval){
    previousMillis = currentMillis;
    if(ledState == LOW){
    	ledState = HIGH;
    }else{
    	ledState = LOW;
    }
  }

    analogWrite(digitalOutRed, outPutValue);
    analogWrite(digitalOutBlue, outPutValue);
  	analogWrite(digitalOutGreen, outPutValue);
}




void loop(){
    knapp();
    if (buttonPushCounter == 0){
        potensiometer();
    }else if(buttonPushCounter == 1){
        potensiometer();
        blink();
    }
}