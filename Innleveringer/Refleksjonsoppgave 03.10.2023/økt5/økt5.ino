const int buttonPin = 2; 

int buttonPushCounter = 0;  
int buttonState = 0;       
int lastButtonState = 0;   
int interval;
int previousMillis = 0;

void setup() {
    
  Serial.begin(9600);
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
        interval =  - previousMillis;
        Serial.println(interval);
    }

    lastButtonState = buttonState;

    if(buttonPushCounter = 5){
        buttonPushCounter = 0;
    }
}


void loop() {
  buttonState = digitalRead(buttonPin);
}