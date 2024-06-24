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
        float getDistance(DetectedObject other) const {
            cv::Point center1 = cv::Point(boundingBox.x + boundingBox.width / 2, boundingBox.y + boundingBox.height / 2);
            cv::Point center2 = cv::Point(other.boundingBox.x + other.boundingBox.width / 2, other.boundingBox.y + other.boundingBox.height / 2);
            return cv::norm(center1 - center2);
        }
};

#endif