//
//  GLView.h
//  ARMagicMirrorARTP
//
//  Author: Markus Konrad, Philipp Jährling
//  Copyright (c) 2013 INKA Research Group. All rights reserved.
//
//  OpenGL View class that renders camera frames and detected markers in a OpenGL context.
//

#pragma once

#include "ofMain.h"
#include "TrackerSingleMarker.h"
#include "glm.h"
#include "ContentElement.h"

class GLView {
    public:    
        /**
         * Init the OpenGL view.
         */
        GLView(int w, int h);
    
        /**
         * Cleanup the OpenGL view.
         */
        ~GLView();
    
        /**
         * Set the Projection Matrix
         */
        void setProjection(const GLfloat *proj);
    
        /**
         * Draw the camera frame <backgroundTex>
         */
        void drawBackground(ofTexture backgroundTex);

        /**
         * Draw the <markers>.
         */
        void drawElement(ContentElement* elem);

        /**
         * Draw the given Text, mostly for evaluation
         */
        void drawText(string text);
    
        /**
         * Toggle the occlusion cube to show it with a solid color for debugging
         */
        void toggleSolidOcclusionCubeDisplay() { showSolidOcclusionCube = !showSolidOcclusionCube; }
    
    private:
        GLuint width;           // view width
        GLuint height;          // view height
        ofLight defaultLight;   // light settings
    
        bool showSolidOcclusionCube;    // for testing: show the occlusion cube directly
    
        // draw a cube for fake occlusion. uses the OpenGL stencil buffer
        void drawOcclusionCube();
    
        // for testing
        void drawMarkerID(ContentElement* elem);
        void drawAxes();
};
