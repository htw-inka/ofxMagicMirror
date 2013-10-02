//
//  ContentTypes.h
//  ARMagicMirror
//
//  Created by Markus Konrad on 03.09.13.
//  Copyright (c) 2013 INKA Research Group. All rights reserved.
//

#ifndef ARMagicMirror_ContentTypes_h
#define ARMagicMirror_ContentTypes_h

#include "ofMain.h"
#include "ofxOpenCv.h"

/**
 * Define the content types.
 */
typedef enum _ContentType {
    IMG,    // image
    VID,    // video
    OBJ,    // 3D object
    _UNKNOWN
} ContentType;

/**
 * Define "extra data" type for image content.
 */

typedef struct _IMGData {
    GLuint texId;           // GL texture id
    GLuint w;               // image width in pixels
    GLuint h;               // image height in pixels
    GLuint chan;            // number of channels in image
    GLfloat quadW, quadH;   // GL display quad size in marker units
    GLfloat offX, offY;     // X and Y offset in marker units
    
    _IMGData() {
        texId = 0;
        w = h = chan = 0;
        quadW = quadH = offX = offY = 0.0f;
    };
    
    ~_IMGData() {
        if (texId) Tools::deleteTexture(texId);
    };
} IMGData;

/**
 * Define "extra data" type for video content.
 */

typedef struct _VIDData {
    ofVideoPlayer *vidPlayer;   // Ofx video capturerer to read video frames
    GLuint w;                   // full video frame width
    GLuint h;                   // full video frame height
    GLuint frameTex;            // texture for the original frame
    GLuint maskTex;            // texture for the masked frame (in alpha masked mode)
    GLuint frameW;              // actual video frame width with enabled alpha-masking (always == w)
    GLuint frameH;              // actual video frame height with enabled alpha-masking (alpha video: h/2, else h)
    cv::Rect origFrameRect;     // alpha video only: rectangle that describes original frame
    cv::Rect maskFrameRect;     // alpha video only: rectangle that describes frame alpha mask
    bool alpha;                 // is this an video with alpha mask?
    bool startPlayback;         // should the video be started on the next draw loop?
    GLfloat quadW, quadH;       // GL display quad size in marker units
    GLfloat offX, offY;         // X and Y offset in marker units
    
    _VIDData() {
        vidPlayer = NULL;
        frameTex = maskTex = 0;
        w = h = 0;
        quadW = quadH = offX = offY = 0.0f;
        alpha = false;
        startPlayback = true;
    };
    
    ~_VIDData() {
        if (vidPlayer) delete vidPlayer;
    };
} VIDData;

/**
 * Define "extra data" type for 3D object content.
 */
typedef struct _OBJData {
    GLuint modelId; // 3d object id
    GLuint texId;   // texture id
    
    _OBJData() {
        modelId = 0;
        texId = 0;
    };
    
    ~_OBJData() {
        if (modelId) Tools::deleteTexture(modelId);
        if (texId) Tools::deleteModel(texId);
    };
} OBJData;

/**
 * Define a content element type.
 */
typedef struct _ContentElem {
    int id;                         // marker id
    float modelViewMat[16];         // marker model view matrix
    ContentType type;               // content type
    char *name;                     // specifier
    char *mainFile;                 // file to use for this content
    char **extraFiles;              // array of additional files of length <numExtraFiles>
    unsigned short numExtraFiles;   // length of array <extraFiles>
    void *extraData;                // extra data of arbitary type: OBJData for type OBJ
    unsigned long lastDrawnTime;    // when the content Element was drawn/shown the last time
    
    _ContentElem(int i, ContentType t) {
        id = i;
        type = t;
        name = NULL;
        mainFile = NULL;
        extraFiles = NULL;
        numExtraFiles = 0;
        extraData = NULL;
        lastDrawnTime = 0;
    };
    
    ~_ContentElem() {
        if (name) delete name;
        if (mainFile) delete mainFile;
        if (extraFiles) delete [] extraFiles;
        if (extraData) free(extraData);
    };
} ContentElem;


#endif
