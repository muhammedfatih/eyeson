#ifndef NEAR_MISS_DETECTOR_HPP
#define NEAR_MISS_DETECTOR_HPP

#include <opencv2/opencv.hpp>
#include <vector>
#include "../models/ProcessedFrame.hpp"
#include "../models/IniConfigurationManager.hpp"

class NearMissDetector{
    private:
        std::string class1;
        std::string class2;
        std::string output;
        int nearMissDistance;
        float inflationSmallImage;
        bool shouldSaveBigImage;
        bool shouldSaveSmallImage;
        void saveBigImage(cv::Mat frame, DetectedObject object1, DetectedObject object2, std::string fileNameSuffix){
            cv::rectangle(frame, object1.getBoundingBox(), cv::Scalar(0, 0, 255), 3);
            cv::rectangle(frame, object2.getBoundingBox(), cv::Scalar(0, 0, 255), 3);
            std::string bigFileName = output + fileNameSuffix + "_big.jpg";
            cv::imwrite(bigFileName, frame);
        }
        void saveSmallImage(cv::Mat frame, DetectedObject object1, DetectedObject object2, std::string fileNameSuffix){
            cv::Rect nearMissRegion = object1.getBoundingBox() | object2.getBoundingBox();
            nearMissRegion.x -= nearMissRegion.width * inflationSmallImage;
            nearMissRegion.y -= nearMissRegion.height * inflationSmallImage;
            nearMissRegion.width += nearMissRegion.width * inflationSmallImage * 2;
            nearMissRegion.height += nearMissRegion.height * inflationSmallImage * 2;
            nearMissRegion &= cv::Rect(0, 0, frame.cols, frame.rows);
            cv::Mat nearMiss = frame(nearMissRegion);
            std::string smallFileName = output + fileNameSuffix + "_small.jpg";
            cv::imwrite(smallFileName, nearMiss);
        }
    public:
        NearMissDetector(IniConfigurationManager config){
            class1 = config.getString("NEARMISS", "class1", "car");
            class2 = config.getString("NEARMISS", "class2", "person");
            output = config.getString("NEARMISS", "output", "../../nearmisses/");
            inflationSmallImage = config.getFloat("NEARMISS", "inflationSmallImage", 0.2);
            nearMissDistance = config.getInt("NEARMISS", "distance", 100);
            shouldSaveBigImage = config.getBool("NEARMISS", "saveBigImage", true);
            shouldSaveSmallImage = config.getBool("NEARMISS", "saveSmallImage", true);
        }
        void save(ProcessedFrame processedFrame){
            std::vector<DetectedObject> detectedObjects = processedFrame.getDetectedObjects();
            for(int i = 0; i < detectedObjects.size(); i++){
                for(int j = i + 1; j < detectedObjects.size(); j++){
                    if(detectedObjects[i].getClassName() == class1 && detectedObjects[j].getClassName() == class2){
                        float distance = detectedObjects[i].getDistance(detectedObjects[j]);
                        if(distance < nearMissDistance){
                            cv::Mat frame = processedFrame.getFrameMatrix().clone();
                            std::string fileNamePrefix = std::to_string(processedFrame.getFrameIndex()) + "_" + std::to_string(i) + "_" + std::to_string(j);
                            if(shouldSaveSmallImage){
                                saveSmallImage(frame, detectedObjects[i], detectedObjects[j], fileNamePrefix);
                            }
                            if(shouldSaveBigImage){
                                saveBigImage(frame, detectedObjects[i], detectedObjects[j], fileNamePrefix);
                            }
                        }
                    }
                }
            }
        }
};

#endif