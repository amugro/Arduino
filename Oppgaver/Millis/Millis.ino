int knapp = 2; //definerer pin som knappen er koblet til
int knappVerdi; // en variabel hvor vi kan lagre verdien til knappen
int nullVerdi = LOW; //en nullverdi for knappen
int led2State = LOW; //Null verdi for led2, altså den i midten
int led1 = 9; //definerer pin som led 1 er koblet til
int led2 = 10;//definerer pin som led 2 er koblet til
int led3 = 11;//definerer pin som led 3 er koblet til
int mode = 0; //en variabel som vi skal bruket til å 
            //velge mellom modus ved hjelp av knappetrykk

const unsigned long interval = 1000; /*interval på 1 sekund
                                brukes til å blinke uten å bruke delay*/
unsigned long previousMillis; //forrige millis, brukes sammen med interval for å få blinke uten delay
unsigned long currentMillis; //Millis som skal gå og telle




void setup(){
    Serial.begin(9600);
    pinMode(knapp, INPUT); //Definerer knappen som input
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);
    /*setter opp alle led til en pinne*/
}


void button(){ //lager en funksjon som leser av knappen og velger hvilke LED som skal lyse.
    int knappVerdi = digitalRead(knapp); //leser av knappeverdien, 0 sålenge den ikke er trykket ned.
    if(knappVerdi != nullVerdi){//hvis knapen er ulik nullverdien, altså LOW
        if(knappVerdi == HIGH){ //og knappVerdi er HIGH, altså trykket ned.
            if(mode == 0){
                //da skal LED 1 lyse.
                digitalWrite(led1, HIGH);
                digitalWrite(led3, LOW);
                mode++;
                //mode blir 1 etter trykket.
            }else{
                mode--; //mode-- hvis si mode - 1, dette vil bli 0
                digitalWrite(led1, LOW);
                digitalWrite(led3, HIGH);//lED 3 skal lyse
            }
        }
    }
    nullVerdi = knappVerdi;
}

void blinkMillis(){//funksjon som gjøre at LED 2 skal blinke uavhengig av knappetrykk
    currentMillis = millis(); //starter millisfunksjonen, denne vil bare fortsette å telle-
    if(currentMillis - previousMillis >= interval){
        //hvis currentmillis - previousMillis(0) er større eller lik interval(1000)
        //så kjører setningen under.
        previousMillis = currentMillis;//setter previous til currentmillis slik at det vil ta et 
        //sekund før if setningen kjører igjenn.
        
        if(led2State == LOW){//hvis led2state er lav.
            led2State = HIGH;//så skal du endre til høy.
        }else{
            led2State = LOW;//hvis den ikke er lav, skal den bli lav.
        }
        digitalWrite(led2, led2State);//printer Led2state på led2, den er enten høy eller lav
        //og oppdateres hvert sekund ved hjelp av Millis.
    }


}

void loop(){
    //initialiserer begge funksjonene.
    button();
    blinkMillis();
}