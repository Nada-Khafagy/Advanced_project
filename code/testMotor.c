#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

#define RANGE 10             // PWM range (0 to RANGE)


const int PWM_pin = 1;   /* GPIO 1 as per WiringPi, GPIO18 as per BCM */

int main (void)
{
  int intensity ;            

  if (wiringPiSetup () == -1)
    exit (1) ;

    // Initialize PWM pin for motor control
  softPwmCreate(PWM_pin, 0, RANGE);

  pinMode (PWM_pin, PWM_OUTPUT) ; /* set PWM pin as output */

  while (1)
  {
	
    for (intensity = 0 ; intensity < 1024 ; ++intensity)
    {
      pwmWrite (PWM_pin, intensity) ;	/* provide PWM value for duty cycle */
      delay (1) ;
    }
    delay(1);

    for (intensity = 1023 ; intensity >= 0 ; --intensity)
    {
      pwmWrite (PWM_pin, intensity) ;
      delay (1) ;
    }
    delay(1);
}
}