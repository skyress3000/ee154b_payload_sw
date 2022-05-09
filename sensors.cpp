#include "sensors.h"

#include <Wire.h>

#include "config.h"


void Sensors::init() {
    Wire.begin();
    atm_sensor.beginI2C();
    imu.begin();
}


void Sensors::update() {
    if (imu.accelAvailable()) imu.readAccel();
    if (imu.gyroAvailable()) imu.readGyro();
}


Vector3 Sensors::get_accel() {
    return { imu.calcAccel(imu.ax), imu.calcAccel(imu.ay), imu.calcAccel(imu.az) };
}


Vector3 Sensors::get_gyro() {
    return { imu.calcGyro(imu.gx), imu.calcGyro(imu.gy), imu.calcGyro(imu.gz) };
}


float Sensors::get_temp() {
    return atm_sensor.readTempC();
}


float Sensors::get_pressure() {
    return atm_sensor.readFloatPressure();
}


float Sensors::get_current() {
    float voltage = (float)analogRead(PIN_CURRENT) / 1023.0;
    return (voltage - current_offset) / current_slope;
}