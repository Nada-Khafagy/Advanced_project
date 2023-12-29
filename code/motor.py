#some library
import RPi.GPIO as GPIO
import time

GPIO_PIN = 23
GPIO.setmode(GPIO.BCM)
GPIO.setup(GPIO_PIN, GPIO.OUT)

pwm = GPIO.PWM(GPIO_PIN, 50)  # 50 Hz frequency

pwm.start(0)  # Start PWM with 0% duty cycle
time.sleep(3)


try:
    while True:
        # Adjust duty cycle as needed (e.g., from 0 to 100 for speed control)
        pwm.ChangeDutyCycle(5)  # Example: 50% duty cycle
        time.sleep(2)
        
     



except KeyboardInterrupt:
    pwm.stop()
    GPIO.cleanup()