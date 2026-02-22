#include <iostream>

const int center_pwm = 1520;
const int pwm_min = 520;
const int pwm_max = 2520;
const int pwm_total = pwm_max - pwm_min;

const int camera_width = 640; //// !!!!!DONT FORGET BENCHMARK TEST 320 AND 1280!!!!!
const int target_x_tolerance = 20; // +- %3.125

const int camera_length = 400;
const int target_y_tolerance = 20; // +- %5

struct Target {
    int x, y;
    int distance;
    double timestamp;
    bool y_islocked;
    bool x_islocked;
    bool islocked;
    int pwm;
    int pwm_x;
    int pwm_y;
};

void updateTarget(Target& t) {

    if (t.x > camera_width) t.x = camera_width;  // !!!!!DONT FORGET BENCHMARK TEST 320 AND 1280!!!!!
    if (t.x < 0) t.x = 0;
    t.pwm_x = pwm_min + (t.x * pwm_total / camera_width);

    if (t.y > camera_length) t.y = camera_length;
    if (t.y < 0) t.y = 0;
    t.pwm_y = pwm_min + (t.y * pwm_total / camera_length);

    if (t.x > 300 && t.x < 340) t.x_islocked = true;
    else { t.x_islocked = false; }

    if (t.y > 180 && t.y < 220) t.y_islocked = true;
    else t.y_islocked = false; 

    t.islocked = (t.y_islocked && t.x_islocked);
}

int main()
{
    Target Target1{ 100, 100, 25, 25.50, false, false, false, 520, 0, 0 };

    while (Target1.x < camera_width && Target1.y < camera_length)
    {
        updateTarget(Target1);
        std::cout << '\n' << "X: " << Target1.x << '\n' << "Y: " << Target1.y << '\n' << "PWM_X:" << Target1.pwm_x << " us" << '\n' << "PWM Y:" << Target1.pwm_y << " us" << '\n' << "--------";
        if (Target1.islocked) std::cout << " [ LOCKED ] --------";
        Target1.x = Target1.x + 20;
        Target1.y = Target1.y + 10;
    }

    std::cout << '\n';
    std::cout << '\n' << "--- COMPLETE ---" << '\n';
    return 0;
}
