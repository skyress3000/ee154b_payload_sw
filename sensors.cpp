#include "sensors.h"

#include <Wire.h>

#include "config.h"
#include "indicators.h"



#define LSM9DS1_M  0x1C // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG 0x6A // Would be 0x6A if SDO_AG is LOW


void Sensors::init() {
    Wire.begin();
    initialized = atm_sensor.beginI2C() && imu.begin();
}


void Sensors::update() {
    if (imu.accelAvailable()) imu.readAccel();
    if (imu.gyroAvailable()) imu.readGyro();

    if (!alt_calibrated) {
        if (alt_pts >= ALT_READINGS_DISCARD) {
            if (alt_pts - ALT_READINGS_DISCARD < ALT_BASELINE_PTS) {
                baseline_pts[alt_pts - ALT_READINGS_DISCARD] = atm_sensor.readFloatAltitudeFeet();
            } else {
                for (unsigned int i = 0; i < ALT_BASELINE_PTS; i++) {
                    alt_baseline += baseline_pts[i];
                }
                alt_baseline /= (float)ALT_BASELINE_PTS;
                alt_calibrated = true;
            }
        }
        alt_pts++;
    }

    blink_led(PIN_LED_SENSORS);
    if (initialized) blink_led(PIN_LED_SENSORS);
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

float Sensors::get_alt() {
    return atm_sensor.readFloatAltitudeFeet() - alt_baseline;
}

bool Sensors::get_alt_calibrated() {
    return alt_calibrated;
}


float Sensors::get_current() {
    float voltage = (float)analogRead(PIN_CURRENT) * 3.3 / 1023.0;
    return -(voltage - current_offset) / current_slope;
    // return voltage;
}
