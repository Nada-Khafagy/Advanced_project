#include <Arduino.h> // Include the Arduino library

#define LEDPIN 3

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    for(int i = 0; i < 255; i++)
    {
        analogWrite(LEDPIN, i);
        Serial.println(i);
        delay(10);
    }

}