#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>

constexpr int crosshair_gap = 2;
constexpr int crosshair_length = 6;

int main() {

    cv::VideoCapture cap(0);
    cv::Mat webcam_main;
    cv::Mat webcam_hsv, webcam_gray;
    cv::Mat webcam_target_mask, webcam_laser_mask;

    int min_hue = 0, min_saturation = 0, min_value = 0;
    int max_hue = 179, max_saturation = 255, max_value = 255;
    float target_x = 0, target_y = 0; 
    float laser_x = 0, laser_y = 0;

    cv::namedWindow("Argus X-1 Vision: Color Calibration");
    cv::resizeWindow("Argus X-1 Vision: Color Calibration", 450, 350);
    cv::createTrackbar("Hue Min", "Argus X-1 Vision: Color Calibration", &min_hue, 179);
    cv::createTrackbar("Hue Max", "Argus X-1 Vision: Color Calibration", &max_hue, 179); 
    cv::createTrackbar("Sat Min", "Argus X-1 Vision: Color Calibration", &min_saturation, 255);
    cv::createTrackbar("Sat Max", "Argus X-1 Vision: Color Calibration", &max_saturation, 255);
    cv::createTrackbar("Val Min", "Argus X-1 Vision: Color Calibration", &min_value, 255);
    cv::createTrackbar("Val Max", "Argus X-1 Vision: Color Calibration", &max_value, 255);

    if (!cap.isOpened()) {
        std::cout << "[ERROR] CAMERA NOT DETECTED!" << '\n';
        return -1;
    }

    std::ofstream data_log("argus_telemetry.csv");
    data_log << "Elapsed Time,Target_X,Target_Y,Target_Area,Laser_X,Laser_Y,Laser_Area" << '\n'; 

    auto start_time = std::chrono::high_resolution_clock::now();

    while (true) {

        cap >> webcam_main;
        if (webcam_main.empty())
            break;

        cv::cvtColor(webcam_main, webcam_hsv, cv::COLOR_BGR2HSV);
        cv::cvtColor(webcam_main, webcam_gray, cv::COLOR_BGR2GRAY);

        cv::Scalar lower_orange(min_hue, min_saturation, min_value);
        cv::Scalar upper_orange(max_hue, max_saturation, max_value);
        cv::inRange(webcam_hsv, lower_orange, upper_orange, webcam_target_mask);

        std::vector<std::vector<cv::Point>> target_contours;
        cv::findContours(webcam_target_mask, target_contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        cv::threshold(webcam_gray, webcam_laser_mask, 210, 255, cv::THRESH_BINARY);

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

            target_x = m.m10 / m.m00;
            target_y = m.m01 / m.m00;

            cv::line(webcam_main, cv::Point(target_x - crosshair_length, target_y), cv::Point(target_x - crosshair_gap, target_y), cv::Scalar(0, 255, 0), 1, cv::LINE_8);
            cv::line(webcam_main, cv::Point(target_x, target_y - crosshair_length), cv::Point(target_x, target_y - crosshair_gap), cv::Scalar(0, 255, 0), 1, cv::LINE_8);
            cv::line(webcam_main, cv::Point(target_x + crosshair_length, target_y), cv::Point(target_x + crosshair_gap, target_y), cv::Scalar(0, 255, 0), 1, cv::LINE_8);
            cv::line(webcam_main, cv::Point(target_x, target_y + crosshair_length), cv::Point(target_x, target_y + crosshair_gap), cv::Scalar(0, 255, 0), 1, cv::LINE_8);
            
            std::string hud_text = " X: " + std::to_string(static_cast<int>(target_x)) + " Y: " + std::to_string(static_cast<int>(target_y));
            cv::putText(webcam_main, hud_text, cv::Point(target_x + 15, target_y + 15), cv::FONT_HERSHEY_SIMPLEX, 0.35, cv::Scalar(0, 255, 0), 1, cv::LINE_8, false);

            std::cout << "TARGET DETECTED AT -> X: " << static_cast<int>(target_x) << " | Y: " << static_cast<int>(target_y) << '\n'; 
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

            laser_x = m.m10 / m.m00;
            laser_y = m.m01 / m.m00;

            cv::circle(webcam_main, cv::Point(laser_x, laser_y), 8, cv::Scalar(255, 255, 0), 1);

            std::string hud_text = " X: " + std::to_string(static_cast<int>(laser_x)) + " Y: " + std::to_string(static_cast<int>(laser_y));
            cv::putText(webcam_main, hud_text, cv::Point(laser_x + 15, laser_y + 15), cv::FONT_HERSHEY_SIMPLEX, 0.35, cv::Scalar(255, 255, 0), 1, cv::LINE_8, false);

            std::cout << "Laser Tracking At -> X: " << static_cast<int>(laser_x) << " | Y: " << static_cast<int>(laser_y) << '\n';
        }

     if (target_best_match != -1 && laser_best_match != -1 && target_max_area > 150 && laser_max_area > 10) {
         double distance = std::hypot(target_x - laser_x, target_y - laser_y); // euclidean distance formula
         if (distance < 25.0) {
             cv::putText(webcam_main, "LOCKED", cv::Point(20, 60), cv::FONT_HERSHEY_DUPLEX, 1.0, cv::Scalar(0, 0, 255), 2, cv::LINE_8); //find better x y fix
             std::cout << "[SYSTEM LOCKED]" << '\n';
             // possible trigger system
         }
     }

        else {
            std::cout << "Searching for a target..." << '\n';
        }

        auto current_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = current_time - start_time;
        data_log << elapsed.count() << "," << target_x << "," << target_y << "," << target_max_area << "," << laser_x << "," << laser_y << "," << laser_max_area << '\n';

        cv::imshow("Argus X-1 Vision: Main Source", webcam_main);
        cv::imshow("Argus X-1 Vision: HSV Space", webcam_hsv);
        cv::imshow("Argus X-1 Vision: Binary Mask", webcam_target_mask);
        cv::imshow("Argus X-1 Vision: Laser Mask", webcam_laser_mask);

        if (cv::waitKey(1) == 27) {
            data_log.close();
            break;
        }

    }
    cv::destroyAllWindows();
    return 0;
}

