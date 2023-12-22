import time
import RPi.GPIO as GPIO

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)

GPIO.setup(7, GPIO.OUT)

#define 50 Hz
p = GPIO.PWM(7, 50)

p.start(0)
print ("starting 0")
time.sleep(3)

p.ChangeDutyCycle(3) #600us
print("start")
time.sleep(5)



while True:
	i = 4 #800us
	#go to max
	while i<7:
		print(i)
		p.ChangeDutyCycle(i)
		time.sleep(.05)
		i +=.02
	#go to min
	while i>4:
		print(i)
		p.ChangeDutyCycle(i)
		time.sleep(.05)
		i -=.05
