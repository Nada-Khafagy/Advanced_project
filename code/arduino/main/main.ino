#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <math.h>
#include <Servo.h>
#include <Arduino.h>
#include <TinyMPU6050.h>


// Constants
const double m = 1.26;      // mass
const double g = 9.81;      // gravity
const double length = 0.2;
const double Lamda = 1;
const double Ix = 4.856e-2;
const double Iy = 4.856e-2;
const double Iz = 8.801e-2;
const double l = 0.2;
const double b = 2.98e-6;    // change thrust factor
const double J_r = 3.357e-5;
const double d = 1.14e-7;     // constant from paper
const float k0 = 2;  
const float k1 = 2;
const float k2 = 2;
const float k3 = 2;    

//connections
#define front_pin  6
#define right_pin  9
#define back_pin  10
#define left_pin  11
#define max_time_on 1200 //in microseconds

// Global variables
float Ts = 0.01; //sampling time
float elapsed_time =0;
float curr_time = 0;
float prev_time = 0;
boolean isRunning = false;

//SMC Control
//phi pitch (y) , theta roll (x), psi yaw (z)
float X[12]; //x,xdot, y, ydot, z, zdot, phi, theta, psi, phidot, thetadot, psidot
float U[4]; //u1, u2, u3, u4
float W[4]; //w1, w2, w3, w4
float wd = 0;
float old_W[4]; //w1, w2, w3, w4
float Duty_cycles[4];

int Z_ref[] = {0,0,0}; //z, zdot, zddot refrence values
int PHI_des[] = {0,0,0}; //phi, phidot, phiddot refrence angels in degree
int THETA_des[] = {0,0,0};
int PSI_des[] = {0,0,0};


// Function declarations
void update_X();
void update_U();
void get_W();
void get_dutycycle_from_w();
void update_motors();
void runge_kutta_4th_order(float *X, float* U);

// create servo object to control the ESC
Servo front_ESC;     
Servo right_ESC;
Servo back_ESC;
Servo left_ESC;

MPU6050 mpu (Wire);

void setup() {
    Serial.begin(9600);
    //the bldc motors must take signal in between 5% duty cycle <--1000us(min) - 2000us(max) -->(10% duty cycle) for T_on
    front_ESC.attach(front_pin,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
    right_ESC.attach(right_pin,1000,2000);
    back_ESC.attach(back_pin,1000,2000);
    left_ESC.attach(left_pin,1000,2000);
    //nullify quadcopter
    front_ESC.write(0);  
    right_ESC.write(0);
    back_ESC.write(0);
    left_ESC.write(0);

    // MPU Initialization
    mpu.Initialize();
    // Calibration
    Serial.begin(9600);
    Serial.println("=====================================");
    Serial.println("Starting calibration...");
    mpu.Calibrate();
    Serial.println("Calibration complete!");
    Serial.println("Offsets:");
    Serial.print("GyroX Offset = ");
    Serial.println(mpu.GetGyroXOffset());
    Serial.print("GyroY Offset = ");
    Serial.println(mpu.GetGyroYOffset());
    Serial.print("GyroZ Offset = ");
    Serial.println(mpu.GetGyroZOffset());
}

void loop() {
if (Serial.available() > 0) {
    char command = Serial.read(); // Read the incoming command from the Serial Monitor
    // Check the command and take action accordingly
    if (command == '1') {
      isRunning = true; // Start the loop
      Serial.println("Loop started");
    } else if (command == '0') {
      isRunning = false; // Stop the loop
      front_ESC.write(0);  
      right_ESC.write(0);
      back_ESC.write(0);
      left_ESC.write(0);
      Serial.println("Loop stopped");
      
    }
  }

  // Execute the loop only if it is running
  if (isRunning) {
    //signal(SIGINT, handle_interrupt);
    update_X();//from sensor
    update_U();
    get_W();
    get_dutycycle_from_w();
    update_motors();
    prev_time = curr_time;
  }
  
    
}


//from the IMU
void update_X()
{
    //x,xdot, y, ydot, z, zdot are zeros cuz the quadcopter is not moving in x,y,z
    X[0] = 0;
    X[1] = 0;
    X[2] = 0;
    X[3] = 0;
    X[4] = 0;
    X[5] = 0;
    curr_time = (unsigned long)(time(NULL) * 1000);
    elapsed_time = curr_time - prev_time;
    mpu.Execute();
    //X = phi, theta, psi, phidot, thetadot, psidot
    //phi pitch (y) , theta roll (x), psi yaw (z)
    X[6] = (mpu.GetAngY())*(3.14/180);
    Serial.print("phi = X[6]:");
    Serial.print(X[6]);
    X[7] = (mpu.GetGyroY())*(3.14/180);
    Serial.print(", phi dot = X[7]:");
    Serial.print(X[7]);
    X[8] = (mpu.GetAngX())*(3.14/180);
    Serial.print(", theta = X[8]:");
    Serial.print(X[8]);
    X[9] = (mpu.GetGyroX())*(3.14/180);
    Serial.print(", theta dot = X[9]:");
    Serial.print(X[9]);
    X[10] = (mpu.GetAngZ())*(3.14/180); 
    Serial.print(", psi = X[10]:");
    Serial.print(X[10]);
    X[11] = (mpu.GetGyroZ())*(3.14/180);
    Serial.print(", psi dot = X[11]:");
    Serial.println(X[11]);

    prev_time = curr_time;
}

//continuous-time
void update_U() {
    float z = X[4];
    float z_dot = X[5];
    float phi = X[6];
    float phi_dot = X[7];
    float theta = X[8];
    float theta_dot = X[9];
    float psi = X[10];
    float psi_dot = X[11];
    
    float z_des = Z_ref[0];
    float z_dot_des = Z_ref[1];
    float z_ddot_des = Z_ref[2];
    float s0 = (z_dot - z_dot_des ) + Lamda * (z - z_des);
    float f0 = -g;
    U[0] = (m / (cos(phi) * cos(theta))) * (-f0  + (Lamda * (z_dot_des - z_dot)) - (k0 * max(min(s0,1),-1)) );
    
    float a1 = (Iy - Iz) / Ix;
    float Phi_des = PHI_des[0];
    float Phi_dot_des = PHI_des[1];
    float Phi_ddot_des = PHI_des[2];
    float s1 = ( phi_dot - Phi_dot_des) + Lamda * (phi - Phi_des) ;
    float f1 = psi_dot * theta_dot * a1 - (J_r / Ix) * theta_dot * wd;
    U[1] = (Ix / l) * (-f1 + Phi_ddot_des + (Lamda * (Phi_dot_des - phi_dot)) - (k1* max(min(s1,1), -1)) );

    float a2 = (Iz - Ix) / Iy;
    float Theta_des = THETA_des[0];
    float Theta_dot_des = THETA_des[1];
    float Theta_ddot_des = THETA_des[2];
    float s2 = (theta_dot - Theta_dot_des) + Lamda * (theta - Theta_des);
    float f2 = psi_dot * phi_dot * a2 + (J_r / Iy) * phi_dot * wd;
    U[2] = (Iy / l) * (-f2 + Theta_ddot_des + (Lamda * (Theta_dot_des - theta_dot)) - (k2* max(min(s2,1), -1))) ;

    float a3 = (Ix - Iy) / Iz;
    float Psi_des = PSI_des[0];
    float Psi_dot_des = PSI_des[1];
    float Psi_ddot_des = PSI_des[2];
    float s3 = (psi_dot - Psi_dot_des ) + Lamda * (psi - Psi_des);
    float f3 = theta_dot * phi_dot * a3;
    U[3] = (Iz / l) * (-f3 + Psi_ddot_des + (Lamda * (Psi_dot_des - psi_dot)) - (k3* max(min(s3,1), -1))) ;

}

//output of controller input to system , assume it is in radian
void get_W() {
    float w1 = (d * U[0] - 2 * d * U[2] - b * U[3]) / (4 * d * b) > 0 ? sqrt((d * U[0] - 2 * d * U[2] - b * U[3]) / (4 * d * b)) : old_W[0];
    float w2 = (d * U[0] - 2 * d * U[1] + b * U[3]) / (4 * d * b) > 0 ? sqrt((d * U[0] - 2 * d * U[1] + b * U[3]) / (4 * d * b)) : old_W[1];
    float w3 = (d * U[0] + 2 * d * U[2] - b * U[3]) / (4 * d * b) > 0 ? sqrt((d * U[0] + 2 * d * U[2] - b * U[3]) / (4 * d * b)) : old_W[2];
    float w4 = (d * U[0] + 2 * d * U[1] + b * U[3]) / (4 * d * b) > 0 ? sqrt((d * U[0] + 2 * d * U[1] + b * U[3]) / (4 * d * b)) : old_W[3];
    W[0] = w1;
    W[1] = w2;
    W[2] = w3;
    W[3] = w4;  
    wd = W[3] + W[1] - W[2] - W[0];
}

void get_dutycycle_from_w() {
    //rpm max when time_on is max(10% duty) is 2442 rpm
    for (int i = 0; i < 4; ++i) {
        Serial.print("W [");
        Serial.print(i);
        Serial.print("]:");
        Serial.println(W[i]);
        float max_rpm = (2200*11.1);
        float max_rad_per_sec = max_rpm * ((2 * 3.14) / 60);
        Duty_cycles[i] = (( W[i] / max_rad_per_sec)) * 100.0;
        if (Duty_cycles[i] > 100) {
            Duty_cycles[i] = 100;
        }
        else if (Duty_cycles[i] < 0) {
            Duty_cycles[i] = 25;
        }
        //Serial.print("Duty_cycles [");
        //Serial.print(i);
        //Serial.print("]:");
        //Serial.println(Duty_cycles[i]);
    }
}
void update_motors() {
    front_ESC.write(min(map(Duty_cycles[0], 0, 100, 0,180),120));  
    right_ESC.write(min(map(Duty_cycles[1], 0, 100, 0, 180),120));
    back_ESC.write(min(map(Duty_cycles[2], 0, 100, 0, 180),120));
    left_ESC.write(min(map(Duty_cycles[3], 0, 100, 0, 180),120));
}

// Implement the 4th order Runge-Kutta integrator
void runge_kutta_4th_order(float *X, float* U) {
    float k1, k2, k3, k4;
    // Compute k1
    //system_dynamics(*X, U, params, &k1);
    k1 *= Ts;
    // Compute k2
    //system_dynamics(*X + 0.5 * k1, U, params, &k2);
    k2 *= Ts;
    // Compute k3
    //system_dynamics(*X + 0.5 * k2, U, params, &k3);
    k3 *= Ts;
    // Compute k4
    //system_dynamics(*X + k3, U, params, &k4);
    k4 *= Ts;
    // Update state using the weighted sum of increments
    *X += (k1 + 2 * k2 + 2 * k3 + k4) / 6.0;
}
