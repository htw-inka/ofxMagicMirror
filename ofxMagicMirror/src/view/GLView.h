//
//  GLView.h
//  ARMagicMirror
//
//  Created by Markus Konrad on 14.08.13.
//  Copyright (c) 2013 INKA Research Group. All rights reserved.
//
//  OpenGL View class that renders camera frames and detected markers
//  in a OpenGL context.
//

#ifndef __ARMagicMirror__GLView__
#define __ARMagicMirror__GLView__

#include <iostream>
#include <set>

#include "aruco.h"
#include "glm.h"
#include "ContentLoader.h"

using namespace std;

class MagicMirrorApp;

/**
 * OpenGL View class that renders camera frames and detected markers
 * in a OpenGL context.
 */
class GLView {
public:
    /**
     * Init the OpenGL view.
     */
    void init(MagicMirrorApp* mirrorApp);
    
    /**
     * Resize the view to new width <w> and height <h>
     */
    void resize(const unsigned int w, const unsigned int h);
    
    /**
     * Draw the camera frame <camFrame> and <markers>.
     */
    void draw(cv::Mat &camFrame, vector<aruco::Marker> &markers);
    
    /**
     * Set the marker size in meters.
     */
    void setMarkerSize(float m) { markerSize = m; };
    
private:
    /**
     * Draw a content element <elem> according to its content type.
     */
    void drawContentElem(ContentElem * const elem, unsigned long nowMs);
    
    MagicMirrorApp *ctrl;        // pointer to instance of main app
    ContentLoader *contents;     // pointer to singleton instance of content holder
    
    set<ContentElem *> curDetectedElems;    // currently detected content elements
    
    GLfloat projMatOrtho[16];       // GL projection matrix for orthographic display (cam image)
    GLfloat modelViewMatOrtho[16];  // GL model view matrix (mirrored) for orthographic display (cam image)
    GLfloat projMatPersp[16];       // GL projection matrix for perspective display (all markers)
    GLfloat modelViewMatPersp[16];  // GL model view matrix (mirrored) for perspective display
    
    GLuint width;           // view width
    GLuint height;          // view height
    
    float markerSize;       // marker size in meters
    
    float modelRotAngle;    // current 3D object rotation angle
    
#ifdef DEBUG
#define FPS_NUM_FRAMES 20
    ofTrueTypeFont fpsDispFont;
    char fpsString[4];
    unsigned long prevMs;
    unsigned int fpsNumFrames;
#endif
};

#endif /* defined(__ARMagicMirror__GLView__) */
