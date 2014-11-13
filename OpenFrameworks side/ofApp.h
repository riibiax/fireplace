#pragma once

#include "ofMain.h"
#include "ofxiOS.h"
#include "ofxiOSExtras.h"
#include "ofxNetwork.h"
#include "ofGLRenderer.h"

#define STATEMENU 0
#define STATERECORD 1
#define STATELAMP 2

class ofApp : public ofxiOSApp{
	
    public:
        void setup();
        void update();
        void draw();
        void exit();
    
        void touchDown(ofTouchEventArgs & touch);
        void touchMoved(ofTouchEventArgs & touch);
        void touchUp(ofTouchEventArgs & touch);
        void touchDoubleTap(ofTouchEventArgs & touch);
        void touchCancelled(ofTouchEventArgs & touch);
        void lostFocus();
        void gotFocus();
        void gotMemoryWarning();
        void deviceOrientationChanged(int newOrientation);
	
        //stateApp
        int stateApp;
        ofTrueTypeFont text;
        ofTrueTypeFont title;
        string msg;
        float alpha;
        bool grow;
    
        void drawMenu();
        void drawRecord();
        void drawLamp();
    
        void updateMenu();
        void updateRecord();
        void updateLamp();
        void updateAlpha();
    
        void touchDownMenu(ofTouchEventArgs & touch);
        void touchDownRecord(ofTouchEventArgs & touch);
        void touchDownLamp(ofTouchEventArgs & touch);

    
        //GPS part
       /* ofxiOSCoreLocation * coreLocation;
        bool hasGPS;
        float lon, lat;*/
     
        //upload part
        void uploadDATA();
        ofxTCPClient tcpClient;
        string msgTx, msgRx;
        int connectTime;
        int deltaTime;
        bool weConnected;
        string ip;
        int port;
    
    
        //accelerometer part
        void motionAnalysis(ofPoint motion);
        ofPoint motion;
       // ofPoint homeLocation;
        float provX;
        float provY;
        float provZ;
        float timeRecord;
        float timeRecordStart;
        float timeRecordTotal;
        int motionDay;
};


