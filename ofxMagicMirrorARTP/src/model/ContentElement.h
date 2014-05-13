//
//  ContentElement.h
//  ARMagicMirrorARTP
//
//  Author: Philipp Jährling, Markus Konrad, Alexander Godoba
//  Copyright (c) 2013 INKA Research Group. All rights reserved.
//
//  Class to represent all contents associated with a marker id.
//  There are different types of Content which will have different kind of data.
//

#pragma once

#include "ofMain.h"
#include "Data.h"

/**
 * Define the content types.
 */
enum ContentType {
    IMG,    // image
    VID,    // video
    OBJ,    // 3D object
    _UNKNOWN
};


/**
 * Definde the ContentElement Class
 */
class ContentElement {
    private:
        GLfloat modelViewMatrix[16];        // the Model View Matrix for the last Frame, where the Marker was found
        float curCamPos[3];                 // the camera position, calculated out of ModelViewMatrix
        float curCamRot[3];                 // the camera rotation, calculated out of ModelViewMatrix
        float *camPosWindow;                // camera position history for a smothing effect
        float *camRotWindow;                // camera rotation history for a smothing effect
    
        float elemScale;                    // the (configured initial) scale value for the content element
        float elemRot[3];                   // the (configured initial) rotation (x, y, z) for the content element
        float elemTrans[3];                   // the (configured initial) translation (x, y, z) for the content element
    
        /**
         * Returns the average position and rotation of a camera from the filter array.
         */
        void calcCameraParams(float* position, float* rotation);
    
        /**
         * Pushes the next position and rotation of a camera into the filter array.
         */
        void pushCameraParams(float* position, float* rotation);
    
    public:
        int markerId;                       // the related marker id
        string name;                        // the name defined with the content folder
        unsigned long lastTimeDetected;     // the timestamp (ms) when the related marker was detected the last time
        ContentType type;                   // content type definition
        Data* typeData;                     // type specific data (for type IMG -> ImgData Object)
    
        /**
         * Constructor
         */
        ContentElement(int markerId, string name, ContentType type);
    
        /**
         * Deconstructor
         */
        ~ContentElement();
    
        /**
         * Draw the content element to the openGL View. This will be delegated to the typeData Object's draw-function
         */
        void draw();
    
        /**
         * Set the scaling value for the element
         */
        void setElemScaling(float scale);
    
        /**
         * Set the rotation values for the element
         */
        void setElemRotation(float x, float y, float z);
    
        /**
        * Set the translation values for the element
        */
        void setElemTranslation(float x = 0, float y = 0, float z = 0) throw (runtime_error);
    
        /**
         * Set a new ModelViewMatrix
         */
        void setModelViewMatrix(GLfloat* mat);
    
        /**
         * Return current ModelViewMatrix
         */
        GLfloat* getModelViewMatrix();
    
        /**
         * Set all CaramParams (Rotation, Position) to 0
         */
        void resetCamParams();
    
        /**
         * Save the current rotation (in radiands) around x,y,z axis of the element to <rot> array.
         */
        void getCurCamRot(float camRot[3]) const;
    
        /**
         * Save the current position of the element to <pos> array.
         */
        void getPosVec(float pos[3]) const;
};
