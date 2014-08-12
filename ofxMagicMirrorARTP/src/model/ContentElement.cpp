//
//  ContentElement.cpp
//  ARMagicMirrorARTP
//
//  Author: Philipp Jährling, Markus Konrad, Alexander Godoba
//  Copyright (c) 2013 INKA Research Group. All rights reserved.
//

#include "ContentElement.h"
#include "conf.h"
#include "Tools.h"
#include "ImgData.h"
#include "VidData.h"

//--------------------------------------------------------------
ContentElement::ContentElement(int markerId, string name, ContentType type) {
    this->markerId          = markerId;
    this->name              = name;
    this->lastTimeDetected  = 0;
    this->type              = type;
    this->typeData          = NULL;
    this->elemScale         = 1;
    this->elemRot[0]        = 0;
    this->elemRot[1]        = 0;
    this->elemRot[2]        = 0;
    this->elemTrans[0]      = 0;
    this->elemTrans[1]      = 0;
    this->elemTrans[2]      = 0;
    
    // Explanation values
    this->showExpl          = false;
    this->explImg           = NULL;
    this->explTrans[0]      = 0;
    this->explTrans[0]      = 0;
    this->explTrans[0]      = 0;
    this->explScale         = 1;
    
    // By default the filter windows (arrays) are filled with 0
    this->camPosWindow = new float[CONF_SMOTHING_FILTER_WINDOW * 3];
    this->camRotWindow = new float[CONF_SMOTHING_FILTER_WINDOW * 3];
    this->resetCamParams();
}

//--------------------------------------------------------------
ContentElement::~ContentElement() {
    if (typeData) delete typeData;
    if (camPosWindow) delete camPosWindow;
    if (camRotWindow) delete camRotWindow;
}

//--------------------------------------------------------------
void ContentElement::resetCamParams() {
    for (int i = 0; i < CONF_SMOTHING_FILTER_WINDOW * 3; i++) {
        this->camPosWindow[i] = 0;
        this->camRotWindow[i] = 0;
    }
    
    memset(curCamPos, 0, 3 * sizeof(float));
    memset(curCamRot, 0, 3 * sizeof(float));
}

//--------------------------------------------------------------
void ContentElement::draw() {
    if (type != _UNKNOWN) {
        
        // put the element at the right place
        ofTranslate(elemTrans[0], elemTrans[1], elemTrans[2]);
        
        if (showExpl && explImg != NULL) {
            if (type == VID) {
                VidData* elemVid = ((VidData*)typeData);
                if (elemVid->isPaused()) {
                    return;
                }
            }
            
            ofPushMatrix();
                // Disable the overlap effect of the occlusion cube
                glDisable(GL_STENCIL_TEST);
                // place and draw the explanation image
                ofTranslate(explTrans[0], explTrans[1], explTrans[2]);
                ofScale(explScale, explScale, explScale);
                explImg->draw();
            ofPopMatrix();
        }
        
        ofPushMatrix();
            // scale up or down, according to config
            ofScale(elemScale, elemScale, elemScale);
        
            // rotate to fit model directions, according to config
            ofRotate(elemRot[0], 1.0f, 0.0f, 0.0f);
            ofRotate(elemRot[1], 0.0f, 1.0f, 0.0f);
            ofRotate(elemRot[2], 0.0f, 0.0f, 1.0f);
            // Enable the overlap effect of the occlusion cube
            glEnable(GL_STENCIL_TEST);
            typeData->draw();
        ofPopMatrix();
        
    }
}

//--------------------------------------------------------------
void ContentElement::setElemScaling(float scale) {
    if (scale > 0) {
        elemScale = scale;
    }
}

//--------------------------------------------------------------
void ContentElement::setElemRotation(float x, float y, float z) {
    elemRot[0] = x;
    elemRot[1] = y;
    elemRot[2] = z;
}

//--------------------------------------------------------------
void ContentElement::setElemTranslation(float x, float y, float z) throw (runtime_error) {
    float offX = 0, offY = 0, offZ = 0;
    if (typeData != NULL) {
        if (type == VID) {
            VidData* vidElem = (VidData*)typeData;
            offX = - (vidElem->getWidth()  / 2) * elemScale;
            offY = - (vidElem->getHeight() / 2) * elemScale;
        } else if (type == IMG) {
            ImgData* imgElem = (ImgData*)typeData;
            offX = - (imgElem->getWidth()  / 2) * elemScale;
            offY = - (imgElem->getHeight() / 2) * elemScale;
        }
    } else {
        throw runtime_error("Set type data object before element translation.");
    }
    
    elemTrans[0] = x + offX;
    elemTrans[1] = y + offY;
    elemTrans[2] = z + offZ;
}

//--------------------------------------------------------------
void ContentElement::setModelViewMatrix(GLfloat* mat) {
    if (CONF_SMOTHING_FILTER_WINDOW > 0) {
        float camPos[3];
        float camRot[3];
        Tools::decomposeModelViewMatrix(mat, camPos, camRot);
        pushCameraParams(camPos, camRot);
        
        // stop the content from "flying in" when filter window is not filled in total
        if (camPosWindow[0] == 0 || camPosWindow[1] == 0 || camPosWindow[2] == 0) {
            memcpy(modelViewMatrix, mat, 16 * sizeof(GLfloat));
        } else {
            // the following will change camPos and camRot again!!!
            calcCameraParams(curCamPos, curCamRot);
            Tools::composeModelViewMatrix(curCamPos, curCamRot, modelViewMatrix);
        }
    } else {
        memcpy(modelViewMatrix, mat, 16 * sizeof(GLfloat));
    }
}

//--------------------------------------------------------------
void ContentElement::setExplanation(ImgData* img, float scale, float x, float y, float z) {
    if (img != NULL) {
        showExpl        = true;
        explImg         = img;
        explTrans[0]    = x;
        explTrans[1]    = y;
        explTrans[2]    = z;
        explScale       = scale;
    }
}

//--------------------------------------------------------------
GLfloat* ContentElement::getModelViewMatrix() {
    return modelViewMatrix;
}

//--------------------------------------------------------------
void ContentElement::getCurCamRot(float camRot[3]) const {
    memcpy(camRot, curCamRot, 3 * sizeof(float));
}

//--------------------------------------------------------------
void ContentElement::getPosVec(float pos[3]) const {
    memcpy(pos, curCamPos, 3 * sizeof(float));
}


/*********************** PRIVATE METHODES **********************/
//--------------------------------------------------------------
void ContentElement::calcCameraParams(float* position, float* rotation) {
    // fill with default values
    position[0] = position[1] = position[2] = 0;
    
    // sum all "history" values
    for (int i = 0; i < CONF_SMOTHING_FILTER_WINDOW; i++) {
        position[0] += camPosWindow[i * 3    ];
        position[1] += camPosWindow[i * 3 + 1];
        position[2] += camPosWindow[i * 3 + 2];
    }
    
    // devide them by the number of "history" values
    position[0] /= CONF_SMOTHING_FILTER_WINDOW;
    position[1] /= CONF_SMOTHING_FILTER_WINDOW;
    position[2] /= CONF_SMOTHING_FILTER_WINDOW;
    
    // calculate the avarage rotation angle for all axes (n)
    for (int n = 0; n < 3; n++) {
        float buff[CONF_SMOTHING_FILTER_WINDOW];
        for (int i = 0; i < CONF_SMOTHING_FILTER_WINDOW; i++) {
            buff[i] = camRotWindow[i * 3 + n];
        }
        rotation[n] = Tools::getAverageAngle(buff, CONF_SMOTHING_FILTER_WINDOW);
    }
}

//--------------------------------------------------------------
void ContentElement::pushCameraParams(float* position, float* rotation) {
    // delete the oldest elements and move up the newer ones
    for (int i = 3; i < CONF_SMOTHING_FILTER_WINDOW * 3; i++) {
        camPosWindow[i - 3] = camPosWindow[i];
        camRotWindow[i - 3] = camRotWindow[i];
    }
    
    // put in the new elements
    camPosWindow[CONF_SMOTHING_FILTER_WINDOW * 3 - 3] = position[0];
    camPosWindow[CONF_SMOTHING_FILTER_WINDOW * 3 - 2] = position[1];
    camPosWindow[CONF_SMOTHING_FILTER_WINDOW * 3 - 1] = position[2];
    
    camRotWindow[CONF_SMOTHING_FILTER_WINDOW * 3 - 3] = rotation[0];
    camRotWindow[CONF_SMOTHING_FILTER_WINDOW * 3 - 2] = rotation[1];
    camRotWindow[CONF_SMOTHING_FILTER_WINDOW * 3 - 1] = rotation[2];
}