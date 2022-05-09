#include "config.h"
#include "indicators.h"
#include "payload.h"
#include "gps.h"
#include "logger.h"


Payload payload(2400);
GPS gps;
Logger logger;


void setup() {
    set_led(PIN_LED_ALL_GOOD);
    set_led(PIN_LED_RF);
    set_led(PIN_LED_LKM);
    set_led(PIN_LED_SD);
    set_led(PIN_LED_POWER);
    set_led(PIN_LED_GPS);

    Serial.begin(9600);
    gps.init();
    logger.init();

    delay(1000);

    unset_led(PIN_LED_ALL_GOOD);
    unset_led(PIN_LED_RF);
    unset_led(PIN_LED_LKM);
    unset_led(PIN_LED_SD);
    unset_led(PIN_LED_POWER);
    unset_led(PIN_LED_GPS);

    delay(100);

    payload.reboot();
}


void loop() {
    payload.update();
    gps.smart_delay(1000);    
}