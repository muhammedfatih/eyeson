#ifndef FRAME_SOURCE_HPP
#define FRAME_SOURCE_HPP

#include <opencv2/opencv.hpp>
#include <string>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <iostream>
#include "../models/Frame.hpp"
#include "../library/IniConfigurationManager.hpp"

class FrameSource{
    private:
        cv::VideoCapture capture;
        cv::Mat lastFrameMatrix;
        int lastFrameIndex;
        bool loop;
        bool showFrameFetchError;
    public:
        FrameSource(IniConfigurationManager &config){
            std::string source = config.getString("SOURCE", "path", "");
            bool loop = config.getBool("SOURCE", "loop", true);
            showFrameFetchError = config.getBool("DEBUG", "showframefetcherror", false);

            capture = cv::VideoCapture(source);
            lastFrameIndex = 0;
            capture.set(cv::CAP_PROP_POS_FRAMES, 0);
            this->loop = loop;
            if(!capture.isOpened()){
                std::cerr << "Error: Could not open the video file." << std::endl;
            }
        }
        Frame getFrame(){
            cv::Mat currentFrameMatrix;
            capture >> currentFrameMatrix;
            lastFrameIndex++;
            if(currentFrameMatrix.empty()){
                if(showFrameFetchError){
                    std::cerr << "Error: Could not read the frame." << std::endl;
                }
            }else{
                lastFrameMatrix = currentFrameMatrix;
            }

            if(loop && capture.get(cv::CAP_PROP_POS_FRAMES) == capture.get(cv::CAP_PROP_FRAME_COUNT)){
                capture.set(cv::CAP_PROP_POS_FRAMES, 0);
            }

            return Frame(lastFrameMatrix, lastFrameIndex);    
        }
        ~FrameSource(){
            capture.release();
        }
};


#endif