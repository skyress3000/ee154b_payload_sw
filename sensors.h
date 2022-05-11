#include <SparkFunBME280.h>
#include <SparkFunLSM9DS1.h>


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
    float get_current();
private:
    const float current_slope = 0.17, current_offset = 2.5;
    BME280 atm_sensor;
    LSM9DS1 imu;
    bool initialized = false;
};
