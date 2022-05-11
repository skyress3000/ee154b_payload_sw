#include "hatch.h"

#include "config.h"


void Hatch::init() {
    pinMode(PIN_BURN_WIRE, OUTPUT);
    digitalWrite(PIN_BURN_WIRE, LOW);
}


void Hatch::open() {
    if (!is_open) {
        open_start_time = millis();
        analogWrite(PIN_BURN_WIRE, 25);
        is_open = true;
    }
}


void Hatch::update() {
    if (is_open && (millis() - open_start_time) >= HATCH_OPEN_TIME) {
        digitalWrite(PIN_BURN_WIRE, LOW);
    }
}


bool Hatch::get_open() {
    return is_open;
}