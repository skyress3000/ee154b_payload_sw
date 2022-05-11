class Hatch {
public:
    void init();
    void open();
    void update();
    bool get_open();
private:
    unsigned long open_start_time;
    bool is_open = false;
};