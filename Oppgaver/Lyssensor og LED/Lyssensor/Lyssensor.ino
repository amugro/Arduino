//standard beregning av gjennomsnitt.
int lyssensorPin = A0; // lyssensor pin
int LED = 2; //led pin
int LysSensorVerdi; //verdi for lyssensoren
int currentMillis;
int previousMillis = 0;


void setup(){
    pinMode(lyssensorPin, INPUT);
    pinMode(LED, OUTPUT);
    Serial.begin(9600);
}

int lysSensor(int i){
    int sum = 0; //sum, skal summere alle verdiene i denne variabelen.
    for(i; i > 0; i--){//for løkke som teller ned, kan lett endre på i slik at vi får
    //flere verdier inn i gjennomsnittet.
        int LysSensorVerdi = analogRead(lyssensorPin);//leser av lyssensoren.
        sum += LysSensorVerdi; //summerer lyssensorverdiene i en sum
        delay(5);//en liten delay, slik at det kan være forskjell på verdiene.
    
    }
    int split = sum/i;//regner ut gjennomsnittet ved å dele på hvor mange verider vi har.
    return split; //returnerer denne verdien
}

void ledBlink(){
    int avrage = lysSensor(5); //avrage er verdien som ble returnert fra forrige funksjon.
    Serial.println(avrage);
    if(avrage < 200){//hvis avrage er mindre enn 200, skal en LED blinke 5 ganger
        for(int n = 0; n < 5; n++){
            digitalWrite(LED, HIGH);
            delay(100);
            digitalWrite(LED, LOW);
            delay(100);   
            if(n == 5){
                digitalWrite(LED, LOW);
            }
        } 
    }
    else{ //avrage er større enn 200 skal den blink 1 gang på 2 sekunder.
        digitalWrite(LED, HIGH);
        delay(1000);
        digitalWrite(LED, LOW);
        delay(1000);
    }
}

void loop(){
    ledBlink();//initialisere ledBlink funkjsonen.
}