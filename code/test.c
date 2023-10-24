#include <wiringPi.h>
#include <softPwm.h>
#include <unistd.h>

#define MOTOR_PIN 1  // The GPIO pin to which the ESC is connected
#define PWM_RANGE 100
#define PWM_FREQUENCY 1000 // PWM frequency in Hz

int main() {
    if (wiringPiSetup() == -1) {
        return 1; // Failed to initialize WiringPi
    }

    if (softPwmCreate(MOTOR_PIN, 0, PWM_RANGE) != 0) {
        return 1; // Failed to initialize PWM on MOTOR_PIN
    }

    // Set PWM frequency (period) in microseconds
    int period = 1000000 / PWM_FREQUENCY;
    softPwmWrite(MOTOR_PIN, PWM_RANGE);
    usleep(period);

    int speed = 0; // Motor speed (0 to 100)

    while (1) {
        // Gradually increase motor speed
        for (speed = 0; speed <= PWM_RANGE; speed++) {
            softPwmWrite(MOTOR_PIN, speed);
            usleep(10000); // Adjust this delay to control the speed increase rate
        }

        usleep(2000000); // Run the motor at full speed for 2 seconds

        // Gradually decrease motor speed
        for (speed = PWM_RANGE; speed >= 0; speed--) {
            softPwmWrite(MOTOR_PIN, speed);
            usleep(10000); // Adjust this delay to control the speed decrease rate
        }

        usleep(2000000); // Run the motor at minimum speed for 2 seconds
    }

    return 0;
}
