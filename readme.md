# Introduction

This code uses 5 different threads, each opens a stream to a usb cam and has it's own instance of ofxCIDetector and detectFaceFeatures.
So far the frame rate drops down to 6 fps, more or less the same fps i get without threading it.
I uploaded the code slightly modified so that is opens 5 streams to the same webcam, since not everyone has 5 cams available.

# Dependencies

ofxCoreImageDetector
https://github.com/Ahbee/ofxCoreImageDetector

# Operating systems

Has only been tested on osx 10.10 with OF 0.8.4.