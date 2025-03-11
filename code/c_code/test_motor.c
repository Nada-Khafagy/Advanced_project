#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include <unistd.h>

#define MIN_PW 10 // Minimum pulse width in microseconds
#define MAX_PW 20 // Maximum pulse width in microseconds


#define PWM_PIN 1

int main() {
    if (wiringPiSetup () == -1)
    {
        return 1 ;
    }

    pinMode(PWM_PIN, PWM_OUTPUT) ; /* set PWM pin as output */

    int frequency = 50; 
    int clk_freq = 19200000;
    int range_resolution = 1024;
    int range = clk_freq / (range_resolution*frequency);
    range = 200;

    if (softPwmCreate(PWM_PIN,0,range) != 0)
    {
        return 1;
    }

    //pwmSetMode(PWM_MODE_MS);
    //pwmSetRange(range);
    //pwmSetClock(192);

    softPwmWrite(PWM_PIN, MIN_PW);
    usleep(4000000);

    /*while(1)
    {
        softPwmWrite(PWM_PIN, 12);
        usleep(100);
    }*/
    

    while(1) {
        for (int i = MIN_PW; i <= 11; i++) {
            softPwmWrite(PWM_PIN, i);
            usleep(100000); // Adjust this delay for the desired speed of change
        }
        usleep(1000000);
        for (int i = 11; i >= MIN_PW; i--) {
            softPwmWrite(PWM_PIN, i);
            usleep(100000); // Adjust this delay for the desired speed of change
        }
        usleep(1000000);
    }


    return 0;


    

}