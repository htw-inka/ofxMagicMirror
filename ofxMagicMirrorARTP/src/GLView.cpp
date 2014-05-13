//
//  GLView.cpp
//  ARMagicMirrorARTP
//
//  Author: Markus Konrad, Philipp Jährling
//  Copyright (c) 2013 INKA Research Group. All rights reserved.
//

#include "ofMain.h"
#include "GLView.h"
#include "conf.h"
#include "Tools.h"

//--------------------------------------------------------------
GLView::GLView(int w, int h) {
    width    = w;
    height   = h;
    showSolidOcclusionCube  = false; // for testing set to true

    // anti-alias
#if CONF_ANTIALIAS == 1
    ofGetCurrentRenderer()->enableAntiAliasing();
#endif
    // clear stuff
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearDepth(1.0);
    
    // normalizing
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable(GL_NORMALIZE);
    
    // light stuff
    ofEnableLighting();
    
    defaultLight.setAmbientColor(ofFloatColor(0.2f, 0.2f, 0.2f));
    defaultLight.setDiffuseColor(ofFloatColor(0.5f, 0.5f, 0.5f));
    defaultLight.setSpecularColor(ofFloatColor(1.0f, 1.0f, 1.0f));
    defaultLight.setPointLight();   // light to all directions
    
    defaultLight.enable();
}

//--------------------------------------------------------------
GLView::~GLView() {
    // nothing to clean up
}

//--------------------------------------------------------------
void GLView::setProjection(const GLfloat *proj) {
    ofSetMatrixMode(OF_MATRIX_PROJECTION);
    ofLoadIdentityMatrix();
    ofMultMatrix(proj);
}

//--------------------------------------------------------------
void GLView::drawBackground(ofTexture backgroundTex) {
    // stuff needed to let the background tex "overlap" the content in certain rotaitions
    glClear(GL_STENCIL_BUFFER_BIT);
    glDisable(GL_STENCIL_TEST);
    glStencilMask(0x0);
    
    // actual draw the background texture
    ofDisableDepthTest();
    ofPushMatrix();
#if CONF_MIRROR_CAM_IMG == 1  // mirror the image?
        ofTranslate(CONF_CAM_W, 0, 0);
        ofScale(-1, 1, 1);
#else
        ofTranslate(0, CONF_CAM_H);
        ofScale(1, -1);
#endif
        backgroundTex.draw(0, 0);
    ofPopMatrix();
    ofEnableDepthTest();
}

//--------------------------------------------------------------
void GLView::drawElement(ContentElement* elem) {
    ofSetMatrixMode(OF_MATRIX_MODELVIEW);    
    ofPushMatrix();
        // Translation, Rotation, ...
        ofLoadIdentityMatrix();
    
#if CONF_MIRROR_CAM_IMG == 1
        ofScale(-1, -1, 1);
#endif
    
        ofMultMatrix(elem->getModelViewMatrix());
        ofScale(0.02, 0.02, 0.02); // scale down (100 units is around one marker now)
    
#if CONF_DEBUG == 0 /* NORMAL WAY*/
    
        if (elem->type == OBJ) {
            // only draw the occlusion fake cube when it can occlude the object, i.e. when
            // its rotation around the x axis is negative (object is seen from beneath)
            float elemRot[3];
            elem->getCurCamRot(elemRot);
            if (elemRot[0] < 0.0f) {
                drawOcclusionCube();
            }
        }
    
        // draw the content
        elem->draw();
    
#else   /* DEBUG WAY */
        drawMarkerID(elem);
        drawAxes();
#endif
}

//--------------------------------------------------------------
void GLView::drawOcclusionCube() {
    ofPushMatrix();
    
    if (showSolidOcclusionCube) {
        ofSetColor(255, 255, 255);
    } else {
        // set up GL stencil test to "draw" the mask
        glEnable(GL_STENCIL_TEST);
        glStencilMask(0x1);
        glStencilFunc(GL_ALWAYS, 0x1, 0x1);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glDepthMask(GL_FALSE);
    }
    
    const float cubeSize = 120.0f;
    glTranslatef(0.0f, 0.0f, -cubeSize / 2.0f);
    ofDrawBox(cubeSize);
    
    if (!showSolidOcclusionCube) {
        // set up GL stencil test to render an object applying the mask
        glStencilFunc(GL_NOTEQUAL, 0x1, 0x1);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glDepthMask(GL_TRUE);
    }
    
    ofPopMatrix();
}

//--------------------------------------------------------------
void GLView::drawText(string text) {
    int fontSize = 24;
    ofTrueTypeFont font;
    font.loadFont(CONF_FONT_PATH, fontSize, true, false, true, 0.3, 0);

    ofDisableLighting();
    ofEnableAlphaBlending();
    
    ofPushMatrix();
        ofSetColor(0, 0, 0, 200);
        ofRect(0,0, 100, fontSize + 20);
        ofSetColor(255, 255, 255);
        font.drawString(text, 10, 10);
    ofPopMatrix();
    
    ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void GLView::drawMarkerID(ContentElement* elem) {
    ofPushMatrix();
        ofTrueTypeFont font;
        font.loadFont(CONF_FONT_PATH, 48, true, false, true, 0.3, 0);
        char buffer [4];
        sprintf(buffer, "%d", elem->markerId);
        font.drawStringAsShapes(buffer, 0, 0);
    ofPopMatrix();
}

//--------------------------------------------------------------
void GLView::drawAxes() {
    // draw axes
    ofLine(-50, -50, 0, 50, -50, 0); // x
    ofLine(-50, -25, 0, 50, -25, 0); // x
    ofLine(-50,   0, 0, 50,   0, 0); // x
    ofLine(-50,  25, 0, 50,  25, 0); // x
    ofLine(-50,  50, 0, 50,  50, 0); // x
    
    ofLine(-50, -50, 0, -50, 50, 0); // y
    ofLine(-25, -50, 0, -25, 50, 0); // y
    ofLine(  0, -50, 0,   0, 50, 0); // y
    ofLine( 25, -50, 0,  25, 50, 0); // y
    ofLine( 50, -50, 0,  50, 50, 0); // y
    
    ofLine(0,   0, -50, 0,  0, 50);  // z
}