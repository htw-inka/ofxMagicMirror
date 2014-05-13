//
//  MagicMirrorApp.cpp
//  ARMagicMirrorARTP
//
//  Author: Philipp Jährling, Markus Konrad
//  Copyright (c) 2013 INKA Research Group. All rights reserved.
//

#include "MagicMirrorApp.h"
#include "Tools.h"
#include "VidData.h"
#include "InfoAnimation.h"
#include "conf.h"

//--------------------------------------------------------------
void MagicMirrorApp::setup(){
    
    // camera connection
    camGrabber = new ofVideoGrabber();
    camGrabber->setDeviceID(CONF_CAM_ID);
    camGrabber->initGrabber(CONF_CAM_W, CONF_CAM_H);
    camBuffer = new unsigned char[CONF_CAM_W * CONF_CAM_H * 3];
    backgroundTexture.allocate(CONF_CAM_W, CONF_CAM_H, GL_RGB);
    
    // create a view object, will handle the drawing stuff
    view = new GLView(CONF_CAM_W, CONF_CAM_H);
    
    // create an init the marker-tracker and load camera intrinsic calibration file
    tracker = new ARToolKitPlus::TrackerSingleMarker(CONF_CAM_W, CONF_CAM_H);
    
    char* absolutPath;
    Tools::copyStringToBuf(ofDirectory(CONF_CAM_FILE).getAbsolutePath(), &absolutPath);
    
    if ( tracker->init(absolutPath, 1.0f, 100.0f) ) {
        configureTracker();
    } else {
        delete camGrabber;
        delete view;
        std::exit(1);
    }
    
    // get instance of the content loader
    content = ContentLoader::getInstance();
    content->loadContents(CONF_CONTENT_DIR);
    
    // to ensure the window has the configured size (could be changed by fullscreen mode)
    ofSetWindowShape(CONF_CAM_W, CONF_CAM_H);
    if (ofGetScreenWidth() < CONF_CAM_W) {
        // Window is bigger then Screen ... isl case ... move max to left (screen top)
        ofSetWindowPosition( (ofGetScreenWidth() - CONF_CAM_W), (ofGetScreenHeight() - CONF_CAM_H)/2 );
    }
    
    infoAnimation = new InfoAnimation();
#if CONF_INFO_SHOW_ON_START == 1
    lastTimeMarkerFound = 0;
#else
    lastTimeMarkerFound = Tools::getMicros() / 1000L;
#endif
}

//--------------------------------------------------------------
void MagicMirrorApp::update(){
    camGrabber->update();                       // calls grabeframe function
    if ( camGrabber->isFrameNew() ){            // is there any new pixel data
        camBuffer = camGrabber->getPixels();
        
        // load texture from camera pixel buffer
        backgroundTexture.loadData(camBuffer, CONF_CAM_W, CONF_CAM_H, GL_RGB);

        // find marker
        tracker->calc(camBuffer, &detMarker, &detMarkerCount);
        logFoundMarkerIDs();
        
        // get current time in ms and handle the Content Elements related to the found marker
        unsigned long nowMs = Tools::getMicros() / 1000L;
        addElementForFoundMarker(nowMs);
        updateVisibleElements(nowMs);
        
        // ToDo ... vielleicht geht das noch schöner? So is irgendwie unelegant
        if (visibleElements.empty()) {
            if (lastTimeMarkerFound + CONF_INFO_SHOW_ANI_AFTER_MILSEC < nowMs) {
                infoAnimation->moveIn();
            }
        } else {
            lastTimeMarkerFound = nowMs;
            infoAnimation->moveOut();
        }
    }
    
    infoAnimation->update();
}

//--------------------------------------------------------------
void MagicMirrorApp::addElementForFoundMarker(unsigned long nowMs) {
    // go through all found markers and check if it's related element is alredy visible
    for (unsigned int i = 0; i < detMarkerCount; i++) {
        ARToolKitPlus::ARMarkerInfo marker = detMarker[i];
        // check if the marker is known (the marker array includes everything which could be a marker and marks it as -1)
        if (marker.id > 0) {
            tracker->selectDetectedMarker(marker.id);
            ContentElement *elem = content->getContentById(marker.id);
            
            if (elem) {
                // add a new element if it is not already in curDetectedElems
                if (visibleElements.find(elem) == visibleElements.end()) {
                    visibleElements.insert(elem);
                }
                
                // if the element is a video, it could be stopped
                if (elem->type == VID) {
                    VidData* elemVid = ((VidData*)elem->typeData);
                    // this will only resume the video if it is paused
                    elemVid->resumeVideo();
                }
                
                // update the time the marker was detected the last time and the position
                elem->lastTimeDetected = nowMs;
                // update the ModelView Matrix (the function returns the pointer to the Matrix it holds, it will change with every tracking)
                elem->setModelViewMatrix((GLfloat*)tracker->getModelViewMatrix());
            }
        }
    }
}

//--------------------------------------------------------------
void MagicMirrorApp::updateVisibleElements(unsigned long nowMs) {
    set<ContentElement*>::iterator elemIt;
    for (elemIt = visibleElements.begin(); elemIt != visibleElements.end(); ++elemIt) {
        ContentElement* elem = *elemIt;
        
        // check if the marker has timed out
        if (elem->lastTimeDetected + CONF_RECOGNITION_TIMEOUT_MILLISEC < nowMs) {
            
            // if the element is a video pause it first
            if (elem->type == VID) {
                VidData* elemVid = ((VidData*)elem->typeData);
                elemVid->pauseVideo();
                
                // now it's really time to stop the video
                if (elem->lastTimeDetected + CONF_RESTART_VIDEO_AFTER_MILLISEC < nowMs) {
                    elemVid->stopVideo();
                    cout << "INFO: marker with id " << elem->markerId<< " timed out" << endl;
                    elem->resetCamParams();
                    visibleElements.erase(elemIt);
                }
                
            } else {
                cout << "INFO: marker with id " << elem->markerId << " timed out" << endl;
                elem->resetCamParams();
                visibleElements.erase(elemIt); // erase if it's not for video
            }
        }
    }
    
}

//--------------------------------------------------------------

void MagicMirrorApp::draw(){
    // draw the background
    view->drawBackground(backgroundTexture);
    infoAnimation->draw();
    
    // set the projection Matrix once
    ofSetMatrixMode(OF_MATRIX_PROJECTION);
    ofPushMatrix();
        view->setProjection(tracker->getProjectionMatrix());
        // call the view object draw functions, which will handle the drawing
        set<ContentElement*>::iterator elemIt;
        for (elemIt = visibleElements.begin(); elemIt != visibleElements.end(); ++elemIt) {
            ContentElement* elem = *elemIt;
            view->drawElement(elem);
        }
    ofPopMatrix();
}

//--------------------------------------------------------------
void MagicMirrorApp::keyReleased(int key){
    switch (key) {
        case 'f':
            ofToggleFullscreen();
            break;
        
        case 's':
            view->toggleSolidOcclusionCubeDisplay();
            break;
    }
}

//--------------------------------------------------------------
void MagicMirrorApp::logFoundMarkerIDs() {
    if (detMarkerCount > 0) {
        cout << "INFO: " << detMarkerCount << " markers found. IDs:";
        for (unsigned int i = 0; i < detMarkerCount; i++) {
            cout << " " << detMarker[i].id;
        }
        cout << endl;
    }
}

//--------------------------------------------------------------
void MagicMirrorApp::configureTracker(){
    tracker->setPatternWidth(2.0); // define size of the marker in OpenGL units
    tracker->activateAutoThreshold(true);
    
// use simple markers or BCH ones (thiner border)
#if CONF_MARKER_USE_SIMPLE == 1
    tracker->setBorderWidth(0.25);
    tracker->setMarkerMode(ARToolKitPlus::MARKER_ID_SIMPLE);
#else
    tracker->setBorderWidth(0.125);
    tracker->setMarkerMode(ARToolKitPlus::MARKER_ID_BCH);
#endif
    
// calculate lense undistortion? usefull with wide angle lenses
#if CONF_FRAME_UNDISTORT == 1
    tracker->setUndistortionMode(ARToolKitPlus::UNDIST_STD);
#else
    tracker->setUndistortionMode(ARToolKitPlus::UNDIST_NONE);
#endif
    
//  process just the half resolution image (for high resolution images or low performance machines)
#if CONF_PROCESS_HALF_RES == 1
    tracker->setImageProcessingMode(ARToolKitPlus::IMAGE_HALF_RES);
#else
    tracker->setImageProcessingMode(ARToolKitPlus::IMAGE_FULL_RES);
#endif
}