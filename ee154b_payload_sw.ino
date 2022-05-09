#include "payload.h"


Payload payload(2400);


void setup() {
    Serial.begin(9600);
    payload.reboot();
}


void loop() {
    payload.update();
    delay(1000);    // TODO: replace with GPS smart delay
}