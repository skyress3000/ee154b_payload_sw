class Hatch {
public:
    void init();
    void open();
    void update(float altitude);
private:
    unsigned long open_start_time;
    bool is_open = false;
    unsigned int should_open = 0;
};