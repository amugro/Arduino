const int analogInPin = A1;
const int digitalOutPin = 1;

int sensorvalue = 0;
int outPutValue= 0;

void setup(){
    Serial.begin(9600);
}


void loop(){
    sensorvalue = analogRead(analogInPin);
    outPutValue = map(sensorvalue, 0, 1023, 0, 255);

    analogWrite(digitalOutPin, outPutValue);

    Serial.print(" sensor = ");
    Serial.print(sensorvalue);
    Serial.print(" Output = ");
    Serial.print(outPutValue);

    delay(10);

}