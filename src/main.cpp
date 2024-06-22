#include <iostream>
#include "models/ThreadSafeQueue.hpp"
#include "library/FrameSource.hpp"
#include "library/DisplayWindow.hpp"
#include "library/ObjectDetector.hpp"
#include "models/DetectedObject.hpp"
#include "models/IniConfigurationManager.hpp"
#include "models/Frame.hpp"
#include <opencv2/opencv.hpp>

int main() {
    IniConfigurationManager config("config.ini");
    std::string source = config.getString("SOURCE", "path", "");
    bool loop = config.getBool("SOURCE", "loop", true);
    FrameSource frameSource(source, loop);
    DisplayWindow displayWindow(config);
    ObjectDetector objectDetector(config);

    while(true){
        Frame frame = frameSource.getFrame();
        std::vector<DetectedObject> detectedObjects = objectDetector.detect(frame);
        frame = frame.drawDetectedObjects(detectedObjects);
        displayWindow.updateWindow(frame);
    }
    displayWindow.destroyWindow();
    return 0;
}
