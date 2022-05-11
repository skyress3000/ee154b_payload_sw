#include "config.h"
#include "indicators.h"
#include "payload.h"
#include "gps.h"
#include "logger.h"
#include "heater.h"
#include "sensors.h"
#include "hatch.h"
#include "radio.h"


Payload payload(2400);
GPS gps;
Logger logger;
Heater heater;
Sensors sensors;
Hatch hatch;


void setup() {
    indicators_init();
    set_led(PIN_LED_SENSORS);
    set_led(PIN_LED_RF);
    set_led(PIN_LED_LKM);
    set_led(PIN_LED_SD);
    set_led(PIN_LED_GPS);

    Serial.begin(9600);
    Serial.println("beginning boot");
    gps.init();
    logger.init();
    sensors.init();
    hatch.init();
    SERIAL_RADIO.begin(57600);

    delay(1000);
    Serial.println("boot done");

    unset_led(PIN_LED_SENSORS);
    unset_led(PIN_LED_RF);
    unset_led(PIN_LED_LKM);
    unset_led(PIN_LED_SD);
    unset_led(PIN_LED_GPS);

    delay(100);

    payload.reboot();
    logger.write_header(
        "timestamp,lat,lng,altitude,"
        "temp_int,temp_ext,pressure,current,"
        "accel_x,accel_y,accel_z,rot_x,rot_y,rot_z,"
        "heater,payload_status"
    );
}


void loop() {
    read_radio();

    // Update everything
    payload.update();
    gps.update(1000);
    heater.update();
    sensors.update();
    hatch.update();

    if (gps.get_alt() >= HATCH_OPEN_ALT) hatch.open();

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

    // Transmit to ground
    SERIAL_RADIO.write(STAT_BYTE);
    SERIAL_RADIO.print(gps.get_lat()); SERIAL_RADIO.print(",");
    SERIAL_RADIO.print(gps.get_lng()); SERIAL_RADIO.print(",");
    SERIAL_RADIO.print(gps.get_alt()); SERIAL_RADIO.print(",");
    SERIAL_RADIO.print(sensors.get_current()); SERIAL_RADIO.print(",");
    SERIAL_RADIO.print(heater.get_temp()); SERIAL_RADIO.print(",");
    SERIAL_RADIO.print(payload.get_status());
    SERIAL_RADIO.print('\n');
    blink_led(PIN_LED_RF);
}


// Radio command implementations
uint8_t cmd_echo(uint8_t *args, uint8_t args_len, uint8_t* resp_buf) {
    for (uint8_t i = 0; i < args_len; i++) {
        resp_buf[i] = args[i];
    }

    return args_len;
}


uint8_t cmd_payload(uint8_t *args, uint8_t args_len, uint8_t* resp_buf) {
    String payload_cmd;
    for (uint8_t i = 0; i < args_len; i++) {
        payload_cmd += args[i];
    }

    String resp = payload.get_response(payload_cmd);
    resp.toCharArray((char *)resp_buf, 256);

    return resp.length();
}


uint8_t cmd_reboot(uint8_t *args, uint8_t args_len, uint8_t* resp_buf) {
    payload.reboot();
    return 0;
}


uint8_t cmd_open_hatch(uint8_t *args, uint8_t args_len, uint8_t* resp_buf) {
    hatch.open();
    return 0;
}