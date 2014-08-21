#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofEnableAlphaBlending();
    ofSetOrientation(OF_ORIENTATION_DEFAULT);
    ofBackground(255,255,0);
	ofSetFrameRate(60);
    
    stateApp=STATEMENU;
    text.loadFont("fonts/Roboto-Regular.ttf", 26);
    title.loadFont("fonts/Roboto-Regular.ttf", 32);
    title.setLetterSpacing(1.5);
    alpha = 0;
    grow = true;
    
    //uploadDATA
    // we don't want to be running to fast
	ofSetVerticalSync(true);
	//our send and recieve strings
	msgTx= "5\n";
	msgRx= "5\n";
	//are we connected to the server - if this fails we
	//will check every few seconds to see if the server exists
    ip="172.20.10.12";
    port=8888;
	weConnected = tcpClient.setup(ip, port);
	//optionally set the delimiter to something else.  The delimter in the client and the server have to be the same
	tcpClient.setMessageDelimiter("\n");
	connectTime = 0;
	deltaTime = 0;
    
    // accelerometer part
    ofxAccelerometer.setup();
    ofxAccelerometer.setForceSmoothing(.9);
    motion.set(0,0,0);
    //homeLocation.set(0,0);
    provX =0.1;
    provY= 0.1;
    provZ= 1;
    timeRecord=ofGetElapsedTimeMillis();
    timeRecordTotal=0;
    timeRecordStart=0;
    motionDay=ofGetDay();
    
    //no sleep mode
    [[UIApplication sharedApplication] setIdleTimerDisabled:YES];
     
    //GPS part
    //coreLocation = new ofxiOSCoreLocation();
    //hasGPS = coreLocation->startLocation();
    //lat=0;
    //lon=0;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    switch(stateApp){
        case(STATEMENU):
            updateMenu();
            break;
        case(STATERECORD):
            updateRecord();
            break;
        case(STATELAMP):
            updateLamp();
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::updateMenu(){
    //reset every day from 6 o'clocks
    if(ofGetHours()>=6 and motionDay!=ofGetDay()){
        motion.set(0,0,0);
        motionDay=ofGetDay();
    }
}

//--------------------------------------------------------------
void ofApp::updateRecord(){
    updateAlpha();
    float timeTmp=ofGetElapsedTimeMillis();
    if(timeTmp-timeRecord>100){
        if(abs(ofxAccelerometer.getForce().x)>0.1 and abs(abs(ofxAccelerometer.getForce().x)-provX)>0.3){
            provX=abs(ofxAccelerometer.getForce().x);
            motion.x += abs(ofxAccelerometer.getForce().x);
        }
        if(abs(ofxAccelerometer.getForce().y)>0.1 and abs(abs(ofxAccelerometer.getForce().y)-provY)>0.3){
            provY=abs(ofxAccelerometer.getForce().y);
            motion.y += abs(ofxAccelerometer.getForce().y);
        }
        if(abs(ofxAccelerometer.getForce().z)>0.4 and abs(abs(ofxAccelerometer.getForce().z)-provZ)>0.2){
            provZ=abs(ofxAccelerometer.getForce().z);
            motion.z += abs(ofxAccelerometer.getForce().z)/10;
        }
        timeRecord=timeTmp;
    }

    //GPS
    /*if(hasGPS){
        //cout<<coreLocation->getLatitude()<<" | "<< coreLocation->getLatitude() <<endl;
        lat=coreLocation->getLatitude();
        lon=coreLocation->getLongitude();
    }*/
}

//--------------------------------------------------------------
void ofApp::updateLamp(){
    //we are connected - lets send our text and check what we get back
	if(!weConnected){
		//if we are not connected lets try and reconnect every 5 seconds
		deltaTime = ofGetElapsedTimeMillis() - connectTime;
        
		if( deltaTime > 5000 ){
			weConnected = tcpClient.setup(ip, port);
			connectTime = ofGetElapsedTimeMillis();
		}
        
	}

}

//Blending string
//--------------------------------------------------------------
void ofApp::updateAlpha(){
    //Blending string
    if (alpha >= 255)
        grow = false;
    else if (alpha<=0)
        grow = true;
    if (grow)
        alpha=alpha+5;
    else
        alpha=alpha-5;
}


//--------------------------------------------------------------
void ofApp::draw(){
    switch(stateApp){
        case(STATEMENU):
            drawMenu();
            break;
        case(STATERECORD):
            drawRecord();
            break;
        case(STATELAMP):
            drawLamp();
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::drawMenu(){
    ofBackground(225, 225, 225);
    //Titre
    ofSetColor(0,0,0, 240);
    ofFill();
    ofRect(0, 0, ofGetWidth(), ofGetHeight()/8);
    ofSetColor(255, 255,255);
    msg = "FIREPLACE";
    title.drawString(msg, (ofGetWidth()-title.stringWidth(msg))/2, ofGetHeight()/13);
    //Date
    ofSetColor(0,0,0, 50);
    ofFill();
    ofRect(0, ofGetHeight()/8, ofGetWidth(), ofGetHeight()/12);
    ofSetColor(255,255,255);
    msg = "TODAY          " + ofToString(ofGetDay()) + "/" + ofToString(ofGetMonth()) + "/" + ofToString(ofGetYear());
    text.drawString(msg, (ofGetWidth()-text.stringWidth(msg))/2, ofGetHeight()/6+10);
    //Record mode
    ofSetColor(224,0,0,200);
    ofFill();
    ofRect(0, ofGetHeight()/3, ofGetWidth(), ofGetHeight()/8);
    msg = "RECORD ACTIVITIES";
    ofSetColor(255,255,255);
    text.drawString(msg, (ofGetWidth()-text.stringWidth(msg))/2, ofGetHeight()/2-90);
    //Lamp mode
    ofSetColor(255,220,0,190);
    ofFill();
    ofRect(0, ofGetHeight()/2+90, ofGetWidth(), ofGetHeight()/8);
    msg = "UPLOAD TO DEVICE";
    ofSetColor(255,255,255);
    text.drawString(msg, (ofGetWidth()-text.stringWidth(msg))/2, ofGetHeight()/3*2);
    //Stats
    /*ofSetColor(0, 0, 0, 180);
    ofFill();
    ofRectRounded(0, ofGetHeight()-ofGetHeight()/7, ofGetWidth(), ofGetHeight()/5, 20);
    ofSetColor(255,255,255);
    msg = "STATS";
    text.drawString(msg, (ofGetWidth()-text.stringWidth(msg))/2, ofGetHeight()-ofGetHeight()/15);*/
}

//--------------------------------------------------------------
void ofApp::drawRecord(){
    ofBackground(225, 225, 225);
    //Title
    ofSetColor(0,0,0,240);
    ofFill();
    ofRect(0, 0, ofGetWidth(), ofGetHeight()/8);
    ofSetColor(255, 255, 255);
    msg = "FIREPLACE";
    title.drawString(msg, (ofGetWidth()-title.stringWidth(msg))/2, ofGetHeight()/13);
    // Stop
    ofSetColor(0,0,0);
    msg = "STOP";
    title.drawString(msg, (ofGetWidth()-title.stringWidth(msg))/2, ofGetHeight()/2);
    //Recording
    ofSetColor(0, 0, 0, 180);
    ofFill();
    ofRectRounded(0, ofGetHeight()-ofGetHeight()/7, ofGetWidth(), ofGetHeight()/5, 20);
    ofSetColor(255, 0, 56, alpha);
    msg = "RECORDING";
    text.drawString(msg, (ofGetWidth()-text.stringWidth(msg))/2, ofGetHeight()-ofGetHeight()/15);
}

//--------------------------------------------------------------
void ofApp::drawLamp(){
    ofBackground(225, 225, 225);
    //Title
    ofSetColor(0,0,0,240);
    ofFill();
    ofRect(0, 0, ofGetWidth(), ofGetHeight()/8);
    ofSetColor(255, 255, 255);
    msg = "FIREPLACE";
    title.drawString(msg, (ofGetWidth()-title.stringWidth(msg))/2, ofGetHeight()/13);
    // Stop
    ofSetColor(0,0,0);
    msg = "STOP";
    title.drawString(msg, (ofGetWidth()-title.stringWidth(msg))/2, ofGetHeight()/2);
}

//--------------------------------------------------------------
void ofApp::exit(){
    
}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){
    switch(stateApp){
        case(STATEMENU):
            touchDownMenu(touch);
            break;
        case(STATERECORD):
            touchDownRecord(touch);
            break;
        case(STATELAMP):
            touchDownLamp(touch);
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::touchDownMenu(ofTouchEventArgs & touch){
    if (touch.y<ofGetHeight()/2 and touch.y>ofGetHeight()/6){
        stateApp=STATERECORD;
        timeRecordStart=ofGetElapsedTimeMillis();
    }
    else if(touch.y>ofGetHeight()/3*2-50 and touch.y< ofGetHeight()-ofGetHeight()/8){
        //homeLocation.set(lat,lon);
        motionAnalysis(motion);
        stateApp=STATELAMP;
        uploadDATA();
    }
}

//--------------------------------------------------------------
void ofApp::touchDownRecord(ofTouchEventArgs & touch){
    if (touch.x >=(ofGetWidth()-text.stringWidth(msg)-60)/2 and touch.x<= (ofGetWidth()+text.stringWidth(msg)+60)/2 and touch.y >=ofGetHeight()/2-text.getLineHeight()-50 and touch.y<= ofGetHeight()/2+text.getLineHeight()+50){
        stateApp=STATEMENU;
        timeRecordTotal+=(ofGetElapsedTimeMillis()-timeRecordStart);
    }
}

//--------------------------------------------------------------
void ofApp::touchDownLamp(ofTouchEventArgs & touch){
    if (touch.x >=(ofGetWidth()-text.stringWidth(msg)-60)/2 and touch.x<= (ofGetWidth()+text.stringWidth(msg)+60)/2 and touch.y >=ofGetHeight()/2-text.getLineHeight()-50 and touch.y<= ofGetHeight()/2+text.getLineHeight()+50){
        stateApp=STATEMENU;
        msgTx = "5\n"; //OFF
        uploadDATA();
    }
}

//--------------------------------------------------------------
void ofApp::uploadDATA(){
	if(weConnected){
		if(tcpClient.send(msgTx)){
			//if data has been sent lets update our text
			string str = tcpClient.receive();
			if( str.length() > 0 ){
				msgRx = str;
			}
		}else if(!tcpClient.isConnected()){
			weConnected = false;
		}
	}else{
        while(!weConnected)
            updateLamp();
        uploadDATA();
	}
}


//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::motionAnalysis(ofPoint value){
    float sum;
    if(motion.x>0 or motion.y>0 or motion.z>0)
        sum= ((motion.x + motion.y + motion.z)/(timeRecordTotal/1000))*100;
    else
        sum=0;
    if (sum==0)
        msgTx = "5\n"; //NO ACTIVITIES
    else if (sum<20)
        msgTx = "1\n"; //SEDENTARY ACTIVITIES
    else if(sum<50 and sum>=20)
        msgTx = "2\n"; //LIGHT ACTIVITIES
    else if(sum<200 and sum>=50)
        msgTx = "3\n"; //MODERATE ACTIVITIES
    else if(sum>=200)
        msgTx = "4\n"; //VIGOROUS ACTIVITIES
}


//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch){
}


//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchCancelled(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::lostFocus(){
    
}

//--------------------------------------------------------------
void ofApp::gotFocus(){
    
}

//--------------------------------------------------------------
void ofApp::gotMemoryWarning(){
    
}

//--------------------------------------------------------------
void ofApp::deviceOrientationChanged(int newOrientation){
    
}
