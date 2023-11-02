int potensiometer = A0; //Definere pin som vi skal lese av
int gjennomsnittArray[4]; // definerer lengden på arrayet, Lengden her er 5.
const int arrayLengde = 5; //Variabel som er lik lengde på array


void setup(){
    Serial.begin(9600);
    pinMode(INPUT, A0); //Definerer pin som vi kobler i og sier at det er en input pin.
}

void gjennomsnitt(){//lager en funksjon som regner ut gjennomsnittet
    int verdi = analogRead(potensiometer); //Leser av potensiometer pinnen.
    int sum = 0; //Variabel som vi skal bruke for å legge sammen alle verdier.
    for(int i = arrayLengde-1; i >0; i--){ 
        gjennomsnittArray[i] = gjennomsnittArray[i-1];
        sum += gjennomsnittArray[i];
    }

    /*Her har vi en for løkke som flytter verdiene i arrayet en plass til høyre slik at vi
     kan sette inn den nyeste verdien i posisjon 0. vi summerer også verdiene og 
     legger de i variabelen sum
    */
    gjennomsnittArray[0] = verdi; //Legger nyeste verdi i index 0 i arrayet.
    sum += verdi; //legger også til den siste verdien i sum

    int avrage = (sum)/arrayLengde; //regner ut gjennomsnittet ved å dele på lengden av arrayet.

    Serial.println(avrage); //printer avrage
}

void loop(){
    gjennomsnitt(); //kjører void gjennomsnitte
    delay(200); // delay på 200 millisekunder.
}