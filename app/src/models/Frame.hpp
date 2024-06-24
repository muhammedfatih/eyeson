#ifndef FRAME_HPP
#define FRAME_HPP

#include <opencv2/opencv.hpp>
#include <vector>
#include "DetectedObject.hpp"

class Frame {
    private:
        cv::Mat frameMatrix;
        int frameIndex;
    public:
        Frame() {}
        Frame(cv::Mat frameMatrix, int frameIndex) : frameMatrix(frameMatrix), frameIndex(frameIndex) {}
        cv::Mat getFrameMatrix() const {
            return frameMatrix;
        }
        int getFrameIndex() const {
            return frameIndex;
        }
};

#endif