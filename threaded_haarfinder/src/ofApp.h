#pragma once

#include "ofMain.h"
#include "threadedObject.h"

#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
        void exit();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
     void saveGui();
    
    string versionNum;
    
    int 				camWidth;
    int 				camHeight;
    
    vector<int> temp_validIDs;
    
    
    vector<ofTexture> collection;
    vector<ofImage> collection_img;
    // float collectionDuration, collectionTimer;
    
    int x1,y1;

    
    ThreadedObject threadedObject[5];
    ofTexture videoTexture[5];
    ofTexture croppedResizedTexture[5];
    //ofTexture croppedTexture[5];
    int max_cams;
    

    //----------GUI
    ofParameter<ofPoint> gui_mainPos;
    ofParameter<int> fps;
    
    bool bShowGUI;
    ofParameter<bool> bShowCamera;
    
    ofxPanel gui_main,gui_usbCam, gui_tracking, gui_image;
    
};
