#include "argus_vision.h"

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>

//#include <fstream>  make sure state machine is actually capable and it works
//#include <chrono> ****

constexpr int crosshair_gap = 2;
constexpr int crosshair_length = 6;
constexpr int hud_offset = 15;

VisionState processFrame(cv::VideoCapture& cap,
    int min_hue, int max_hue,
    int min_sat, int max_sat,
    int min_val, int max_val) {

    VisionState data;

    cv::Mat webcam_main;
    cv::Mat webcam_hsv, webcam_gray;
    cv::Mat webcam_target_mask, webcam_laser_mask;
    float target_x = 0, target_y = 0;
    float laser_x = 0, laser_y = 0;

    cap >> webcam_main;

    if (webcam_main.empty()) {
        return data;
    }

    cv::cvtColor(webcam_main, webcam_hsv, cv::COLOR_BGR2HSV);
    cv::cvtColor(webcam_main, webcam_gray, cv::COLOR_BGR2GRAY);

    cv::Scalar lower_orange(min_hue, min_sat, min_val);
    cv::Scalar upper_orange(max_hue, max_sat, max_val);
    cv::inRange(webcam_hsv, lower_orange, upper_orange, webcam_target_mask);

    std::vector<std::vector<cv::Point>> target_contours;
    cv::findContours(webcam_target_mask, target_contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    cv::threshold(webcam_gray, webcam_laser_mask, 200, 255, cv::THRESH_BINARY);

    double target_max_area = 0, laser_max_area = 0;
    int target_best_match = -1, laser_best_match = -1;

    for (int i = 0; i < target_contours.size(); i++) {
        double area = cv::contourArea(target_contours[i]);
        if (area > target_max_area) {
            target_max_area = area;
            target_best_match = i;
        }
    }

    if (target_best_match != -1 && target_max_area > 150) {
        cv::Moments m = cv::moments(target_contours[target_best_match]);
        if (m.m00 > 0) {
            target_x = m.m10 / m.m00;
            target_y = m.m01 / m.m00;
        }

        cv::line(webcam_main, cv::Point(target_x - crosshair_length, target_y), cv::Point(target_x - crosshair_gap, target_y), cv::Scalar(0, 255, 0), 1, cv::LINE_8);
        cv::line(webcam_main, cv::Point(target_x, target_y - crosshair_length), cv::Point(target_x, target_y - crosshair_gap), cv::Scalar(0, 255, 0), 1, cv::LINE_8);
        cv::line(webcam_main, cv::Point(target_x + crosshair_length, target_y), cv::Point(target_x + crosshair_gap, target_y), cv::Scalar(0, 255, 0), 1, cv::LINE_8);
        cv::line(webcam_main, cv::Point(target_x, target_y + crosshair_length), cv::Point(target_x, target_y + crosshair_gap), cv::Scalar(0, 255, 0), 1, cv::LINE_8);

        std::string hud_text = " X: " + std::to_string(static_cast<int>(target_x)) + " Y: " + std::to_string(static_cast<int>(target_y));
        cv::putText(webcam_main, hud_text, cv::Point(target_x + hud_offset, target_y + hud_offset), cv::FONT_HERSHEY_SIMPLEX, 0.35, cv::Scalar(0, 255, 0), 1, cv::LINE_8, false);
    }

    std::vector<std::vector<cv::Point>> laser_contours;
    cv::findContours(webcam_laser_mask, laser_contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    for (int i = 0; i < laser_contours.size(); i++) {
        double area = cv::contourArea(laser_contours[i]);
        if (area > laser_max_area) {
            laser_max_area = area;
            laser_best_match = i;
        }
    }

    if (laser_best_match != -1 && laser_max_area > 2) {
        cv::Moments m = cv::moments(laser_contours[laser_best_match]);

        if (m.m00 > 0) {
            laser_x = m.m10 / m.m00;
            laser_y = m.m01 / m.m00;
        }

        cv::circle(webcam_main, cv::Point(laser_x, laser_y), 8, cv::Scalar(255, 255, 0), 1);

        std::string hud_text = " X: " + std::to_string(static_cast<int>(laser_x)) + " Y: " + std::to_string(static_cast<int>(laser_y));
        cv::putText(webcam_main, hud_text, cv::Point(laser_x + hud_offset, laser_y + hud_offset), cv::FONT_HERSHEY_SIMPLEX, 0.35, cv::Scalar(255, 255, 0), 1, cv::LINE_8, false);
    }

    if (target_best_match != -1 && laser_best_match != -1 && target_max_area > 150 && laser_max_area > 10) {
        double distance = std::hypot(target_x - laser_x, target_y - laser_y);
        if (distance < 25.0) {
            cv::putText(webcam_main, "LOCKED", cv::Point(20, 60), cv::FONT_HERSHEY_DUPLEX, 1.0, cv::Scalar(0, 0, 255), 2, cv::LINE_8);
            data.locked = true;
        }
    }

    data.target_x = target_x;
    data.target_y = target_y;
    data.laser_x = laser_x;
    data.laser_y = laser_y;
    data.target_area = target_max_area;
    data.laser_area = laser_max_area;
    data.target_detected = (target_best_match != -1 && target_max_area > 150);
    data.laser_detected = (laser_best_match != -1 && laser_max_area > 2);
    data.frame = webcam_main.clone();
    data.hsv = webcam_hsv.clone();
    data.target_mask = webcam_target_mask.clone();
    data.laser_mask = webcam_laser_mask.clone();

    return data;
}
