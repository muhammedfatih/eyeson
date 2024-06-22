#ifndef DETECTED_OBJECT_HPP
#define DETECTED_OBJECT_HPP

#include <opencv2/opencv.hpp>
#include <string>

class DetectedObject{
    private:
        cv::Rect boundingBox;
        std::string className;
        float confidence;
    public:
        DetectedObject(cv::Rect boundingBox, std::string className, float confidence) : boundingBox(boundingBox), className(className), confidence(confidence) {}
        cv::Rect getBoundingBox() const {
            return boundingBox;
        }
        std::string getClassName() const {
            return className;
        }
        float getConfidence() const {
            return confidence;
        }
};

#endif