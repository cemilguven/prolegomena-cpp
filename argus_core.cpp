#include <iostream>

const int center_pwm = 1520;
const int pwm_min = 520;
const int pwm_max = 2520;
const int pwm_total = pwm_max - pwm_min;
const int camera_width = 640; //// !!!!!DONT FORGET BENCHMARK TEST 320 AND 1280!!!!!
const int target_tolerance = 20; // +-

struct Target {
    int x, y;
    int distance;
    double timestamp;
    bool islocked;
    int pwm;
};

void updateTarget(Target& t) {

    if (t.x > camera_width) t.x = camera_width;  // !!!!!DONT FORGET BENCHMARK TEST 320 AND 1280!!!!!
    if (t.x < 0) t.x = 0;
    t.pwm = pwm_min + (t.x * pwm_total / camera_width);

    if (t.x > 300 && t.x < 340) { 
        t.islocked = true;
    }

    else {
        t.islocked = false;
    }
}

int main()
{
    std::cout << "Test" << '\n';
    Target Target1{ 100, 150, 25, 25.50, false, 1520 };

    while (Target1.x < camera_width)
    {
        updateTarget(Target1);
        std::cout << '\n' << "X: " << Target1.x << '\n' << "PWM: " << Target1.pwm << " us" << '\n' << "--------";
        if (Target1.islocked) std::cout << " [ LOCKED ] --------";
        Target1.x = Target1.x + 10;
    }
    std::cout << '\n';
    std::cout << '\n' << "--- COMPLETE ---" << '\n';
    return 0;
}











/* void updateTarget(Target* t) { t->x = t->x + 10; std::cout << "Coordinates:" << '\n'; std::cout << t->x << '\n' << t->y; 
Target Target1;Target1.x = 100;Target1.y = 150;Target1.distance = 25;Target1.timestamp = 12.25;Target1.islocked = true;*/
