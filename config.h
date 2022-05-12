#include <Arduino.h>


// Pins
#define PIN_PAYLOAD_POWER 5
#define PIN_BURN_WIRE 6
#define PIN_HEATER 7
#define PIN_THERMISTOR_VCC 32
#define PIN_THERMISTOR A0
#define PIN_CURRENT A1
#define PIN_SD_CS 10
#define PIN_LED_LKM 45
#define PIN_LED_GPS 47
#define PIN_LED_SENSORS 49
#define PIN_LED_SD 51
#define PIN_LED_RF 53

#define SERIAL_RADIO Serial1
#define SERIAL_PAYLOAD Serial2
#define SERIAL_GPS Serial3

#define BLINK_TIME 100 // ms

#define LOG_FILENAME "log.csv"

#define RF_TX_INTERVAL 30 // approx number of seconds between RF transmissions

// Payload config
#define N_ATTEMPTS 5
#define PAYLOAD_TIMEOUT 500
#define PAYLOAD_BOOT_TIME 5000

#define HEATER_SETPOINT 0.0 // deg C

// Hatch config
#define HATCH_OPEN_ALT 100 // feet
#define HATCH_OPEN_TIME 15 // seconds
#define HATCH_OPEN_READINGS 5 // number of readings above HATCH_OPEN_ALT before opening

// Sensors config
#define ALT_READINGS_DISCARD 5 // number of initial readings to discard
#define ALT_BASELINE_PTS 5 // number of points to average for baseline

// Radio commands
#define CMD_ECHO 0x03
#define CMD_PAYLOAD 0x0C
#define CMD_PAYLOAD_REBOOT 0x30
#define CMD_OPEN_HATCH 0xC0

uint8_t cmd_echo(uint8_t *args, uint8_t args_len, uint8_t* resp_buf);
uint8_t cmd_payload(uint8_t *args, uint8_t args_len, uint8_t* resp_buf);
uint8_t cmd_payload_reboot(uint8_t *args, uint8_t args_len, uint8_t* resp_buf);
uint8_t cmd_open_hatch(uint8_t *args, uint8_t args_len, uint8_t* resp_buf);