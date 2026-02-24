#include <opencv2/opencv.hpp>
#include <iostream>

int main() {

    cv::VideoCapture cap(0);
    cv::Mat webcam;

    if (!cap.isOpened()) {
        std::cout << "Camera is not detected!" << '\n';

        return -1;

    }

    while (true) {

        cap >> webcam;

        if (webcam.empty())
        break;

        cv::imshow("Test", webcam);

        if (cv::waitKey(33) == 27) // 1000/33 = 30fps
        break;
    }
    cv::destroyAllWindows();
    return 0;
}
