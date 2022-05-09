#include <Arduino.h>


// Pins
#define PIN_PAYLOAD_POWER 5
#define PIN_BURN_WIRE 6
#define PIN_HEATER 7
#define PIN_SD_CS 10
#define PIN_LED_ALL_GOOD 9
#define PIN_LED_RF 10
#define PIN_LED_LKM 11
#define PIN_LED_SD 12
#define PIN_LED_POWER 13
#define PIN_LED_GPS 0 // TODO: update

#define SERIAL_RADIO Serial1
#define SERIAL_PAYLOAD Serial2
#define SERIAL_GPS Serial3

// Payload config
#define N_ATTEMPTS 5
#define PAYLOAD_TIMEOUT 500

#define LOG_FILENAME "log.csv"

#define BLINK_TIME 10 // ms