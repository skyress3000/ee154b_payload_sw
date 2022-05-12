#include "hatch.h"

#include "config.h"
#include "radio.h"


void Hatch::init() {
    pinMode(PIN_BURN_WIRE, OUTPUT);
    digitalWrite(PIN_BURN_WIRE, LOW);
}


void Hatch::open() {
    if (!is_open) {
        Serial.println("opening hatch");
        open_start_time = millis();
        analogWrite(PIN_BURN_WIRE, 50);
        is_open = true;

        SERIAL_RADIO.write(STAT_BYTE);
        SERIAL_RADIO.print("OPENING HATCH\n");
    }
}


void Hatch::update(float altitude) {
    if (!is_open && altitude >= HATCH_OPEN_ALT) should_open++;

    if (!is_open && should_open >= HATCH_OPEN_READINGS) open();

    if (is_open && ((millis() - open_start_time) / 1000 >= HATCH_OPEN_TIME)) {
        digitalWrite(PIN_BURN_WIRE, LOW);
    }
}