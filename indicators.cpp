#include "indicators.h"

#include "config.h"


void indicators_init() {
    pinMode(PIN_LED_SENSORS, OUTPUT);
    pinMode(PIN_LED_RF, OUTPUT);
    pinMode(PIN_LED_LKM, OUTPUT);
    pinMode(PIN_LED_SD, OUTPUT);
    pinMode(PIN_LED_GPS, OUTPUT);
}


void set_led(unsigned int pin) {
    digitalWrite(pin, HIGH);
}


void unset_led(unsigned int pin) {
    digitalWrite(pin, LOW);
}


void blink_led(unsigned int pin) {
    digitalWrite(pin, HIGH);
    delay(BLINK_TIME);
    digitalWrite(pin, LOW);
    delay(BLINK_TIME);
}