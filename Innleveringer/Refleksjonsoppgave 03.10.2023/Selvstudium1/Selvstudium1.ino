//Blink without delay
const int LED = 2;

const int interval = 1000;
int previousMillis = 0;
int ledState = LOW;


void setup()
{
  pinMode(LED, OUTPUT);
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

  digitalWrite(LED, ledState);
}


void loop()
{
	blink();
}