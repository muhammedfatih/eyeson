# Project Name: eyeson

## Description:
eyeson is a personal project aimed at creating a visual monitoring system using computer vision techniques. The project utilizes a camera to capture live video feed and applies computer vision technologies and try to detect near misses.

## Features:
- Live video feed from a camera
- Object detection
- Near miss detections
- Saving near miss as image

## Prerequests:
- OpenCV 4
- C++ 14

## Installation:
- `brew install opencv`

## Compile
- ```g++ -std=c++14 -pthread -o bin/eyeson src/main.cpp `pkg-config --cflags --libs opencv4` ```

## Run
- `bin/eyeson`

## Configure

### SOURCE

- path: video or feed url
- loop: to run same video right after it finishes continuously

### DETECTOR

- weight: yolo weight path
- config: yolo config path
- names: yolo coco name paths
- threshold: confidence threshold
- nms: nms threshold value, to decide how close objects we would like to show
- networkHeight
- networkWidth

### NEARMISS
- class1: coco name for nearmiss
- class2: coco name for nearmiss
- output: folder to save nearmiss images
- distance: distance to accept two object as nearmiss
- inflationSmallImage: to have a better small image
- saveBigImage
- saveSmallImage

### DISPLAY_WINDOW
- name: window name

### DEBUG
- showFrameIndex:
- stepByStepFrame: next frame will be fetched only if there is a hit for a space char
- showFrameFetchError: