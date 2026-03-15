//CHECK CODE PID  VALUES 0 ADD TRACKING TRY SQUARE BOX LOCKED TRY OPENCV GAME
//INTEGRAL WINDUP!!!!!!!!!!!!!!!!!!!!!
//SPDXXXXXXX

#include <iostream>
#include <chrono>
#include <cmath>
#include <thread>
#include <fstream>
#include <opencv2/opencv.hpp>

constexpr int crosshair_gap = 2;
constexpr int crosshair_length = 6;
constexpr int hud_offset = 15;

const int center_pwm = 1365; // and/or 1355 tested center with mirror ds tape attached 10-15*x*x
const int pwm_min = 520;
const int pwm_max = 2520;

const float proportional_gain = 0.088f; // Kp -- f = float literal   0.02u0.1u0.01 0.02u0.1u0.01
const float integral_gain = 0.1f;     // Ki -- f = float literal
const float derivative_gain = 0.01f;   // Kd -- f = float literal 

struct TestState {

    float pwm_x = 1365.0f;
    float pwm_y = 1365.0f;
    float prev_error_x = 0.0f;
    float prev_error_y = 0.0f;
    float integral_x = 0.0f;
    float integral_y = 0.0f;
};

void updatePID(TestState& ctrl, float target_x, float target_y, float laser_x, float laser_y, double dt, float& output_x, float& output_y) {

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

void TestControl() {
    TestState ctrl;

    float output_x = 0.0f;
    float output_y = 0.0f;
    float target_x = 450.0f;
    float target_y = 150.0f;
    float laser_x = 50.0f;
    float laser_y = 20.0f;

    std::ofstream data_log("argus_core_sim_telemetry.csv");
    data_log << "Elapsed Time(s),Target_X,Target_Y,Laser_X,Laser_Y,Error_X,Error_Y,PWM_X,PWM_Y" << '\n';

    cv::Mat prep_screen = cv::Mat::zeros(600, 800, CV_8UC3);

    cv::putText(prep_screen, "ARGUS X-1: PID Simulation", cv::Point(190, 175), cv::FONT_HERSHEY_DUPLEX, 1.0, cv::Scalar(0, 255, 255), 2);
    cv::putText(prep_screen, "Proportional Gain 0.02", cv::Point(220, 310), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 255, 255), 1);
    cv::putText(prep_screen, "Integral Gain 0.1", cv::Point(220, 340), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 255, 255), 1);
    cv::putText(prep_screen, "Derivative Gain  0.01", cv::Point(220, 370), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 255, 255), 1);

    cv::imshow("Argus X-1: PID Simulation", prep_screen);

    while (true) {
        if (cv::waitKey(10) == 32) {
            break;
        }
    }

    auto start_time = std::chrono::steady_clock::now();
    auto last_time = start_time;
    for (int i = 0; i < 1000; i++) {

        auto current_time = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = current_time - last_time;

        double dt = elapsed.count();
        if (dt < 0.01) dt = 0.01; // prevent 2e-07 integral kick, better solutions??

        std::chrono::duration<double> total_elapsed = current_time - start_time;

        updatePID(ctrl, target_x, target_y, laser_x, laser_y, dt, output_x, output_y);

        laser_x += output_x;
        laser_y += output_y;

        target_x += -0.70f;
        target_y += 0.60f;

        float error_x = target_x - laser_x;
        float error_y = target_y - laser_y;

        data_log << total_elapsed.count() << "," << target_x << "," << target_y << "," << laser_x << "," << laser_y << "," << error_x << "," << error_y << "," << ctrl.pwm_x << "," << ctrl.pwm_y << "\n";

        if (i % 6 == 0 && (std::abs(error_x) >= 3.0f || std::abs(error_y) >= 3.0f)) {
            std::cout << '\n' << "--------------" << '\n' << '\n' << "Target_X: " << (int)target_x << " px | Target_Y: " << (int)target_y << " px" << '\n'
                << "Laser_X: " << (int)laser_x << " px  | Laser_Y: " << (int)laser_y << " px" << '\n'
                << "Error_X: " << (int)error_x << " px  | Error_Y: " << (int)error_y << " px" << '\n'
                << "Elapsed Time: " << dt << " seconds" << '\n';
        }
        if (i % 6 == 0 && std::abs(error_x) < 3.0f && std::abs(error_y) < 3.0f) {
            std::cout << '\n' << "**********" << '\n' << "[SYSTEM LOCKED]" << '\n' << "**********" << '\n';
        }

        cv::Mat simulation = cv::Mat::zeros(600, 800, CV_8UC3);

        if (std::abs(error_x) < 3.0f && std::abs(error_y) < 3.0f) {
            cv::putText(simulation, "LOCKED", cv::Point(10, 40), cv::FONT_HERSHEY_DUPLEX, 1.0, cv::Scalar(0, 0, 255), 1, cv::LINE_8);
        }

        last_time = current_time;

        cv::circle(simulation, cv::Point((int)target_x, (int)target_y), 5, cv::Scalar(255, 255, 0), 1);

        cv::line(simulation, cv::Point(laser_x - crosshair_length, laser_y), cv::Point(laser_x - crosshair_gap, laser_y), cv::Scalar(0, 255, 0), 1, cv::LINE_8);
        cv::line(simulation, cv::Point(laser_x, laser_y - crosshair_length), cv::Point(laser_x, laser_y - crosshair_gap), cv::Scalar(0, 255, 0), 1, cv::LINE_8);
        cv::line(simulation, cv::Point(laser_x + crosshair_length, laser_y), cv::Point(laser_x + crosshair_gap, laser_y), cv::Scalar(0, 255, 0), 1, cv::LINE_8);
        cv::line(simulation, cv::Point(laser_x, laser_y + crosshair_length), cv::Point(laser_x, laser_y + crosshair_gap), cv::Scalar(0, 255, 0), 1, cv::LINE_8);

        std::string hud_text = " X: " + std::to_string(static_cast<int>(laser_x)) + " Y: " + std::to_string(static_cast<int>(laser_y));
        cv::putText(simulation, hud_text, cv::Point(laser_x + hud_offset, laser_y + hud_offset), cv::FONT_HERSHEY_SIMPLEX, 0.3, cv::Scalar(0, 255, 0), 1, cv::LINE_8, false);

        cv::imshow("Argus X-1: PID Simulation", simulation);
        std::this_thread::sleep_for(std::chrono::milliseconds(5)); //delay  
        if (cv::waitKey(30) == 27) {
            break;
        }
    }
    data_log.close();
    cv::destroyAllWindows();
}

int main() {

    TestControl();
    return 0;
}
