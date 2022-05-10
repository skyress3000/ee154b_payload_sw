#include "radio.h"
#include <Arduino.h>

command_t command_list[] = {
    {
        EXAMPLE_COMMAND,
        run_example_command
    }
};

int n_commands = sizeof(command_list)/sizeof(command_t);

void read_radio() {
    if(Serial1.available()) {
        int tmp_serial_in; // used for checking for timeout

        // first 2 bytes should be command and arg length
        uint8_t cmd = Serial1.read();
        tmp_serial_in = Serial1.read()
        if(tmp_serial_in == -1) {
            // timed out when we should've received data
            Serial1.write(ERR_BYTE);
            // don't keep trying
            return;
        }
        uint8_t args_len = (uint8_t) tmp_serial_in;
        // buffer to put argument bytes into
        uint8_t[256] args;
        // read in the argument bytes
        for(int i = 0; i < args_len; i++) {
            tmp_serial_in = Serial1.read()
            if(tmp_serial_in == -1) {
                // timed out when we should've received data
                Serial1.write(ERR_BYTE);
                // don't keep trying
                return;
            }
            args[i] = (uint8_t) tmp_serial_in;
        }
        // received everything successfully
        // we don't do any error checking here because the radios already have error checking built in
        Serial1.write(ACK_BYTE);

        // response from the command
        uint8_t[256] resp;
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
            Serial1.write(DONE_BYTE);
            // send the response back once done
            Serial1.write(resp, resp_len);
        } else {
            // got an invalid command
            Serial1.write(ERR_BYTE);
        }
    }
}