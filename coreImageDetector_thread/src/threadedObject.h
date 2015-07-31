#pragma once


#include "ofThread.h"
#include "ofxCIDetector.h"

/// This is a simple example of a ThreadedObject created by extending ofThread.
/// It contains data (count) that will be accessed from within and outside the
/// thread and demonstrates several of the data protection mechanisms (aka
/// mutexes).
class ThreadedObject: public ofThread
{
public:
    
   // ofPixels pixels;
    ofVideoGrabber vidGrabber;
    int 				camWidth;
    int 				camHeight;
    int deviceID;
    
    bool bUseFeatures;
    ofxCIDetector detector;
    vector<shared_ptr<ofxCIFaceFeature> > detectedFaces;
 ofImage image,image_gray;
    
    /// Create a ThreadedObject and initialize the member
    /// variable in an initialization list.
    ThreadedObject(): count(0)
    {
    }

    void setup(int _id, int _camW, int _camH, bool _useFeatures = false)
    {
        vector<ofVideoDevice> devices = vidGrabber.listDevices();
        
        for(int i = 0; i < devices.size(); i++){
            cout << devices[i].id << ": " << devices[i].deviceName;
            if( devices[i].bAvailable ){
                cout << endl;
            }else{
                cout << " - unavailable " << endl;
            }
        }
        
        deviceID = _id;
        camWidth = _camW;
        camHeight = _camH;
        
        vidGrabber.listDevices();
        
        vidGrabber.setDeviceID(deviceID);
        vidGrabber.setDesiredFrameRate(60);
        vidGrabber.initGrabber(camWidth,camHeight);
        vidGrabber.setUseTexture(false);
        
        detector.setup(OFX_ACCURACY_LOW, true, .05);
        
        bUseFeatures = _useFeatures;
        
        image.allocate(camWidth, camHeight, OF_IMAGE_COLOR);
        image.setUseTexture(false);


    }
    /// Start the thread.
    void start()
    {
        
        // Mutex blocking is set to true by default
        // It is rare that one would want to use startThread(false).
        startThread();
    }

    /// Signal the thread to stop.  After calling this method,
    /// isThreadRunning() will return false and the while loop will stop
    /// next time it has the chance to.
    void stop()
    {
        stopThread();
    }

    /// Our implementation of threadedFunction.
    void threadedFunction()
    {
        while(isThreadRunning())
        {
            // Attempt to lock the mutex.  If blocking is turned on,
            if(lock())
            {
                // The mutex is now locked and the "count"
                // variable is protected.  Time to modify it.
                count++;

                // Here, we simply cause it to reset to zero if it gets big.
                if(count > 50000) count = 0;

        
                
                vidGrabber.update();
                
                if (vidGrabber.isFrameNew()){
                   // int totalPixels = camWidth*camHeight*3;
                   // pixels = vidGrabber.getPixelsRef();
                    
                    //[ error ] ofImage: putBmpIntoPixels(): unable to set ofPixels from FIBITMAP
                    image.setFromPixels(vidGrabber.getPixelsRef());
                    image.update();
                    
                    //image.setImageType(OF_IMAGE_GRAYSCALE);
               
                    //detectFaceFeatures(const ofImage &img, bool detectSmiles, bool detectClosedEyes,OFX_IMAGE_ORIENTATION imageOrientation)
           
                    
                   if(bUseFeatures) detectedFaces = detector.detectFaceFeatures(image, true, true);
                    else detectedFaces = detector.detectFaceFeatures(image, false, false);
                    
                  
//                    vidGrabber.getPixelsRef().rotate90To(p,ofMap(mouseX, 0, ofGetWidth(), 0, 3));
//                    p.update();
//                    
//                    unsigned char * pixels = vidGrabber.getPixels();
//                    for (int i = 0; i < totalPixels; i++){
//                        videoInverted[i] = 255 - pixels[i];
//                    }
//                    videoTexture.loadData(videoInverted, camWidth,camHeight, GL_RGB);
                }

                
                // Unlock the mutex.  This is only
                // called if lock() returned true above.
                unlock();

                // Sleep for 1 second.
               // sleep(1000);
            }
            else
            {
                // If we reach this else statement, it means that we could not
                // lock our mutex, and so we do not need to call unlock().
                // Calling unlock without locking will lead to problems.
                ofLogWarning("threadedFunction()") << "Unable to lock mutex.";
            }
        }
    }

    /// This drawing function cannot be called from the thread itself because
    /// it includes OpenGL calls (ofDrawBitmapString).
    void draw()
    {
        /*
        std::stringstream ss;

        ss << "I am a slowly increasing thread. " << std::endl;
        ss << "My current count is: ";

        if(lock())
        {
            // The mutex is now locked and the "count"
            // variable is protected.  Time to read it.
            ss << count;

            // Unlock the mutex.  This is only
            // called if lock() returned true above.
            unlock();
        }
        else
        {
            // If we reach this else statement, it means that we could not
            // lock our mutex, and so we do not need to call unlock().
            // Calling unlock without locking will lead to problems.
            ofLogWarning("threadedFunction()") << "Unable to lock mutex.";
        }

        ofDrawBitmapString(ss.str(), 50, 56);
         */
        
         ofNoFill();
        for (int i = 0; i < detectedFaces.size(); i++) {
            shared_ptr<ofxCIFaceFeature> &f = detectedFaces[i];
            ofSetColor(ofColor::green);
            ofRect(f->getBounds());
            // for some reason the first face detected is not tracked but evrything else is.
            // this bug might be fixed in the 64 bit version
            if (f->hasTrackingID()) {
                ofSetColor(ofColor::pink);
            //    font.drawString(ofToString(f->getTrackingID()), f->getBounds().getCenter().x,f->getBounds().getCenter().y);
                ofDrawBitmapString(ofToString(f->getTrackingID()), f->getBounds().getCenter().x,f->getBounds().getCenter().y);
            }
            
            if(bUseFeatures){
            ofSetColor(ofColor::yellow);
            if (f->hasLeftEyePosition()) {
                if(f->isLeftEyeClosed()) ofSetColor(ofColor::blue);
                ofCircle(f->getLeftEyePosition(), 10);
            }
            ofSetColor(ofColor::yellow);
            if (f->hasRightEyePosition()) {
                if(f->isRightEyeClosed()) ofSetColor(ofColor::blue);
                ofCircle(f->getRightEyePosition(), 10);
            }
             ofSetColor(ofColor::yellow);
            if (f->hasMouthPosition()) {
                if (f->hasSmile()) ofSetColor(ofColor::blue);
                ofCircle(f->getMouthPosition(), 15);
            }
         
            
            if (f->hasFaceAngle()) {
                ofLine(f->getMouthPosition(),f->getMouthPosition()+ofPoint(f->getFaceAngle()*5,-200));
                ofLog() << f->getFaceAngle();
            }
            }
        }
        
    }

    // Use ofScopedLock to protect a copy of count while getting a copy.
    int getCount()
    {
        ofScopedLock lock(mutex);
        return count;
    }

protected:
    // This is a simple variable that we aim to always access from both the
    // main thread AND this threaded object.  Therefore, we need to protect it
    // with the mutex.  In the case of simple numerical variables, some
    // garuntee thread safety for small integral types, but for the sake of
    // illustration, we use an int.  This int could represent ANY complex data
    // type that needs to be protected.
    int count;

};
