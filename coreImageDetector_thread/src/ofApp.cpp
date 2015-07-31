#include "ofApp.h"


void ofApp::setup()
{
    mainAppsCount = 0;
    
    camWidth 		= 1920/2;	// try to grab at this size.
    camHeight 		= 1080/2;
    
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
            
            //            cam.push_back(<#const value_type &__x#>)
            //            cam.setDeviceID(deviceId);
            //            cam.setDesiredFrameRate(60);
            //            cam.initGrabber(camWidth,camHeight);
            
            cout << endl;
        }else{
            cout << " - unavailable " << endl;
        }
    }
    
    max_cams = 5;
    //  if(temp_validIDs.size() < max_cams) max_cams = temp_validIDs.size();
    
    cout<<"found max_cams "<<max_cams<<endl;
    
    for(int i=0; i<max_cams;i++){
        videoTexture[i].allocate(camWidth,camHeight, GL_RGB);
        
        //just used for testing since this uses the same cam 5 times
        threadedObject[i].setup(0,camWidth,camHeight,false);
        
        //this uses 5 different cams
        //    threadedObject[i].setup(temp_validIDs[i],camWidth,camHeight);
        
        threadedObject[i].start();
        
    }
    
}

void ofApp::exit(){
    for(int i=0; i<max_cams;i++){
        if(threadedObject[i].isThreadRunning()){
            threadedObject[i].stop();
            ofSleepMillis(300);
        }
    }
}
void ofApp::update()
{
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    //  mainAppsCount++;
    
    for(int i=0; i<max_cams;i++){
        threadedObject[i].lock();
        //videoTexture[i].loadData(threadedObject[i].pixels);
        videoTexture[i].loadData(threadedObject[i].image);
        threadedObject[i].unlock();
    }
}

void ofApp::draw()
{
    
    int t_x = 0;
    int t_y = 0;
    
    for(int i=0 ; i<max_cams;i++){
        ofSetHexColor(0xffffff);
        
        t_x = i*(camWidth/3);
        
        videoTexture[i].draw(t_x,t_y,camWidth/3,camHeight/3);
        
        
        ofPushMatrix();
        ofTranslate(t_x, t_y);
        ofScale(1/3.0, 1/3.0);
        threadedObject[i].draw();
        ofPopMatrix();
        
        
        
        
    }
    
    
}

void ofApp::keyPressed(int key)
{
    
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
}
