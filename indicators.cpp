#include "indicators.h"

#include "config.h"

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