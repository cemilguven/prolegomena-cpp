#pragma once

struct ControlState {

    float pwm_x = 1365.0f;
    float pwm_y = 1365.0f;
    float prev_error_x = 0.0f;
    float prev_error_y = 0.0f;
    float integral_x = 0.0f;
    float integral_y = 0.0f;
};

void CoreControl(ControlState& ctrl, float target_x, float target_y, float laser_x, float laser_y, double dt, float& output_x, float& output_y)
