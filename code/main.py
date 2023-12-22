import time
import threading
import RPi.GPIO as GPIO
import imu
import control

#define controll constants
m = 1.26 #mass
g = 9.81 #gravity
length = 0.2
Lamda = 1
Ix = 4.856*10**-2
Iy = Ix
Iz = 8.801*10**-2
l = 0.2
b =2.98* (10**-6) #change thrust factor
b1 = l / Ix
b2 = b1
b3 = b1
J_r = 3.357*10**-5 
d = 1.14*10**-7 #constant from paper
elapsed_time =0
current_time = 0
previous_time = 0
ph = 0 # pitch
theta = 0 #roll
psi= 0 # yaw
w_d = 0#initially
states = 6*[0]
Z_des = 0
PHI_des = 0
THETA_des = 0
PSI_des = 0
motor_angles_old = 4*[0]



prev_time = time.time() * 1000
curr_time = prev_time
elapsed_time= 0

#the bldc motors must take signal in between 5% duty cycle <--1000us(min) - 2000us(max) -->(10% duty cycle) for T_on
front_pin = 18
right_pin = 23
back_pin = 24
left_pin = 25

#Set the type of labling used for the pins
GPIO.setmode(GPIO.BCM)

GPIO.setup(front_pin, GPIO.OUT)
GPIO.setup(right_pin, GPIO.OUT)
GPIO.setup(back_pin, GPIO.OUT)
GPIO.setup(left_pin, GPIO.OUT)

#frequency of the BLDC 
frequency = 50 #HZ

#define 4 PWM PINs and set frequency
front_bldc = GPIO.PWM(front_pin, frequency) #cw
right_bldc = GPIO.PWM(right_pin, frequency) #ccw
back_bldc  = GPIO.PWM(back_pin, frequency) #cw
left_bldc  = GPIO.PWM(left_pin, frequency) #ccw

#duty --> [0, 100]
#BLDC_duty --> [5, 10]%
def map_duty_for_BLDC(duty):
    BLDC_duty = 5 + 0.05 * duty
    return BLDC_duty

def get_dutycycle_from_w(W):
    #conversion
    #voltage = w / 2000
    #voltage_max 11.1
    #voltage_min 0
    #dutycycle = (voltage/11.1) * 100
    Duty_cycles = [ ( ( (w/2000) / 11.1) * 100 ) for w in W]
    return Duty_cycles

#initalize the pins
front_bldc.start(map_duty_for_BLDC(0))
right_bldc.start(map_duty_for_BLDC(0))
back_bldc.start(map_duty_for_BLDC(0))
left_bldc.start(map_duty_for_BLDC(0))
time.sleep(3)

while True:
    curr_time = time.time() * 1000
    elapsed_time = curr_time - prev_time
    print("elabsed time ", elapsed_time)

    try:
        #do stff here
        #read imu (states)
        
        #calculate control U
        control_input = control.get_U(states, Z_des, PHI_des, THETA_des, PSI_des, w_d, m, Lamda )
        #get W
        motor_angles = control.get_W(control_input, motor_angles_old, b, d)
        #get_wd
        wd = control.get_wd(motor_angles)

        #write w on motor 
        #calc duty cycle
        #converse duty cycle
        motor_angles_old = motor_angles

    #in case of interrupts
    except KeyboardInterrupt:
        GPIO.output(front_pin, GPIO.LOW)
        GPIO.cleanup()
     
    prev_time = curr_time


    
    
