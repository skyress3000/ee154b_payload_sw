class Heater {
public:
    void update();
    float get_temp();
    unsigned int get_pwm();
private:
    const float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
    const float res1 = 10000;
    float temp;
    unsigned int pwm;
};