#pragma once


#include "ofMain.h"
#include "threadedObject.h" // include our ThreadedObject class.


class ofApp: public ofBaseApp
{
public:
    void setup();
    void exit();
    void update();
    void draw();
    
    void keyPressed(int key);
    
    ThreadedObject threadedObject[6];
    ofTexture videoTexture[6];
    ///< \brief Our threaded object.
    int mainAppsCount;
    ///< \brief A local count that is only accessed in the main thread
    
    int 				camWidth;
    int 				camHeight;
    vector<int> temp_validIDs;
    
    
    ofVideoGrabber temp_grabber;
    int max_cams;
};
