#include "config.h"
#include "indicators.h"
#include "payload.h"
#include "gps.h"
#include "logger.h"
#include "heater.h"
#include "sensors.h"


Payload payload(2400);
GPS gps;
Logger logger;
Heater heater;
Sensors sensors;


void setup() {
    set_led(PIN_LED_SENSORS);
    set_led(PIN_LED_RF);
    set_led(PIN_LED_LKM);
    set_led(PIN_LED_SD);
    set_led(PIN_LED_POWER);
    set_led(PIN_LED_GPS);

    Serial.begin(9600);
    gps.init();
    logger.init();
    sensors.init();

    delay(1000);

    unset_led(PIN_LED_SENSORS);
    unset_led(PIN_LED_RF);
    unset_led(PIN_LED_LKM);
    unset_led(PIN_LED_SD);
    unset_led(PIN_LED_POWER);
    unset_led(PIN_LED_GPS);

    delay(100);

    logger.write_header(
        "timestamp,lat,lng,altitude,"
        "temp_int,temp_ext,pressure,current,"
        "accel_x,accel_y,accel_z,rot_x,rot_y,rot_z,"
        "heater,payload_status"
    );
    payload.reboot();
}


void loop() {
    // TODO: check for and handle incoming command

    // Update everything
    payload.update();
    gps.update(1000);
    heater.update();
    sensors.update();

    // Write to log
    logger.write_item(gps.get_timestamp());
    logger.write_item(String(gps.get_lat()));
    logger.write_item(String(gps.get_lng()));
    logger.write_item(String(gps.get_alt()));

    logger.write_item(String(heater.get_temp()));
    logger.write_item(String(sensors.get_temp()));
    logger.write_item(String(sensors.get_pressure()));
    logger.write_item(String(sensors.get_current()));

    Vector3 accel = sensors.get_accel();
    Vector3 gyro = sensors.get_gyro();
    logger.write_item(String(accel.x));
    logger.write_item(String(accel.y));
    logger.write_item(String(accel.z));
    logger.write_item(String(gyro.x));
    logger.write_item(String(gyro.y));
    logger.write_item(String(gyro.z));

    logger.write_item(String(heater.get_pwm()));
    logger.write_item(String(payload.get_status()));
    logger.end_line();

    // TODO: Transmit to ground
}