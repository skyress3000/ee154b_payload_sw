#include <Arduino.h>
#include <SPI.h>
#include <SD.h>


class Logger {
public:
    void init();
    void write_header(String header);
    void write_item(String item);
    void end_line();
};