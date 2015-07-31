#include "ofApp.h"
using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
    
    versionNum = "v_1";
    
    ofSetVerticalSync(true);
    
    //ofSetWindowPosition(-500, 0);
    //ofSetFullscreen(true);
    
    //-----------GUI
    int gui_x = 10;
    int gui_y = 20;
    int gui_w = 210;
    int gui_h = 350;
    
    gui_main.setup("zoomer "+versionNum);
    gui_main.setPosition(gui_x,gui_y);
    gui_main.setDefaultHeaderBackgroundColor(ofColor(255,0,0));
    gui_main.add(fps.set("fps",0,0,120));
    
    gui_tracking.setup("tracking");
    gui_tracking.setPosition(gui_x+=gui_w,gui_y);
    gui_tracking.add(bShowCamera.set("showCamera",true));
    
    cout<<"load gui"<<endl;
    
    //load gui data
    gui_main.loadFromFile("gui_main.xml");
    gui_main.loadFromFile("gui_tracking.xml");
    
    
    bShowGUI = false;//true;
    //bShowCamera = false;
    //ofHideCursor();
    
    cout<<"done load gui"<<endl;
    
    camWidth 		= 1920/2;	// try to grab at this size.
    camHeight 		= 1080/2;
    
    //we can now get back a list of devices.
    
    string cameraName = "HD Pro Webcam C920";
    int deviceId = -1;
    
    ofVideoGrabber temp_grabber;
    
    
    vector<ofVideoDevice> devices = temp_grabber.listDevices();
    
    
    for(int i = 0; i < devices.size(); i++){
        string temp_deviceName = devices[i].deviceName;
        
        vector<string> splitName = ofSplitString(temp_deviceName, " ");
        cout << devices[i].id << ": " << devices[i].deviceName;
        if( devices[i].bAvailable && splitName[0] == "HD" && splitName[1] == "Pro"){
            deviceId = i;
            temp_validIDs.push_back(i);
                   
            cout << endl;
        }else{
            cout << " - unavailable " << endl;
        }
    }
    
    
    //videoTexture = vector<ofTexture>(temp_validIDs.size());
    //  videoTexture.resize(temp_validIDs.size());
    
    max_cams = 5;
    //  if(temp_validIDs.size() < max_cams) max_cams = temp_validIDs.size();
    
    cout<<"found max_cams "<<max_cams<<endl;
    
    for(int i=0; i<max_cams;i++){
        videoTexture[i].allocate(camWidth,camHeight, GL_RGB);
        croppedResizedTexture[i].allocate(camWidth,camHeight, GL_RGB);
        
        //just used for testing since this uses the same cam 5 times
        threadedObject[i].setup(0,camWidth,camHeight,false);
        
        //this uses 5 different cams
        //   threadedObject[i].setup(temp_validIDs[i],camWidth,camHeight);
        
        threadedObject[i].start();
        
    }
    
    //collectionDummy.allocate(30,30,GL_RGB);
    temp_grabber.close();
    
    
    x1 = 0;
    y1 = camHeight/3 * 2;
    
}

void ofApp::exit(){
    for(int i=0; i<max_cams;i++){
        if(threadedObject[i].isThreadRunning()){
            threadedObject[i].stop();
            ofSleepMillis(300);
        }
    }
}

void ofApp::saveGui(){
    gui_mainPos = gui_main.getPosition();
    
    gui_main.saveToFile("gui_main.xml");
    gui_tracking.saveToFile("gui_tracking.xml");

    
}

void ofApp::update() {
    
    fps = ofGetFrameRate();
    ofSetWindowTitle(ofToString(fps));
    
    //collection.clear();
    for(int i=0; i<max_cams;i++){
        threadedObject[i].lock();
        if(bShowCamera)   videoTexture[i].loadData(threadedObject[i].vidGrabber.getPixelsRef());
        // videoTexture[i].loadData(threadedObject[i].image);
        
        croppedResizedTexture[i].loadData(threadedObject[i].cropped);
        
       
        threadedObject[i].unlock();
    }
    
}

void ofApp::draw() {
    
    
    int t_x = 0;
    int t_y = 0;
    
    
    for(int i=0 ; i<max_cams;i++){
        ofSetHexColor(0xffffff);
        
        t_x = i*(camWidth/3);
        
        if(bShowCamera)  videoTexture[i].draw(t_x,t_y,camWidth/3,camHeight/3);
        
        
        ofPushMatrix();
        ofTranslate(t_x, t_y);
        ofScale(1/3.0, 1/3.0);
        threadedObject[i].draw();
        ofPopMatrix();
        
        croppedResizedTexture[i].draw(t_x,t_y+camHeight/3,camWidth/3,camHeight/3);
        
    }
    
    
    if(bShowGUI == true){
        gui_main.draw();
        gui_tracking.draw();
    }
    
    ofSetColor(0);
    ofDrawBitmapString("press g to see gui", 10, ofGetHeight()-30);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'f') ofToggleFullscreen();
    
    if (key == 'v') bShowCamera = !bShowCamera;
    
    if (key == 'a')
    {
        for(int i=0; i<max_cams;i++){
            threadedObject[i].start();
        }
    }
    else if (key == 's')
    {   for(int i=0; i<max_cams;i++){
        threadedObject[i].stop();
    }
    }
    
    //  if(ofGetKeyPressed(OF_KEY_CONTROL) && ofGetKeyPressed(OF_KEY_ALT)){
    
    if(key == 7 || key == 'g'){ //'g'
        bShowGUI = !bShowGUI;
        
        if(bShowGUI == false){
            ofHideCursor();
            saveGui();
        }else{
            ofShowCursor();
        }
    }
    
    if(key == 3 || key == 'c'){//'c'
        bShowCamera = !bShowCamera;
    }
    
    // } //end if(ofGetKeyPressed(OF_KEY_CONTROL) && ofGetKeyPressed(OF_KEY_ALT))
    
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}
