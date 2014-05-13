//
//  ImgData.cpp
//  ARMagicMirrorARTP
//
//  Author: Philipp Jährling
//  Copyright (c) 2013 INKA Research Group. All rights reserved.
//

#include "ImgData.h"
#include "Tools.h"

//--------------------------------------------------------------
ImgData::ImgData(ofPixels texPixel, int width, int height, bool alpha) {
    this->w     = width;
    this->h     = height;
    this->alpha = alpha;
    
    if (alpha) {
        this->tex.loadData(texPixel, GL_RGBA);
    } else {
        this->tex.loadData(texPixel, GL_RGB);
    }
}

//--------------------------------------------------------------
ImgData::~ImgData() {
    // nothin here
}

//--------------------------------------------------------------
void ImgData::draw() {
    
    ofPushMatrix();
        glDisable(GL_LIGHTING);
    
        if (alpha) {   // alpha-channel texture
            // over marker
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);  // take the alpha channel of the image and blend it with the full background
        }
    
        // draw the image as a plane
        tex.draw(0,0);
    
        if (alpha) {  // we had an alpha-channel texture
            glDisable(GL_BLEND);
            glBlendFunc(GL_ONE,GL_ZERO);
        }
    ofPopMatrix();
}

//--------------------------------------------------------------
int ImgData::getWidth() {
    return w;
}

//--------------------------------------------------------------
int ImgData::getHeight() {
    return h;
}

//--------------------------------------------------------------
bool ImgData::isAlpha() {
    return alpha;
}