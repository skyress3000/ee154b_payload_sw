#include "radio.h"

#include <Arduino.h>

#include "config.h"
#include "indicators.h"

command_t command_list[] = {
    { CMD_ECHO, &cmd_echo },
    { CMD_PAYLOAD, &cmd_payload },
    { CMD_REBOOT, &cmd_reboot },
    { CMD_OPEN_HATCH, &cmd_open_hatch },
};

int n_commands = sizeof(command_list)/sizeof(command_t);

void read_radio() {
    if(SERIAL_RADIO.available()) {
        int tmp_serial_in; // used for checking for timeout

        // first 2 bytes should be command and arg length
        uint8_t cmd = SERIAL_RADIO.read();
        tmp_serial_in = SERIAL_RADIO.read();
        if(tmp_serial_in == -1) {
            // timed out when we should've received data
            SERIAL_RADIO.write(ERR_BYTE);
            // don't keep trying
            return;
        }
        uint8_t args_len = (uint8_t) tmp_serial_in;
        // buffer to put argument bytes into
        uint8_t args[256];
        // read in the argument bytes
        for(int i = 0; i < args_len; i++) {
            tmp_serial_in = SERIAL_RADIO.read();
            if(tmp_serial_in == -1) {
                // timed out when we should've received data
                SERIAL_RADIO.write(ERR_BYTE);
                // don't keep trying
                return;
            }
            args[i] = (uint8_t) tmp_serial_in;
        }
        // received everything successfully
        // we don't do any error checking here because the radios already have error checking built in
        SERIAL_RADIO.write(ACK_BYTE);

        blink_led(PIN_LED_RF);
        blink_led(PIN_LED_RF);
        blink_led(PIN_LED_RF);

        // response from the command
        uint8_t resp[256];
        uint8_t resp_len;
        // if we've found it, to make sure we do
        int found_cmd = 0;

        // find the command
        for(int cmd_i = 0; cmd_i < n_commands; cmd_i++) {
            if(command_list[cmd_i].command_byte == cmd) {
                // found it, run the command
                found_cmd = 1;
                resp_len = command_list[cmd_i].run_command(args, args_len, resp);
            }
        }

        if(found_cmd) {
            // indicate command finished
            SERIAL_RADIO.write(DONE_BYTE);
            // send the response back once done
            SERIAL_RADIO.write(resp, resp_len);
        } else {
            // got an invalid command
            SERIAL_RADIO.write(ERR_BYTE);
        }
    }
}