#include <Arduino.h>
#line 1 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\arduino1\\arduino1.ino"
#line 1 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\arduino1\\arduino1.ino"
void setup();
#line 5 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\arduino1\\arduino1.ino"
void loop();
#line 1 "C:\\Users\\amund\\OneDrive\\Dokumenter\\IELS 1001\\Arduino\\arduino1\\arduino1.ino"
void setup(){
    Serial.begin(9600);
}

void loop(){
    Serial.println("Hello wotld!");
    delay(1000);
}
