const int buttonPin = 2; 
const int ledPin = 1; 

int interval = 1000;
int previousMillis = 0;
int ledState = LOW;

int buttonPushCounter = 0;  
int buttonState = 0;       
int lastButtonState = 0;    

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void knapp(){
  buttonState = digitalRead(buttonPin);

  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {   
      buttonPushCounter++;
      Serial.println(buttonPushCounter);
  	}
  }

  lastButtonState = buttonState;

  if(buttonPushCounter == 3){
    buttonPushCounter = 0;
  }
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
            digitalWrite(ledPin, ledState);
        }

}


void loop() {
    void knapp();
}
