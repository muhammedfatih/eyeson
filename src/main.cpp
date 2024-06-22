#include <iostream>
#include <thread>
#include <opencv2/opencv.hpp>
#include "models/ThreadSafeQueue.hpp"
#include "library/FrameSource.hpp"
#include "library/DisplayWindow.hpp"
#include "library/ObjectDetector.hpp"
#include "models/DetectedObject.hpp"
#include "models/IniConfigurationManager.hpp"
#include "models/Frame.hpp"

void fetchFrame(IniConfigurationManager& config, ThreadSafeQueue<Frame>& queue) {
    FrameSource source(config);
    while (true) {
        Frame frame = source.getFrame();
        queue.push(frame);
    }
}

void processFrame(IniConfigurationManager& config, ThreadSafeQueue<Frame>& rawQueue, ThreadSafeQueue<Frame>& processedQueue) {
    ObjectDetector detector(config);
    while (true) {
        Frame frame;
        if (rawQueue.tryPop(frame)) {
            std::vector<DetectedObject> detectedObjects = detector.detect(frame);
            frame = frame.drawDetectedObjects(detectedObjects);
            processedQueue.push(frame);
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}

int main() {
    IniConfigurationManager config("config.ini");
    DisplayWindow window(config);

    ThreadSafeQueue<Frame> fetchedFrameQueue;
    ThreadSafeQueue<Frame> processedFrameQueue;

    std::thread threadFetchFrame(fetchFrame, std::ref(config), std::ref(fetchedFrameQueue));
    std::thread threadProcessFrame(processFrame, std::ref(config), std::ref(fetchedFrameQueue), std::ref(processedFrameQueue));

    while (true) {
        Frame frame;
        if (processedFrameQueue.tryPop(frame)) {
            window.updateWindow(frame);
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    threadFetchFrame.join();
    threadProcessFrame.join();

    return 0;
}
