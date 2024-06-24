#ifndef LAST_FRAME_SAVER_HPP
#define LAST_FRAME_SAVER_HPP

#include "IniConfigurationManager.hpp"
#include "../models/ThreadSafeQueue.hpp"
#include "../models/Frame.hpp"
#include <opencv2/opencv.hpp>
#include <chrono>
#include <thread>

class LastFrameSaver {
private:
    bool saveLastFrame = false;
    std::string lastFramePath = "last_frame.png";    
public:
    LastFrameSaver(IniConfigurationManager &config){
        this->saveLastFrame = config.getBool("BACKEND", "savelastframe", false);
        this->lastFramePath = config.getString("BACKEND", "lastframepath", "last_frame.png");
    }
    void save(Frame frame) {
        if (saveLastFrame) {
            cv::imwrite(lastFramePath, frame.getFrameMatrix());
        }
    }
};

#endif // LAST_FRAME_SAVER_HPP