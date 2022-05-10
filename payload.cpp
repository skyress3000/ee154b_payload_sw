#include "payload.h"

#include "config.h"
#include "indicators.h"


void Payload::reboot() {
    pinMode(PIN_PAYLOAD_POWER, OUTPUT);

    Serial.println("rebooting payload");

    digitalWrite(PIN_PAYLOAD_POWER, LOW);
    delay(1000);
    digitalWrite(PIN_PAYLOAD_POWER, HIGH);

    // SERIAL_PAYLOAD.end();
    SERIAL_PAYLOAD.begin(9600);

    state = PayloadState::BOOTING;
    set_led(PIN_LED_LKM);
}


void Payload::update() {
    switch (state) {
        case PayloadState::BOOTING:
            if (SERIAL_PAYLOAD.available() >= 12 + 30 + 5) { // number of chars in initialization message
                Serial.println("payload done booting");

                // Request new baudrate
                SERIAL_PAYLOAD.print("$DATA,");
                SERIAL_PAYLOAD.print(baudrate);
                SERIAL_PAYLOAD.print(";");
                SERIAL_PAYLOAD.flush();

                // Wait for request to send, then re-open serial connection with new baudrate
                delay(1000);
                SERIAL_PAYLOAD.end();
                SERIAL_PAYLOAD.begin(baudrate);
                SERIAL_PAYLOAD.setTimeout(PAYLOAD_TIMEOUT);
                delay(1000);

                Serial.print("payload ready at ");
                Serial.print(baudrate);
                Serial.println(" baud");

                state = PayloadState::WORKING;
                unset_led(PIN_LED_LKM);
            }
            break;

        default:
            status_str = get_response("$STAT,;");
            if (status_str.length() == 0) {
                state = PayloadState::UNRESPONSIVE;
                blink_led(PIN_LED_LKM);
            } else {
                state = PayloadState::WORKING;
                blink_led(PIN_LED_LKM);
                blink_led(PIN_LED_LKM);
            }
            break;
    }
}


String Payload::get_response(String cmd) {
    String attempts[N_ATTEMPTS];

    for (unsigned int i = 0; i < N_ATTEMPTS; i++) {
        write(cmd);
        attempts[i] = read();
    }

    unsigned int expected_len = attempts[0].length();

    String ret;
    for (unsigned int i = 0; i < expected_len; i++) {
        char chars[N_ATTEMPTS];
        unsigned int count[N_ATTEMPTS];

        // Get each character and a count of frequency
        for (unsigned int j = 0; j < N_ATTEMPTS; j++) {
            char ch = attempts[j][i];
            chars[j] = ch;

            count[j] = 0;
            for (unsigned int k = 0; k < N_ATTEMPTS; k++) {
                if (attempts[k][i] == ch) count[j]++;
            }
        }

        // Find most frequent character
        char best_char = 0;
        unsigned int best_count = 0;
        for (unsigned int j = 0; j < N_ATTEMPTS; j++) {
            if (count[j] > best_count) {
                best_char = chars[j];
                best_count = count[j];
            }
        }

        ret += best_char;
    }

    return ret;
}


String Payload::get_status() {
    return status_str;
}


unsigned int Payload::write(String cmd) {
    return SERIAL_PAYLOAD.write(cmd.c_str());
}


String Payload::read() {
    return SERIAL_PAYLOAD.readStringUntil('\n');
}