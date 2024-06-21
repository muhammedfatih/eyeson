#ifndef DISPLAY_WINDOW_HPP
#define DISPLAY_WINDOW_HPP

#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "../models/IniConfigurationManager.hpp"
#include "../models/Frame.hpp"

class DisplayWindow{
    private:
        std::string windowName;
        bool debugShowFrameIndex = false;
    public:
        DisplayWindow(IniConfigurationManager &config){
            this->windowName = config.getString("DISPLAY_WINDOW", "name", "Display Window");
            this->debugShowFrameIndex = config.getBool("DEBUG", "showFrameIndex", false);
            cv::namedWindow(windowName, cv::WINDOW_NORMAL);
        }
        void updateWindow(Frame frame){
            if(debugShowFrameIndex){
                cv::putText(frame.getFrameMatrix(), "Frame Index: " + std::to_string(frame.getFrameIndex()), cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);
            }
            cv::imshow(windowName, frame.getFrameMatrix());
            cv::waitKey(1);
        }
        void destroyWindow(){
            cv::destroyWindow(windowName);
        }
};

#endif