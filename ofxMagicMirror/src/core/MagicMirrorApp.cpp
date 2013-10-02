#include "MagicMirrorApp.h"

//--------------------------------------------------------------
MagicMirrorApp::~MagicMirrorApp(){
    // destroy the content loader
    if (content) content->destroy();
    content = NULL;
}

//--------------------------------------------------------------
void MagicMirrorApp::setup(){
    const size_t camFilePathLen = 1024;
    char camFilePath[camFilePathLen] = {0};
    snprintf(camFilePath, camFilePathLen, CONF_CAM_FILE_FORMAT, CONF_CAM_ID);
    
    initCam(CONF_CAM_ID, camFilePath);
    initDetection(CONF_MARKER_SIZE_METERS);
    
    content = ContentLoader::getInstance();
    content->loadContents(CONF_CONTENT_DIR);
    
    view.init(this);
    view.resize(camCap.getWidth(), camCap.getHeight());
}

//--------------------------------------------------------------
void MagicMirrorApp::update(){
    camCap.update();
    
    if (camCap.isFrameNew()) {    // if we have a new frame, process it
        cv::Mat input(camCap.getHeight(), camCap.getWidth(), CV_8UC3, camCap.getPixels());
        curFrame = input;
        frameProc.processFrame(curFrame, dispFrame);
    }
}

//--------------------------------------------------------------
void MagicMirrorApp::draw(){
    if (dispFrame.rows == 0) //prevent from going on until the image is initialized
        return;
    
    // call the view to draw the frame and found markers
    view.draw(dispFrame, markers);
}

//--------------------------------------------------------------
void MagicMirrorApp::initCam(const int camId, const char * const camConf) throw (cv::Exception){
    // open the camera with the device id
    camCap.setDeviceID(camId);
    camCap.initGrabber(CONF_CAM_W, CONF_CAM_H, false);
    
    cv::Size frameSize(camCap.getWidth(), camCap.getHeight());
    
    //read camera parameters
    camParams.readFromXMLFile(ofToDataPath(string(camConf)));
    
    // resize according to camera input frame size
    camParams.resize(frameSize);
    ofSetWindowShape(frameSize.width, frameSize.height);
}

//--------------------------------------------------------------
void MagicMirrorApp::initDetection(const float arucoMarkerSize) throw (cv::Exception){
    markerSize = arucoMarkerSize;
    
    // init the frame processor
    frameProc.init(markerSize, this);

    // set the correct marker size
    view.setMarkerSize(markerSize);
}

//--------------------------------------------------------------
void MagicMirrorApp::keyReleased(int key){
    if (key == 'f') {
        ofToggleFullscreen();
    }
}
