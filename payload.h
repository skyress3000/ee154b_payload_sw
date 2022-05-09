#include <Arduino.h>


enum PayloadState {
    BOOTING,
    WORKING,
    UNRESPONSIVE,
    UNKNOWN
};


class Payload {
public:
    Payload(int baudrate) { this->baudrate = baudrate; }
    void reboot();
    void update();  // update state, try to get status if not booting
    String get_response(String cmd); // write/read multiple times and error-correct response
    String get_status();
private:
    unsigned int write(String cmd); // write a command
    String read();  // read a line

    int baudrate;
    PayloadState state = PayloadState::UNKNOWN;
    long boot_time;
    String status_str;
};