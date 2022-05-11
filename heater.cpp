#include "heater.h"

#include "config.h"


void Heater::init() {
    pinMode(PIN_HEATER, OUTPUT);
}


void Heater::update() {
    unsigned int voltage = analogRead(PIN_THERMISTOR);
    float res2 = res1 * (1023.0 / (float)voltage - 1.0);
    float log_res2 = log(res2);
    temp = (1.0 / (c1 + c2*log_res2 + c3*log_res2*log_res2*log_res2)) - 273.15;

    if (temp > HEATER_MARGIN) pwm = 0;
    if (temp < -HEATER_MARGIN) pwm = 255;

    analogWrite(PIN_HEATER, pwm);
}


float Heater::get_temp() {
    return temp;
}


unsigned int Heater::get_pwm() {
    return pwm;
}
