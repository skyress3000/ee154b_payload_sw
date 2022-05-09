#include <TinyGPSPlus.h>


class GPS {
public:
    void init();
    void smart_delay(unsigned long ms);
    void check_location_valid();
    float get_lat();
    float get_lng();
    float get_alt();
    String get_timestamp();
private:
    TinyGPSPlus gps;
};