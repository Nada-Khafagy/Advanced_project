import pigpio
import time

# Connect to the pigpio daemon
pi = pigpio.pi()

# Specify the GPIO pin connected to the signal input of the ESC
esc_pin = 23
pi.set_PWM_frequency(esc_pin, 50)
#print(pi.get_PWM_frequency(23))
try:
    # Arm the ESC (send a high signal)
    pi.set_servo_pulsewidth(esc_pin, 0)
    time.sleep(1)

    # Set a lower signal for minimum throttle (adjust as needed)
    #pi.set_servo_pulsewidth(esc_pin, 1000)
    #time.sleep(4)

    #pi.set_servo_pulsewidth(esc_pin, 1050)
    #time.sleep(2)

finally:
    # Disarm the ESC (send a low signal)
    pi.set_servo_pulsewidth(esc_pin, 0)
    pi.stop()