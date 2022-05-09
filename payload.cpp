#include "payload.h"


void Payload::reboot() {
    Serial.println("rebooting payload");

    // TODO: pull payload power pin low, wait, then high

    // Serial2.end();
    Serial2.begin(9600);

    state = PayloadState::BOOTING;
}


void Payload::update() {
    switch (state) {
        case PayloadState::BOOTING:
            Serial.print(Serial2.available()); Serial.println(" chars");

            if (Serial2.available() >= 12 + 30 + 5) { // number of chars in initialization message
                Serial.println("payload done booting");

                // Request new baudrate
                Serial2.print("$DATA,");
                Serial2.print(baudrate);
                Serial2.print(";");
                Serial2.flush();

                // Wait for request to send, then re-open serial connection with new baudrate
                delay(1000);
                Serial2.end();
                Serial2.begin(baudrate);
                Serial2.setTimeout(PAYLOAD_TIMEOUT);
                delay(1000);

                Serial.print("payload ready at ");
                Serial.print(baudrate);
                Serial.println(" baud");

                state = PayloadState::WORKING;
            }
            break;

        default:
            status_str = get_response("$STAT,;");
            if (status_str.length() == 0) {
                state = PayloadState::UNRESPONSIVE;
                Serial.println("payload unresponsive");
            } else {
                state = PayloadState::WORKING;
                Serial.print("payload status: ");
                Serial.println(status_str.c_str());
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


unsigned int Payload::write(String cmd) {
    return Serial2.write(cmd.c_str());
}


String Payload::read() {
    return Serial2.readStringUntil('\n');
}