// INTEGRAL WINDUP!

#include "argus_core.h"

#include <iostream>
#include <cmath>

const int center_pwm = 1365; // and/or 1355 tested center with mirror ds tape attached 10-15*x*x
const int pwm_min = 520;
const int pwm_max = 2520;

const float proportional_gain = 0.12f; 
const float integral_gain = 0.011f;     
const float derivative_gain = 0.045f; 

void CoreControl(ControlState& ctrl, float target_x, float target_y, float laser_x, float laser_y, double dt, float& output_x, float& output_y) {

    float error_x = target_x - laser_x;
    float error_y = target_y - laser_y;

    if (std::abs(error_x) < 3.0f) error_x = 0.0f;
    if (std::abs(error_y) < 3.0f) error_y = 0.0f;

    float p_x = proportional_gain * error_x;
    ctrl.integral_x += (error_x * dt);
    float i_x = integral_gain * ctrl.integral_x;
    float d_x = 0.0f;
    if (dt > 0.0) {
        d_x = derivative_gain * ((error_x - ctrl.prev_error_x) / dt);
    }

    float p_y = proportional_gain * error_y;
    ctrl.integral_y += (error_y * dt);
    float i_y = integral_gain * ctrl.integral_y;
    float d_y = 0.0f;
    if (dt > 0.0) {
        d_y = derivative_gain * ((error_y - ctrl.prev_error_y) / dt);
    }

    output_x = p_x + i_x + d_x;
    output_y = p_y + i_y + d_y;

    ctrl.pwm_x += output_x;
    ctrl.prev_error_x = error_x;

    ctrl.pwm_y += output_y;
    ctrl.prev_error_y = error_y;

    if (ctrl.pwm_x > pwm_max) ctrl.pwm_x = pwm_max;
    if (ctrl.pwm_x < pwm_min) ctrl.pwm_x = pwm_min;
    if (ctrl.pwm_y > pwm_max) ctrl.pwm_y = pwm_max;
    if (ctrl.pwm_y < pwm_min) ctrl.pwm_y = pwm_min;
}
