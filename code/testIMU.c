#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

// I2C address of the IMU
#define IMU_ADDRESS 0x68

// Registers for reading data
#define ACCEL_XOUT_H 0x3B
#define GYRO_XOUT_H 0x43

// Function to read 16-bit values from the IMU
int readSensorData(int fd, int reg) {
    int highByte, lowByte;
    highByte = wiringPiI2CReadReg8(fd, reg);
    lowByte = wiringPiI2CReadReg8(fd, reg + 1);
    return (highByte << 8) | lowByte;
}

int main() {
    int imu_fd;
    int accelX, gyroX;

    if (wiringPiSetup() == -1) {
        fprintf(stderr, "Unable to setup WiringPi. Are you running as root?\n");
        return 1;
    }

    imu_fd = wiringPiI2CSetup(IMU_ADDRESS);

    if (imu_fd == -1) {
        fprintf(stderr, "Unable to initialize I2C. Check your connections and permissions.\n");
        return 1;
    }

    while (1) {
        // Read accelerometer and gyroscope data
        accelX = readSensorData(imu_fd, ACCEL_XOUT_H);
        gyroX = readSensorData(imu_fd, GYRO_XOUT_H);

        printf("Accelerometer X: %d, Gyroscope X: %d\n", accelX, gyroX);

        // Adjust the sampling rate as needed
        delay(1000); // Read data every second
    }

    return 0;
}
