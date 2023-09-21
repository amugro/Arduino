const int LED_PIN = LED_BUILTIN;
const int buttonPin = 2;
bool buttonState = false;


void blinkLed() {  
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN,  LOW);
    delay(200);
}

void updateButton(){
    buttonState = digitalRead(buttonPin);
}

void setup(){
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
    pinMode(buttonPin, INPUT);
}

void loop(){
    updateButton();
    //blinkLed();
    if (buttonState){
        blinkLed();
    }
    }