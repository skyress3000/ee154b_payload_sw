#include <Arduino.h>


// Pins
#define PIN_PAYLOAD_POWER 5
#define PIN_BURN_WIRE 6
#define PIN_HEATER 7
#define PIN_THERMISTOR A1
#define PIN_CURRENT A0
#define PIN_SD_CS 10
#define PIN_LED_SENSORS 45
#define PIN_LED_RF 47
#define PIN_LED_LKM 49
#define PIN_LED_SD 51
#define PIN_LED_GPS 53

#define SERIAL_RADIO Serial1
#define SERIAL_PAYLOAD Serial2
#define SERIAL_GPS Serial3

// Payload config
#define N_ATTEMPTS 5
#define PAYLOAD_TIMEOUT 500

#define LOG_FILENAME "log.csv"

#define BLINK_TIME 50 // ms

#define HEATER_MARGIN 2 // hysteresis (deg C)