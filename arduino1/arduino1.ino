const int trigPin = 9;
const int echoPin = 10;
// defines variables
long duration;
int distance;
void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
}
void loop() {
    Serial.print("Hei");
    delay(1000);
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
}


/*const int button = 1;
const int trigPin = 2;
const int  echoPin = 3;
const int knap = 4;

long duration;
int distance;
int knappVerdi = LOW;


void setup(){
    Serial.begin(9600);
    pinMode(button, INPUT);
    pinMode(2, OUTPUT);
    pinMode(3, INPUT);
}

void knapp(){
    knappVerdi = digitalRead(knap);
    if(knappVerdi == HIGH){
        
    }
}

void distance(){
    digitalWrite(trigpin)
}


void loop(){
}*/