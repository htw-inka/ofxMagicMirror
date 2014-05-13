//
//  ObjData.cpp
//  ARMagicMirrorARTP
//
//  Author: Philipp Jährling
//  Copyright (c) 2013 INKA Research Group. All rights reserved.
//

#include "ObjData.h"
#include "Tools.h"

//--------------------------------------------------------------
ObjData::ObjData() {
    this->texId = 0;
    this->modelId = 0;
}

//--------------------------------------------------------------
ObjData::~ObjData() {
    if (texId > 0) Tools::deleteTexture(texId);
    if (modelId > 0) Tools::deleteModel(modelId);
}

//--------------------------------------------------------------
void ObjData::draw() {
    ofPushMatrix();
    if (modelId > 0) {
        
        if (texId > 0) {
            glEnable(GL_TEXTURE_2D);
            Tools::useTexture(texId);
        }
        
        ofDisableAlphaBlending();
        ofEnableLighting();
        ofEnableDepthTest();
        
        glColor3f(1.0f, 1.0f, 1.0f);
        
        Tools::drawModel(modelId);
        
        ofDisableLighting();
        ofDisableDepthTest();
        
        if (texId > 0) {
            glDisable(GL_TEXTURE_2D);
        }
    }
    ofPopMatrix();
}