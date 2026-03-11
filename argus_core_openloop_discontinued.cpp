// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Cemil Uğurcan Güven

#include <iostream>
#include <chrono>
#include <cmath>
#include <pigpio.h>

const int center_pwm = 1520;
const int pwm_min = 520;
const int pwm_max = 2520;
const int pwm_total = pwm_max - pwm_min;

const int camera_width = 640; 
const int target_x_tolerance = 20;

const int camera_length = 400;
const int target_y_tolerance = 20; // %5

const float proportional_gain = 0.02f; // Kp -- f = float literal
const float integral_gain = 0.00f;  // Ki f -- = float literal
const float derivative_gain = 0.01f; // Kd -- f = float literal

struct Target {
    int x, y;
    int distance;
    double timestamp;
    bool y_islocked;
    bool x_islocked;
    bool islocked;
    float pwm_x = 1520.0f;
    float pwm_y = 1520.0f;
    int setpoint_pwm_x;
    int setpoint_pwm_y;
    float prev_error_x = 0.0f;
    float prev_error_y = 0.0f;
    float integral_x = 0.0f;
    float integral_y = 0.0f;
};

void updateTarget(Target& t, double dt) {

    if (t.x > camera_width) t.x = camera_width;  // !!!!!DONT FORGET BENCHMARK TEST 320 AND 1280!!!!!
    if (t.x < 0) t.x = 0;
    t.setpoint_pwm_x = pwm_min + (t.x * pwm_total / camera_width);

    if (t.y > camera_length) t.y = camera_length;
    if (t.y < 0) t.y = 0;
    t.setpoint_pwm_y = pwm_min + (t.y * pwm_total / camera_length);

    float error_x = t.setpoint_pwm_x - t.pwm_x;

    if (std::abs(error_x) < 3.0f) error_x = 0.0f; // creaating a ***deadband***!?!!?!

    float p_x = proportional_gain * error_x;
    t.integral_x += (error_x * dt);
    float i_x = integral_gain * t.integral_x;

    float d_x = 0.0f;
    if (dt > 0.0) {
        d_x = derivative_gain * ((error_x - t.prev_error_x) / dt);
    }
    t.pwm_x += (p_x + i_x + d_x);
    t.prev_error_x = error_x;

    float error_y = t.setpoint_pwm_y - t.pwm_y;

    if (std::abs(error_y) < 3.0f) error_y = 0.0f;

    float p_y = proportional_gain * error_y;
    t.integral_y += (error_y * dt);
    float i_y = integral_gain * t.integral_y;

    float d_y = 0.0f;
    if (dt > 0.0) {
        d_y = derivative_gain * ((error_y - t.prev_error_y) / dt);
    }
    t.pwm_y += (p_y + i_y + d_y);
    t.prev_error_y = error_y;

    if (t.pwm_x > pwm_max) t.pwm_x = pwm_max;
    if (t.pwm_x < pwm_min) t.pwm_x = pwm_min;
    if (t.pwm_y > pwm_max) t.pwm_y = pwm_max;
    if (t.pwm_y < pwm_min) t.pwm_y = pwm_min;

    if (t.x > 300 && t.x < 340) t.x_islocked = true;
    else { t.x_islocked = false; }

    if (t.y > 180 && t.y < 220) t.y_islocked = true;
    else t.y_islocked = false; 

    t.islocked = (t.y_islocked && t.x_islocked);
}

int main()
{
    Target Target1;

    Target1.x = camera_width / 2;
    Target1.y = camera_length / 2;

    if (gpioInitialise() < 0) {
        std::cerr << "pigpio error test" << '\n';
        return 1;
    }
    auto last_time = std::chrono::steady_clock::now();

    while (Target1.x < camera_width && Target1.y < camera_length)
    {
        auto current_time = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = current_time - last_time;
        double dt = elapsed.count();

        if (dt == 0.0) dt = 0.001; // to prevent division by zero in der calc

        updateTarget(Target1, dt);

        std::cout << '\n' << "X: " << Target1.x << '\n' << "Y: " << Target1.y << '\n' << "PWM_X: " << Target1.pwm_x << " us" << '\n' << "PWM Y: " << Target1.pwm_y << " us" << '\n' << "Elapsed Time: " << dt << " seconds" << '\n' << "--------";
        if (Target1.islocked) std::cout << " [ LOCKED ] --------";
     
        gpioServo(12, (int) Target1.pwm_x);
        gpioServo(13, (int) Target1.pwm_y);

        last_time = current_time;
    }
    gpioTerminate(); //doesnt prevent pigpio initialisation failed error if ctrl c find solution

    std::cout << '\n';
    std::cout << '\n' << "--- COMPLETE ---" << '\n';
    return 0;
}
