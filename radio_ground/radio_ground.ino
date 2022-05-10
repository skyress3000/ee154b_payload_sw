void setup() {
  Serial.begin(9600);
  Serial1.begin(57600);
}

void loop() {
  // just send from one serial to the other:
  if (Serial.available() > 0) {
    uint8_t inbyte = Serial.read();
    Serial1.write(inbyte);
  }
  if (Serial1.available() > 0) {
    Serial.write(Serial1.read());
  }
}
