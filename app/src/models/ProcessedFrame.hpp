#ifndef PROCESSED_FRAME_HPP
#define PROCESSED_FRAME_HPP

#include <opencv2/opencv.hpp>
#include <vector>
#include "DetectedObject.hpp"

class ProcessedFrame : public Frame {
    private:
        std::vector<DetectedObject> detectedObjects;
    public:
        ProcessedFrame() {}
        ProcessedFrame(Frame frame, std::vector<DetectedObject> detectedObjects) : Frame(frame.getFrameMatrix(), frame.getFrameIndex()), detectedObjects(detectedObjects) {}
        std::vector<DetectedObject> getDetectedObjects() const {
            return detectedObjects;
        }
        Frame draw() const{
            cv::Mat frame = getFrameMatrix().clone();
            for(auto detectedObject : detectedObjects){
                cv::rectangle(frame, detectedObject.getBoundingBox(), cv::Scalar(0, 255, 0), 3);
                cv::rectangle(frame, cv::Point(detectedObject.getBoundingBox().x, detectedObject.getBoundingBox().y - 20), cv::Point(detectedObject.getBoundingBox().x + detectedObject.getBoundingBox().width, detectedObject.getBoundingBox().y), cv::Scalar(0, 255, 0), cv::FILLED);
                cv::putText(frame, detectedObject.getClassName(), cv::Point(detectedObject.getBoundingBox().x, detectedObject.getBoundingBox().y - 5), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
            }
            return Frame(frame, getFrameIndex());
        }
};

#endif