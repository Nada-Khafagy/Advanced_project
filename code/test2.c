#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include <unistd.h>


#define MIN_PW 1000 // Minimum pulse width in microseconds
#define MAX_PW 2000 // Maximum pulse width in microseconds
#define PWM_PIN 1 

int main() {
    if (wiringPiSetup () == -1)
    {
        return 1 ;
    }
    pinMode (PWM_PIN, PWM_OUTPUT) ; /* set PWM pin as output */
    int frequency = 2000;
    int range = 19200000/frequency;

    pwmSetMode(PWM_MODE_MS);
    pwmSetRange(range);
    pwmSetClock(192);

    

     while(1) {
        for (int i = MIN_PW; i <= MAX_PW; i++) {
            softPwmWrite(PWM_PIN, i);
            usleep(100); // Adjust this delay for the desired speed of change
        }
        for (int i = MAX_PW; i >= MIN_PW; i--) {
            softPwmWrite(PWM_PIN, i);
            usleep(100); // Adjust this delay for the desired speed of change
        }
    }

    //pwmWrite(PWM_PIN, range/2);
    return 0;


    

}