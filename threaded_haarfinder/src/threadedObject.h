#pragma once


#include "ofThread.h"
#include "ofxCv.h"

/// This is a simple example of a ThreadedObject created by extending ofThread.
/// It contains data (count) that will be accessed from within and outside the
/// thread and demonstrates several of the data protection mechanisms (aka
/// mutexes).

using namespace ofxCv;
using namespace cv;

class ThreadedObject: public ofThread
{
public:
    
   // ofPixels pixels;
    ofVideoGrabber vidGrabber;
    int 				camWidth;
    int 				camHeight;
    int deviceID;
    
    ofImage cropped;
        ofImage collectionDummy;

    
    //vector<ofPixels> collection;
    ofPixels collection[10];
    
        float collectionDuration, collectionTimer;
    
   // ofImage image;
    ofxCv::ObjectFinder objectFinder;
   // cv::Mat camMat;
    
    /// Create a ThreadedObject and initialize the member
    /// variable in an initialization list.
    ThreadedObject(): count(0)
    {
    }

    void setup(int _id, int _camW, int _camH, bool _useFeatures = false)
    {
               
        deviceID = _id;
        camWidth = _camW;
        camHeight = _camH;
        
        vidGrabber.listDevices();
        
        vidGrabber.setDeviceID(deviceID);
      //  vidGrabber.setDesiredFrameRate(60);
        vidGrabber.initGrabber(camWidth,camHeight);
        vidGrabber.setUseTexture(false);
        
        objectFinder.setup(ofToDataPath("haarcascade_frontalface_default.xml"));
        objectFinder.setPreset(ObjectFinder::Fast);
       // objectFinder.setPreset(ObjectFinder::Accurate);
        
      //  bUseFeatures = _useFeatures;
        
//        image.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);
//        image.setUseTexture(false);

        cropped.allocate(camWidth,camHeight, OF_IMAGE_COLOR);
        cropped.setUseTexture(false);

        collectionDummy.allocate(30, 30, OF_IMAGE_COLOR);
        collectionDummy.setUseTexture(false);
        
        collectionDuration = 0.2;
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
                // The mutex is now locked
                
                vidGrabber.update();
                
                if (vidGrabber.isFrameNew()){
                    
                  //  if(deviceID == 2) cout<<"-----------------------vidGrabber.isFrameNew()"<<endl;
                    
                    // int totalPixels = camWidth*camHeight*3;
                    // pixels = vidGrabber.getPixelsRef();
                    
                    //[ error ] ofImage: putBmpIntoPixels(): unable to set ofPixels from FIBITMAP
                    //  image.setFromPixels(vidGrabber.getPixelsRef());
                    //   image.update();
                    //image.setImageType(OF_IMAGE_GRAYSCALE);
                    
                    objectFinder.update(vidGrabber);
                    
                 //   collection.clear();
                    
                    if(objectFinder.size() > 0) {
                        
                        
                        Mat camMat = toCv(vidGrabber);
                        
                        for(int n =0; n<objectFinder.size(); n++){
                          
                            
                            cv::Rect roi = toCv(objectFinder.getObject(n));
                            
                            Mat croppedCamMat(camMat, roi);
                            resize(croppedCamMat, cropped);
                            cropped.update();
                            
                            
                            
                            // if(ofGetElapsedTimef() - collectionTimer > collectionDuration){
                            
                            resize(croppedCamMat, collectionDummy);
                            collectionDummy.update();
                            /*
                            if(deviceID == 2){
                                string str = "img/"+ofGetTimestampString()+"_"+ofToString(deviceID)+"_"+ofToString(n)+".png";
                            collectionDummy.saveImage(str);
                                cout<<n<<" "<<str;
                                cout<<" xy "<<roi.x<<" , "<<roi.y <<endl;
                                
                            }
                            */
                            
                            //   collectionTimer = ofGetElapsedTimef();
                            //    collection.push_back(collectionDummy.getPixels());
                            
                            // ofPixels p;
                            // p.setFromPixels(collectionDummy.getPixels(), 30, 30, 3);
                            // collection.push_back(p);
                            collection[n].setFromPixels(collectionDummy.getPixels(), 30, 30, 3);
                            //  collection[n] =collectionDummy.getPixelsRef();
                            //  }
                        }
                    }
                    
                    
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
        

            objectFinder.draw();
        
        
       //  ofDrawBitmapStringHighlight(ofToString(collection.size()), 10,30);
        
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
