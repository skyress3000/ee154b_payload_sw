#include <SparkFunBME280.h>
#include <SparkFunLSM9DS1.h>

#include "config.h"


struct Vector3 {
    float x, y, z;
};

class Sensors {
public:
    void init();
    void update();
    Vector3 get_accel();
    Vector3 get_gyro();
    float get_temp();
    float get_pressure();
    float get_alt();
    bool get_alt_calibrated();
    float get_current();
private:
    const float current_slope = 0.05; // volts / amp
    const float current_offset = 1.25; // volts
    BME280 atm_sensor;
    LSM9DS1 imu;
    bool initialized = false;

    bool alt_calibrated = false;
    float baseline_pts[ALT_BASELINE_PTS];
    unsigned int alt_pts = 0;
    float alt_baseline = 0;
};
