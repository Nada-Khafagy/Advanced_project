from mpu6050 import mpu6050
import math
import time
import kalman

# Initialize MPU6050
sensor = mpu6050(0x68)  # Use the correct I2C address (0x68 or 0x69) for your MPU6050

# Complementary filter constants
alpha = 0.98  # Weight for accelerometer data (1 - alpha for gyroscope data)

def read_accel_gyro_data():
    accel_data = sensor.get_accel_data()
    return accel_data, gyro_data

def calculate_angles(accel_data, gyro_data, dt, last_angles):
    # Accelerometer data
    accel_pitch = math.atan2(-accel_data['x'], math.sqrt(accel_data['y']**2 + accel_data['z']**2)) * 180 / math.pi
    accel_roll = math.atan2(accel_data['y'], accel_data['z']) * 180 / math.pi

    # Gyroscope data (angular rate)
    gyro_rate_x = gyro_data['x']
    gyro_rate_y = gyro_data['y']
    gyro_rate_z = gyro_data['z']

    # Complementary filter to combine accelerometer and gyroscope data
    pitch = alpha * (last_angles[0] + gyro_rate_x * dt) + (1 - alpha) * accel_pitch
    roll = alpha * (last_angles[1] + gyro_rate_y * dt) + (1 - alpha) * accel_roll

    # Yaw calculation using gyroscope data directly (unaffected by accelerometer)
    yaw = last_angles[2] + gyro_rate_z * dt
    

    return pitch, roll, yaw,

# Main loop
try:
    previous_time = time.time()
    last_angles = [0, 0, 0]  # Initial angles

    while True:
        # Read accelerometer and gyroscope data
        accel_data, gyro_data = read_accel_gyro_data()

        # Calculate time difference (dt) since the last reading
        current_time = time.time()
        dt = current_time - previous_time
        previous_time = current_time

        # Calculate pitch, roll, and yaw angles using the complementary filter
        pitch, roll, yaw = calculate_angles(accel_data, gyro_data, dt, last_angles)

        # Print the results
        print(f"Pitch: {pitch:.2f} degrees, Roll: {roll:.2f} degrees, Yaw: {yaw:.2f} degrees")

        # Update last angles for the next iteration
        last_angles = [pitch, roll, yaw]

        # Pause for a short time before the next reading
        time.sleep(0.1)

except KeyboardInterrupt:
    pass