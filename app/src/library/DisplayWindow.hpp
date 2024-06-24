#ifndef DISPLAY_WINDOW_HPP
#define DISPLAY_WINDOW_HPP

#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "../library/IniConfigurationManager.hpp"
#include "../models/Frame.hpp"

class DisplayWindow{
    private:
        std::string windowName;
        bool debugshowFrameIndex = false;
        bool debugStepByStep = false;
        bool debugShowWindow = false;
    public:
        DisplayWindow(IniConfigurationManager &config){
            this->windowName = config.getString("DISPLAY_WINDOW", "name", "Display Window");
            this->debugshowFrameIndex = config.getBool("DEBUG", "showframeindex", false);
            this->debugStepByStep = config.getBool("DEBUG", "stepbystepframe", false);
            this->debugShowWindow = config.getBool("DEBUG", "showdisplaywindow", false);
            if(this->debugShowWindow){
                cv::namedWindow(windowName, cv::WINDOW_NORMAL);
            }
        }
        void updateWindow(Frame frame){
            if(this->debugShowWindow){
                if(debugshowFrameIndex){
                    cv::putText(frame.getFrameMatrix(), "Frame Index: " + std::to_string(frame.getFrameIndex()), cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);
                }
                cv::imshow(windowName, frame.getFrameMatrix());
                cv::waitKey(this->debugStepByStep ? 0 : 1);
            }
        }
        void destroyWindow(){
            if(this->debugShowWindow){
                cv::destroyWindow(windowName);
            }
        }
};

#endif