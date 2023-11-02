int input = 0;
const int red = 8;
const int green = 12;
const int Yellow = 10;
int ledStateRed = LOW;
int ledStateYellow = LOW;
int ledStateGreen = LOW;


void setup(){
    Serial.begin(9600);
    pinMode(red, OUTPUT);
    pinMode(green, OUTPUT);
    pinMode(Yellow, OUTPUT);
}

void RGB(){
    if(Serial.available() > 0){
        byte input = Serial.read();
        switch (input)
        {
        case 'R':
            if(ledStateRed == LOW){
                ledStateRed = HIGH;
            }
            else{
            ledStateRed = LOW;               
            }

            digitalWrite(red, ledStateRed);
            break;
        case 'Y':
            if(ledStateYellow == LOW){
                ledStateYellow = HIGH;
            }
            else{
            ledStateYellow = LOW;               
            }

            digitalWrite(Yellow, ledStateYellow);
            break;

        case 'G':
            if(ledStateGreen == LOW){
                ledStateGreen = HIGH;
            }
            else{
            ledStateGreen = LOW;               
            }

            digitalWrite(green, ledStateGreen);
            break;
        case 'O':
        digitalWrite(red, LOW);
        digitalWrite(green, LOW);
        digitalWrite(Yellow, LOW);
            break;
        default:
        Serial.println("SKriv inn en gyldig verdi, enten R, Y eller G");
            break;
        }
    }
}

void loop(){
    RGB();
}