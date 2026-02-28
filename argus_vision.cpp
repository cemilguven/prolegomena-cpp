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
    cv::Mat webcam_hsv, webcam_gray, webcam_binary_mask;

    int min_hue = 0, min_saturation = 0, min_value = 0;
    int max_hue = 179, max_saturation = 255, max_value = 255;
    float target_x = 0, target_y = 0; // 32-bit ~6–7 digits

    cv::namedWindow("Argus X-1 Vision: Color Calibration");
    cv::resizeWindow("Argus X-1 Vision: Color Calibration", 450, 350);
    cv::createTrackbar("Hue Min", "Argus X-1 Vision: Color Calibration", &min_hue, 179);
    cv::createTrackbar("Hue Max", "Argus X-1 Vision: Color Calibration", &max_hue, 179); // max 179?
    cv::createTrackbar("Sat Min", "Argus X-1 Vision: Color Calibration", &min_saturation, 255);
    cv::createTrackbar("Sat Max", "Argus X-1 Vision: Color Calibration", &max_saturation, 255);
    cv::createTrackbar("Val Min", "Argus X-1 Vision: Color Calibration", &min_value, 255);
    cv::createTrackbar("Val Max", "Argus X-1 Vision: Color Calibration", &max_value, 255);

    if (!cap.isOpened()) {
        std::cout << "[ERROR] CAMERA NOT DETECTED!" << '\n';
        return -1;
    }

    std::ofstream data_log("argus_telemetry.csv");
    data_log << "Elapsed Time,X,Y,Area" << '\n'; // , to separate into columns
    auto start_time = std::chrono::high_resolution_clock::now();

    while (true) {

        cap >> webcam_main;
        if (webcam_main.empty())
            break;

        cv::cvtColor(webcam_main, webcam_hsv, cv::COLOR_BGR2HSV);

        cv::Scalar lower_orange(min_hue, min_saturation, min_value);
        cv::Scalar upper_orange(max_hue, max_saturation, max_value);
        cv::inRange(webcam_hsv, lower_orange, upper_orange, webcam_binary_mask);

        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(webcam_binary_mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        double max_area = 0; // largest area found while running
        int best_match = -1; // -1 not found - starts from 0 (found)

        for (int i = 0; i < contours.size(); i++) {
            double area = cv::contourArea(contours[i]); // calcualtes each one
            if (area > max_area) { // only updates when finds smthn bigger
                max_area = area;
                best_match = i; // saves the biggest one
            }
        }

        if (best_match != -1 && max_area > 150) { // max_area same logic as (m.m00 > 150)  ALSO! could this be used to calculate z? distance? Z ! Z ! Z ! Z ! Z ! Z !  
            //the smaller the pixel farther away it is and the bigger it is, the closer is?? !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            cv::Moments m = cv::moments(contours[best_match]);

            target_x = m.m10 / m.m00; // m00 = number of white pixels -- m10 = sum of x positions of white pixels -- m01 = sum of y positions of white pixels
            target_y = m.m01 / m.m00;

            auto current_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> elapsed = current_time - start_time;
            data_log << elapsed.count() << "," << target_x << "," << target_y << "," << m.m00 << '\n';

            //cv::circle(webcam_main, cv::Point(target_x, target_y), 4, cv::Scalar(0, 0, 255), -1);

            cv::line(webcam_main, cv::Point(target_x - crosshair_length, target_y), cv::Point(target_x - crosshair_gap, target_y), cv::Scalar(0, 255, 0), 1, cv::LINE_8);
            cv::line(webcam_main, cv::Point(target_x, target_y - crosshair_length), cv::Point(target_x, target_y - crosshair_gap), cv::Scalar(0, 255, 0), 1, cv::LINE_8);
            cv::line(webcam_main, cv::Point(target_x + crosshair_length, target_y), cv::Point(target_x + crosshair_gap, target_y), cv::Scalar(0, 255, 0), 1, cv::LINE_8);
            cv::line(webcam_main, cv::Point(target_x, target_y + crosshair_length), cv::Point(target_x, target_y + crosshair_gap), cv::Scalar(0, 255, 0), 1, cv::LINE_8);
            
            std::string hud_text = " X: " + std::to_string(static_cast<int>(target_x)) + " Y: " + std::to_string(static_cast<int>(target_y));
            cv::putText(webcam_main, hud_text, cv::Point(target_x + 15, target_y + 15), cv::FONT_HERSHEY_SIMPLEX, 0.35, cv::Scalar(0, 255, 0), 1, cv::LINE_8, false);

            std::cout << "TARGET DETECTED AT -> X: " << static_cast<int>(target_x) << " | Y: " << static_cast<int>(target_y) << '\n'; 
        }

        else {
            std::cout << "Searching for a target..." << '\n';
        }

        cv::imshow("Argus X-1 Vision: Main Source", webcam_main);
        cv::imshow("Argus X-1 Vision: HSV Space", webcam_hsv);
        cv::imshow("Argus X-1 Vision: Binary Mask", webcam_binary_mask);

        if (cv::waitKey(1) == 27) {// 1000/33 = 30fps
            data_log.close();
            break;
        }
    }
    cv::destroyAllWindows();
    return 0;
}
