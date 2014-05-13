//
//  VidData.cpp
//  ARMagicMirrorARTP
//
//  Author: Philipp Jährling
//  Copyright (c) 2013 INKA Research Group. All rights reserved.
//

#include "VidData.h"
#include "Tools.h"

//--------------------------------------------------------------
VidData::VidData(ofVideoPlayer* vidPlayer, bool alpha) {
    this->vidPlayer     = vidPlayer;
    this->startPlayback = true;
    this->alpha         = alpha;
    this->w             = vidPlayer->getWidth();
    this->h             = vidPlayer->getHeight();
}

//--------------------------------------------------------------
VidData::~VidData() {
    if (vidPlayer) delete vidPlayer;
}

//--------------------------------------------------------------
void VidData::draw() {
    ofPushMatrix();
    // video was not shown for a certain time. rewind the video
    if (!vidPlayer->isPaused()) {
    
        // update the video frame texture if necessary
        vidPlayer->update();
        if (vidPlayer->isFrameNew()) {
            unsigned char* vidBuffer = vidPlayer->getPixels();
            vidTex.loadData(vidBuffer, w, h, GL_RGB);
        }
        
        // draw the frame
        if (alpha) {   // we have an alpha-channel
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);
        }
        
        vidTex.draw(0,0);
            
        if (alpha) {   // we have an alpha-channel
            glDisable(GL_BLEND);
            glBlendFunc(GL_ONE,GL_ZERO);
        }
    }
    ofPopMatrix();
}

//--------------------------------------------------------------
void VidData::pauseVideo() {
    if (!vidPlayer->isPaused()) {
        vidPlayer->setPaused(true);
        cout << "INFO: video paused" << endl;
    }
}

//--------------------------------------------------------------
void VidData::resumeVideo() {
    // video was not shown for a certain time. rewind the video
    if (vidPlayer->isPaused()) {
        cout << "resuming video player playback" << endl;
        vidPlayer->setPaused(false);
    } else if (startPlayback) {
        vidPlayer->play();
        startPlayback = false;
    }
}

//--------------------------------------------------------------
void VidData::stopVideo() {
    vidPlayer->stop();
    vidPlayer->setFrame(0);
    startPlayback = true;
    cout << "INFO: video stopped" << endl;
}

//--------------------------------------------------------------
int VidData::getWidth() {
    return w;
}

//--------------------------------------------------------------
int VidData::getHeight() {
    return h;
}

//--------------------------------------------------------------
bool VidData::isAlpha() {
    return alpha;
}