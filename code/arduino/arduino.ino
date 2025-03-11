#include <Servo.h>

Servo FRONT_ESC;//twords x axis
Servo RIGHT_ESC;
Servo LEFT_ESC;
Servo BACK_ESC;

#define FRONT_ESCPIN 6
#define RIGHT_ESCPIN 9
#define LEFT_ESCPIN 10
#define BACK_ESCPIN 11

#define POTPIN A0
#define MAX_VELOCITY 255

int integerVariable;
int potValue;

void setup() {
  FRONT_ESC.attach(FRONT_ESCPIN, 1000, 2000);
  RIGHT_ESC.attach(RIGHT_ESCPIN, 1000, 2000);
  LEFT_ESC.attach(LEFT_ESCPIN, 1000, 2000);
  BACK_ESC.attach(BACK_ESCPIN, 1000, 2000);
  
  Serial.begin(9600);
  Serial.println("Enter Motors collective Speed (0-120) max");
  
  FRONT_ESC.write(0);
  RIGHT_ESC.write(0);
  LEFT_ESC.write(0);
  BACK_ESC.write(0);
  delay(100);
  
}

void loop() {
    //potValue = analogRead(POTPIN);   // reads the value of the potentiometer (value between 0 and 1023)
    //potValue = map(potValue, 0, 1023, 0, 180);   // scale it to use it with the servo library 
    //ESC.write(potValue);
    
    
  while (Serial.available() == 0) {
    // Wait for input
  }

  if (Serial.available()) {
    // Read the input value
    integerVariable = Serial.parseInt();

    
    // Validate the input range
    if (integerVariable >= 0 && integerVariable <= MAX_VELOCITY) {
      Serial.println(integerVariable);
      FRONT_ESC.write(integerVariable);
      RIGHT_ESC.write(integerVariable+5);
      LEFT_ESC.write(integerVariable);
      BACK_ESC.write(integerVariable+5);
      
    } else {
      Serial.println("Invalid input. Please enter a value between 0 and 120.");
    }

    // Clear the Serial buffer
    while (Serial.available() > 0) {
      Serial.read();
    }}
  
}
