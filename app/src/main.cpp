#include <iostream>
#include <thread>
#include <opencv2/opencv.hpp>
#include "models/ThreadSafeQueue.hpp"
#include "models/DetectedObject.hpp"
#include "library/IniConfigurationManager.hpp"
#include "models/Frame.hpp"
#include "models/ProcessedFrame.hpp"
#include "library/FrameSource.hpp"
#include "library/DisplayWindow.hpp"
#include "library/ObjectDetector.hpp"
#include "library/NearMissDetector.hpp"
#include "library/LastFrameSaver.hpp"

void fetchFrame(IniConfigurationManager& config, ThreadSafeQueue<Frame>& queue) {
    FrameSource source(config);
    while (true) {
        Frame frame = source.getFrame();
        queue.push(frame);
    }
}

void processFrame(IniConfigurationManager& config, ThreadSafeQueue<Frame>& fetchedFrameQueue, ThreadSafeQueue<ProcessedFrame>& processedFrameQueue) {
    ObjectDetector detector(config);
    while (true) {
        Frame frame;
        if (fetchedFrameQueue.tryPop(frame)) {
            std::vector<DetectedObject> detectedObjects = detector.detect(frame);
            ProcessedFrame processedFrame(frame, detectedObjects);
            processedFrameQueue.push(processedFrame);
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}

void checkNearMisses(IniConfigurationManager& config, ThreadSafeQueue<ProcessedFrame>& processedFrameQueue, ThreadSafeQueue<Frame>& displayFrameQueue) {
    NearMissDetector detector(config);
    while(true){
        ProcessedFrame processedFrame;
        if(processedFrameQueue.tryPop(processedFrame)){
            displayFrameQueue.push(processedFrame.draw());
            detector.save(processedFrame);
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}

void saveLastFrame(IniConfigurationManager& config, ThreadSafeQueue<Frame>& displayFrameQueue) {
    LastFrameSaver saver(config);
    while (true) {
        Frame frame;
        if (displayFrameQueue.tryPop(frame)) {
            saver.save(frame);
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

int main(int argc, char** argv) {
    std::string configPath = "/app/config/config.ini";
    if (argc > 1) {
        configPath = argv[1];
    }

    IniConfigurationManager config(configPath);
    DisplayWindow window(config);

    ThreadSafeQueue<Frame> fetchedFrameQueue;
    ThreadSafeQueue<ProcessedFrame> processedFrameQueue;
    ThreadSafeQueue<Frame> displayFrameQueue;

    std::thread threadFetchFrame(fetchFrame, std::ref(config), std::ref(fetchedFrameQueue));
    std::thread threadProcessFrame(processFrame, std::ref(config), std::ref(fetchedFrameQueue), std::ref(processedFrameQueue));
    std::thread threadCheckNearMisses(checkNearMisses, std::ref(config), std::ref(processedFrameQueue), std::ref(displayFrameQueue));
    std::thread threadSaveLastFrame(saveLastFrame, std::ref(config), std::ref(displayFrameQueue));

    while (true) {
        Frame frame;
        if (displayFrameQueue.tryPop(frame)) {
           window.updateWindow(frame);
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    threadFetchFrame.join();
    threadProcessFrame.join();

    return 0;
}
