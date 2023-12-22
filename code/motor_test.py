import RPi.GPIO as GPIO
import time

GPIO_PIN = 23
GPIO.setmode(GPIO.BCM)
GPIO.setup(GPIO_PIN, GPIO.OUT)

#pwm = GPIO.PWM(GPIO_PIN, 50)  # 50 Hz frequency

#pwm.start(0)  # Start PWM with 0% duty cycle
#time.sleep(3)

minmum_time = 1000 #us
maximum_time = 2000 #us
freq = 50

toltal_time = 1.0/freq

#duty cylcle 0>100
def drive_motor(duty_cycle, motor_pin):
    time_on = (minmum_time + (minmum_time * (duty_cycle/100.0)))/1000000 #in second
    time_off = toltal_time - time_on
    print("time on", time_on)
    print("time off", time_off)
    GPIO.output(motor_pin,GPIO.HIGH) 
    time.sleep(time_on)
    GPIO.output(motor_pin,GPIO.LOW)

    time.sleep(time_off)

#intalize motor
drive_motor(0, GPIO_PIN)
time.sleep(4)

try:
    while True:
        # Adjust duty cycle as needed (e.g., from 0 to 100 for speed control)
        #pwm.ChangeDutyCycle(5)  # Example: 50% duty cycle
        #time.sleep(2)
        drive_motor(10, GPIO_PIN)
     



except KeyboardInterrupt:
    GPIO.output(GPIO_PIN, GPIO.LOW)
    GPIO.cleanup()