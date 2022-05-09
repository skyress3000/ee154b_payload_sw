#include "logger.h"

#include "config.h"
#include "indicators.h"


void Logger::init() {
    SD.begin(PIN_SD_CS);
}


void Logger::write_header(String header) {
    File f = SD.open(LOG_FILENAME, FILE_WRITE);
    if (f.println(header.c_str())) blink_led(PIN_LED_SD);
    f.close();
}


void Logger::write_item(String item) {
    File f = SD.open(LOG_FILENAME, FILE_WRITE);
    if (f.print(item.c_str()) && f.print(",")) blink_led(PIN_LED_SD);
    f.close();
}


void Logger::end_line() {
    File f = SD.open(LOG_FILENAME, FILE_WRITE);
    if (f.print("\n")) blink_led(PIN_LED_SD);
    f.close();
}