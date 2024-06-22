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
        Frame(cv::Mat frameMatrix, int frameIndex) : frameMatrix(frameMatrix), frameIndex(frameIndex) {}
        cv::Mat getFrameMatrix() const {
            return frameMatrix;
        }
        int getFrameIndex() const {
            return frameIndex;
        }
        Frame drawDetectedObjects(std::vector<DetectedObject> detectedObjects) const{
            cv::Mat frame = frameMatrix.clone();
            for(auto detectedObject : detectedObjects){
                cv::rectangle(frame, detectedObject.getBoundingBox(), cv::Scalar(0, 255, 0), 3);
                cv::rectangle(frame, cv::Point(detectedObject.getBoundingBox().x, detectedObject.getBoundingBox().y - 20), cv::Point(detectedObject.getBoundingBox().x + detectedObject.getBoundingBox().width, detectedObject.getBoundingBox().y), cv::Scalar(0, 255, 0), cv::FILLED);
                cv::putText(frame, detectedObject.getClassName(), cv::Point(detectedObject.getBoundingBox().x, detectedObject.getBoundingBox().y - 5), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
            }
            return Frame(frame, frameIndex);
        }
};

#endif