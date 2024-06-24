# 1. eyeson

## 1.1 Summary:
eyeson is a personal project aimed at creating a visual monitoring system using computer vision techniques. The project utilizes a camera to capture live video feed and applies computer vision technologies and try to detect near misses.

Project can operate either:
- on a host machine locally due to performance concerns or 
- with docker containers to have a quick look.

## 1.2 Features:
- Capturing live camera feeds
- Detecting objects in real time
- Detecting near misses in real time
- Saving near misses as image
- Configurable and monitorable by a web interface

## 1.3 Prerequests

All prerequests are tested on Mac OSX Sonoma 14.5.

- Docker (tested version 24.0.7, build afdd53b4e3)
- Docker-Compose (tested version v2.23.3-desktop.2)

## 1.4 Run

`docker-compose up --build`

## 1.5 Improvements

- App can be controlled by a service so web interface might start/stop it.
- No automation tests were written at all.
- For security wise, only frontend port should be exposed.

# 2. App

## 2.1 Summary

This section will contain C++ application related informations. 

eyeson application has some basic functionalities:

- working with configurable parameters
- capturing frames from streams or videos
- detecting objects using darknet
- detecting near miss with a primitive algorithm
- saving catched near misses as whole image or small image

## 2.2 Prerequests:
- OpenCV 4
- C++ 14

## 2.3 Compile
```g++ -std=c++14 -pthread -o bin/eyeson src/main.cpp `pkg-config --cflags --libs opencv4` ```

## 2.4 Run
`bin/eyeson app/config/config.ini`

## 2.5 Configurations

### SOURCE

- path: video or feed url
- loop: to run same video right after it finishes continuously


### DETECTOR

- weight: yolo weight path
- config: yolo config path
- names: yolo coco name paths
- threshold: confidence threshold
- nms: nms threshold value, to decide how close objects we would like to show
- networkheight
- networkwidth

### NEARMISS

- class1: coco name for nearmiss
- class2: coco name for nearmiss
- output: folder to save nearmiss images
- distance: distance to accept two object as nearmiss
- inflationsmallimage: to have a better small image
- savebigimage
- savesmallimage

### DISPLAY_WINDOW

- name: window name

### DEBUG

- showframeindex:
- stepbystepframe: next frame will be fetched only if there is a hit for a space char
- showframefetcherror:
- showdisplaywindow

### BACKEND

- savelastframe:
- lastframepath:

## 2.6 Improvements

- Communication between backend can be handle better. 
- USB Camera support can be provided.
- There could be a better exception handling.
- Tracking algorithms can be used to improve quality of outputs.

# 3. Backend

## 3.1 Summary

There is a simple Fast API application to have a web interface for the app.

It has basically three functionality:

- Setting/Getting app configs
- Getting near miss images and thumbnails
- Getting last frame

## 3.2 Prerequests:

- python (tested version 3.12.2)
- pip (tested version 24.0)

## 3.3 Install

`pip install -r requirements.txt`

## 3.4 Run

With specifing config file path:

- `CONFIG_FILE=app/config/config.ini uvicorn app.main:app --reload`

or if config file is under `/app/config/config.ini`, can be run directly:

- `uvicorn app.main:app --reload`

## 3.5 Improvements

- Security can be improved, no credential check is currently exist
- Last frame communication is based on files which is far away from ideal. It can be replaced to memory.
- We can keep last 10-20 frames in memory so there could be a smooth streaming.

# 4. Frontend

## 4.1 Summary

There is a simple React application to have a web interface for the app.

It has basically three functionality:

- Showing near miss gallery
- Showing and updating app configurations
- Showing the app stream

## 4.2 Prerequests:

- node (tested version v20.5.1)
- npm (tested version 9.8.0)

## 4.3 Install

`npm install`

## 4.4 Run

`npm start`

If there is any ssl related issue in local runs, type command below and re-run `npm start`

```
export NODE_OPTIONS=--openssl-legacy-provider
```

## 4.5 Configurations

Configuration form visualization is handling dynamically with help of the `config.ini` file. There are two sections in `config.ini` to help this visualization: `WEB_INTERFACE_LABELS` and `WEB_INTERFACE_TYPES`.
 
## 4.6 Improvements

- State management can be centrialized.
- Component separation can be better.
- Showing the app stream can be improved.
- Configuration form handling can be improved.
- Service url can be fetched from a config file.
- There could be better UI.

# Screenshots

![Stream](docs/stream.png)

![Gallery](docs/nearmiss-gallery.png)

![Camera Configuration](docs/configuration-camera.png)

![Detector Configuration](docs/configuration-detection.png)

![Detector Class Configuration](docs/configuration-detection-class.png)

![Nearmiss Configuration](docs/configuration-nearmiss.png)
