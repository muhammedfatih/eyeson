#ifndef OBJECT_DETECTOR_HPP
#define OBJECT_DETECTOR_HPP

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "../models/DetectedObject.hpp"
#include "../models/Frame.hpp"
#include "../library/IniConfigurationManager.hpp"

class ObjectDetector{
private:
    cv::dnn::Net net;
    std::vector<std::string> classNames;
    float confidenceThreshold;
    float nmsThreshold;
    int networkHeight;
    int networkWidth;

public:
    ObjectDetector(IniConfigurationManager &config) {
        std::string modelConfiguration = config.getString("DETECTOR", "config", "");
        std::string modelWeights = config.getString("DETECTOR", "weight", "");
        std::string classNamesFile = config.getString("DETECTOR", "names", "");
        confidenceThreshold = config.getFloat("DETECTOR", "threshold", 0.5);
        nmsThreshold = config.getFloat("DETECTOR", "nms", 0.4);
        networkWidth = config.getInt("DETECTOR", "networkwidth", 416);
        networkHeight = config.getInt("DETECTOR", "networkheight", 416);
        net = cv::dnn::readNetFromDarknet(modelConfiguration, modelWeights);
        if (net.empty()) {
            std::cerr << "Error: Could not load the model." << std::endl;
        }

        std::ifstream classNamesFileStream(classNamesFile);
        if (classNamesFileStream.is_open()) {
            std::string className = "";
            while (std::getline(classNamesFileStream, className)) {
                classNames.push_back(className);
            }
        } else {
            std::cerr << "Error: Could not open the file with the class names." << std::endl;
        }
    }

    std::vector<DetectedObject> detect(Frame frame) {
        cv::Mat blob = cv::dnn::blobFromImage(frame.getFrameMatrix(), 1/255.0, cv::Size(networkWidth, networkHeight), cv::Scalar(0, 0, 0), true, false);

        net.setInput(blob);
        std::vector<cv::Mat> outputLayers;
        net.forward(outputLayers, getOutputLayerNames());

        std::vector<int> classIds;
        std::vector<float> confidences;
        std::vector<cv::Rect> boxes;

        for (auto &outputLayer : outputLayers) {
            for (int i = 0; i < outputLayer.rows; i++) {
                cv::Mat objectData = outputLayer.row(i);
                cv::Point classIdPoint;
                double confidence;
                cv::minMaxLoc(objectData.colRange(5, outputLayer.cols), 0, &confidence, 0, &classIdPoint);
                if (confidence > confidenceThreshold) {
                    int centerX = static_cast<int>(objectData.at<float>(0) * frame.getFrameMatrix().cols);
                    int centerY = static_cast<int>(objectData.at<float>(1) * frame.getFrameMatrix().rows);
                    int width = static_cast<int>(objectData.at<float>(2) * frame.getFrameMatrix().cols);
                    int height = static_cast<int>(objectData.at<float>(3) * frame.getFrameMatrix().rows);
                    int left = centerX - width / 2;
                    int top = centerY - height / 2;
                    classIds.push_back(classIdPoint.x);
                    confidences.push_back(static_cast<float>(confidence));
                    boxes.push_back(cv::Rect(left, top, width, height));
                }
            }
        }

        std::vector<int> indices;
        cv::dnn::NMSBoxes(boxes, confidences, confidenceThreshold, nmsThreshold, indices);

        std::vector<DetectedObject> detectedObjects;
        for (int idx : indices) {
            detectedObjects.push_back(DetectedObject(boxes[idx], classNames[classIds[idx]], confidences[idx]));
        }

        return detectedObjects;
    }

    std::vector<std::string> getOutputLayerNames() {
        static std::vector<std::string> outputLayerNames;
        if (outputLayerNames.empty()) {
            std::vector<int> outputLayers = net.getUnconnectedOutLayers();
            std::vector<std::string> layerNames = net.getLayerNames();
            for (int outputLayer : outputLayers) {
                outputLayerNames.push_back(layerNames[outputLayer - 1]);
            }
        }
        return outputLayerNames;
    }
};

#endif