#include "gps.h"

#include "config.h"
#include "indicators.h"


void GPS::init() {
    SERIAL_GPS.begin(9600);
}


void GPS::update(unsigned long ms) {
    unsigned long start = millis();
    while (millis() - start < ms) {
        while (SERIAL_GPS.available()) {
            gps.encode(SERIAL_GPS.read());
        }
    }
}


void GPS::check_location_valid() {
    if (gps.location.isValid()) {
        blink_led(PIN_LED_GPS);
        blink_led(PIN_LED_GPS);
        blink_led(PIN_LED_GPS);
    } else {
        blink_led(PIN_LED_GPS);
    }
}


float GPS::get_lat() {
    return gps.location.lat();
}


float GPS::get_lng() {
    return gps.location.lng();
}


float GPS::get_alt() {
    return gps.altitude.meters();
}


String GPS::get_timestamp() {
    TinyGPSDate d = gps.date;
    TinyGPSTime t = gps.time;

    String date_str =  String(d.year()) + "-" + String(d.month()) + "-" + String(d.day());
    String time_str = String(t.hour()) + ":" + String(t.minute()) + ":" + String(t.second());

    return date_str + time_str;
}