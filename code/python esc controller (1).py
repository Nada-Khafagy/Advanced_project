import time
import RPi.GPIO as GPIO
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)

GPIO.setup(7, GPIO.OUT)

p = GPIO.PWM(7, 50)

p.start(0)
print ("starting 0")
time.sleep(3)

p.ChangeDutyCycle(3)
print("start")
time.sleep(5)



while True:
	i = 4
	while i<10:
		
		print(i)
		p.ChangeDutyCycle(i)
		time.sleep(.05)
		i +=.02
	
	while i>4:
		print(i)
		p.ChangeDutyCycle(i)
		time.sleep(.05)
		i -=.05
