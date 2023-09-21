const int LED_PIN = LED_BUILTIN;

void blinkLed() {  
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN,  LOW);
    delay(200);
}

void setup(){
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
}

void loop(){
    blinkLed();
    }