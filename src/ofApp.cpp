#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    camWidth = 640;
    camHeight = 480;

    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(camWidth, camHeight);    // ratio 4:3
    
    colorImg.allocate(camWidth,camHeight);
    grayImage.allocate(camWidth,camHeight);
    grayBg.allocate(camWidth,camHeight);
    grayDiff.allocate(camWidth,camHeight);
    grayDiffAccumulate.allocate(camWidth, camHeight);
    
    bLearnBakground = true;
    bInitRun = true;
    threshold = 80;
}

//--------------------------------------------------------------
void ofApp::update(){
    ofBackground(100,100,100);
    bool bNewFrame = false;
    vidGrabber.update();
    bNewFrame = vidGrabber.isFrameNew();
    
    if (bNewFrame){
        colorImg.setFromPixels(vidGrabber.getPixels());
        grayImage = colorImg;
        if(bInitRun){
            grayBg = grayImage;
            bInitRun = false;
        }
        
        // take the abs value of the difference between background and incoming and then threshold
        grayDiff.absDiff(grayBg, grayImage);
        grayDiff.blurHeavily();
        grayDiff.threshold(threshold);
        grayDiffAccumulate.convertToRange(0, 240);
        grayDiff.blurHeavily();
        grayDiffAccumulate += grayDiff;
        if (bLearnBakground){
            grayBg = grayImage;
        }
        
        // find contours which are between the size of 10 pixels and the w*h pixels
        // find holes is set to true so we will get interior contours as well
        contourFinder.findContours(grayDiff, 10, (camWidth*camHeight), 5, true);
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofSetHexColor(0xffffff);
    grayDiffAccumulate.draw(20, 20);
    colorImg.draw(camWidth+40,20,camWidth/2, camHeight/2);

    // draw the contours:
    ofFill();
    ofSetHexColor(0x333333);
    ofDrawRectangle(camWidth+40,(camHeight/2)+20,camWidth/2,camHeight/2);
    ofSetHexColor(0xffffff);
    
    // draw the whole contour finder
    contourFinder.draw(camWidth+40,(camHeight/2)+20,camWidth/2, camHeight/2);
    
    // draw each blob individually from the blobs vector,
    for (int i = 0; i < contourFinder.nBlobs; i++){
//        contourFinder.blobs[i].draw((camWidth/10)+40,(camHeight/10)+20);
        ofSetColor(255);
        // draw over the centroid if the blob is a hole
        if(contourFinder.blobs[i].hole){
            ofDrawBitmapString("hole",
                               contourFinder.blobs[i].boundingRect.getCenter().x + 360,
                               contourFinder.blobs[i].boundingRect.getCenter().y + 540);
        }
    }
    
    ofSetHexColor(0xffffff);
    stringstream reportStr;
    reportStr << "bg subtraction and blob detection" << endl
    << "press ' ' to capture bg" << endl
    << "threshold " << threshold << " (press: +/-)" << endl
    << "num blobs found " << contourFinder.nBlobs << ", fps: " << ofGetFrameRate();
    ofDrawBitmapString(reportStr.str(), 20, 600);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch (key){
        case ' ':
            bLearnBakground = true;
            break;
        case '+':
            threshold++;
            if (threshold > 255) threshold = 255;
            break;
        case '-':
            threshold--;
            if (threshold < 0) threshold = 0;
            break;
    }
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

