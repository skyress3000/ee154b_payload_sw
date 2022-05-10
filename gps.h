#include <TinyGPSPlus.h>


class GPS {
public:
    void init();
    void update(unsigned long ms);
    float get_lat();
    float get_lng();
    float get_alt();
    String get_timestamp();
private:
    TinyGPSPlus gps;
};