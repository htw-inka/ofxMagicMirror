//
//  Tools.cpp
//  ARMagicMirrorARTP
//
//  Author: Markus Konrad
//  Copyright (c) 2013 INKA Research Group. All rights reserved.
//

#include <sys/time.h>

#include "Tools.h"
#include "glm.h"

//--------------------------------------------------------------
vector<string> &Tools::splitString(const string &s, const char delim, vector<string> &tokens) {
    stringstream sstream(s);
    string item;
    
    while (std::getline(sstream, item, delim)) {
        tokens.push_back(item);
    }
    
    return tokens;
}

//--------------------------------------------------------------
void Tools::copyStringToBuf(const string& s, char **buf) {
    const size_t len = s.length();
    *buf = new char[len + 1];
    s.copy(*buf, len);
    (*buf)[len] = '\0'; // set string termination character
}

//--------------------------------------------------------------
uint64_t Tools::getMicros() {
    struct timeval tv;
    
    gettimeofday(&tv, NULL);
    
    return tv.tv_sec * 1000000UL + tv.tv_usec;
}

//--------------------------------------------------------------
float Tools::getAverageAngle(float *angles, int count) {
    double x = 0;
    double y = 0;
    
    // sum all values
    for (int i = 0; i < count; i++) {
        x += cos(angles[i]);
        y += sin(angles[i]);
    }
    
    // devide by the amount of values
    x /= count;
    y /= count;
    
    if (x > 0) {
        return atan(y / x);
    } else if (x < 0) {
        return PI + atan(y / x);
    }

    // this will just be used when x = 0
    return (y > 0) ? (PI / 2) : (PI / 2);
}


//--------------------------------------------------------------
void Tools::composeModelViewMatrix(float* camPos, float* camRot, float* matrixColumns) {
    matrixColumns[0] =  cos(camRot[2]) * cos(camRot[1]) - sin(camRot[2]) * sin(camRot[0]) * sin(camRot[1]);
    matrixColumns[1] =  sin(camRot[2]) * cos(camRot[1]) + cos(camRot[2]) * sin(camRot[0]) * sin(camRot[1]);
    matrixColumns[2] = -cos(camRot[0]) * sin(camRot[1]);
    matrixColumns[3] = 0;
    
    matrixColumns[4] = -sin(camRot[2]) * cos(camRot[0]);
    matrixColumns[5] =  cos(camRot[2]) * cos(camRot[0]);
    matrixColumns[6] =  sin(camRot[0]);
    matrixColumns[7] =  0;
    
    matrixColumns[8] =  cos(camRot[2]) * sin(camRot[1]) + sin(camRot[2]) * sin(camRot[0]) * cos(camRot[1]);
    matrixColumns[9] =  sin(camRot[2]) * sin(camRot[1]) - cos(camRot[2]) * sin(camRot[0]) * cos(camRot[1]);
    matrixColumns[10] = cos(camRot[0]) * cos(camRot[1]);
    matrixColumns[11] = 0;
    
    matrixColumns[12] = -(camPos[0] * matrixColumns[0] + camPos[1] * matrixColumns[4] + camPos[2] * matrixColumns[8]),
    matrixColumns[13] = -(camPos[0] * matrixColumns[1] + camPos[1] * matrixColumns[5] + camPos[2] * matrixColumns[9]),
    matrixColumns[14] = -(camPos[0] * matrixColumns[2] + camPos[1] * matrixColumns[6] + camPos[2] * matrixColumns[10]),
    matrixColumns[15] = 1;
}


//--------------------------------------------------------------
void Tools::decomposeModelViewMatrix(float* matrixColumns, float* camPos, float* camRot) {
    camRot[0]   = asin(matrixColumns[6]);
    float cosrx = cos(camRot[0]);
    
    camRot[1] = atan(-matrixColumns[2] / matrixColumns[10]);
    if (matrixColumns[10] * cosrx < 0) {
        camRot[1] += PI;
    }
    
    camRot[2] = atan(-matrixColumns[4] / matrixColumns[5]);
    if (matrixColumns[5] * cosrx < 0) {
        camRot[2] += PI;
    }
    
    ofMatrix3x3 rotMatInv = ofMatrix3x3(
        matrixColumns[0], matrixColumns[4], matrixColumns[8],
        matrixColumns[1], matrixColumns[5], matrixColumns[9],
        matrixColumns[2], matrixColumns[6], matrixColumns[10]
    );
    rotMatInv.invert();
    
    camPos[0] = -(rotMatInv[0] * matrixColumns[12] + rotMatInv[1] * matrixColumns[13] + rotMatInv[2] * matrixColumns[14]);
    camPos[1] = -(rotMatInv[3] * matrixColumns[12] + rotMatInv[4] * matrixColumns[13] + rotMatInv[5] * matrixColumns[14]);
    camPos[2] = -(rotMatInv[6] * matrixColumns[12] + rotMatInv[7] * matrixColumns[13] + rotMatInv[8] * matrixColumns[14]);
}


//--------------------------------------------------------------
void Tools::swapRGBPixelBuffer(unsigned char* inBuffer, unsigned char* outBuffer, int width, int height) {
    int rowLength = width * 3; // for every Pixel we got the RGB Components
    for (int i = 0; i < height; i++) {
        int rowStartIndex = i * rowLength;
        for (int j = 0; j < rowLength; j+=3) {
            // pixel number
            int pix1 = rowStartIndex + j;
            int pix2 = rowStartIndex + (j + 1);
            int pix3 = rowStartIndex + (j + 2);
            // mirror pixel number
            int mir1 = rowStartIndex + (rowLength - j - 3);
            int mir2 = rowStartIndex + (rowLength - j - 2);
            int mir3 = rowStartIndex + (rowLength - j - 1);
            // swap pixels
            outBuffer[pix1] = inBuffer[mir1];
            outBuffer[pix2] = inBuffer[mir2];
            outBuffer[pix3] = inBuffer[mir3];
        }
    }
}


//--------------------------------------------------------------
void Tools::drawTempTextureQuad2D(float quadW, float quadH, float texW, float texH, const void *pixelData) {
    // create texture
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_RECTANGLE_EXT, tex);
    glTexImage2D(GL_TEXTURE_RECTANGLE_EXT, 0, GL_RGB, texW, texH, 0, GL_RGB, GL_UNSIGNED_BYTE, pixelData);
    
    // draw texture
    glEnable(GL_TEXTURE_RECTANGLE_EXT);
    glBegin (GL_QUADS);
    
    glTexCoord2f (0, 0);
    glVertex2f (0, 0);
    glTexCoord2f (quadW, 0);
    glVertex2f (quadW, 0);
    glTexCoord2f (quadW, quadH);
    glVertex2f (quadW, quadH);
    glTexCoord2f (0, quadH);
    glVertex2f (0, quadH);
    
    glEnd();
    glDisable(GL_TEXTURE_RECTANGLE_EXT);
    
    // dont forget to destroy the texture
    glDeleteTextures(1, &tex);
}

//--------------------------------------------------------------
void Tools::drawTempTextureQuad3D(float quadW, float quadH, float texW, float texH, const void *pixelData) {
    // generate an id and bind the texture
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    
    // load the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 texW, texH, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, pixelData);
    
    // set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // Linear Filtering
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // Linear Filtering
    
    // draw texture
    Tools::drawTexturedPlane(quadW, quadH, 1.0f, 1.0f);
    
    // dont forget to destroy the texture
    glDeleteTextures(1, &tex);
}

//--------------------------------------------------------------
void Tools::drawTexturedPlane(float quadW, float quadH, float texCoordW, float texCoordH) {
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    
    glTexCoord2f(0, 0);
    glVertex3f(0, 0, 0);
    
    glTexCoord2f(texCoordW, 0);
    glVertex3f(quadW, 0, 0);
    
    glTexCoord2f(texCoordW, texCoordH);
    glVertex3f(quadW, quadH, 0);
    
    glTexCoord2f(0, texCoordH);
    glVertex3f(0, quadH, 0);
    
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

//--------------------------------------------------------------
GLuint Tools::createTexture(const string imgFile, unsigned int *w, unsigned int *h, GLenum pxlFormat, unsigned int *numChan) {
    ofPixels pxls;
    bool imgLoaded = ofLoadImage(pxls, imgFile);
    
    assert(imgLoaded);
    
    if (w != NULL && h != NULL) {
        *w = pxls.getWidth();
        *h = pxls.getHeight();
    }
    
    const int c = pxls.getNumChannels();
    if (numChan != NULL) {
        *numChan = c;
    }
    
    if (pxlFormat == 0) { // no pixel format specified
        if (c == 3) {
            pxlFormat = GL_RGB;
        } else {
            pxlFormat = GL_RGBA;
        }
    }
    
    // create the texture
    GLuint tex = Tools::createTexture(pxls.getPixels(), pxls.getWidth(), pxls.getHeight(), pxlFormat);
    
    // destroy original image data
    pxls.clear();
    
    return tex;
}

//--------------------------------------------------------------
GLuint Tools::createTexture(unsigned char * const pixelData, unsigned int w, unsigned int h, GLenum pxlFormat) {
    // generate an id and bind the texture
    GLuint tex;
    glGenTextures(1, &tex);
    GLenum err = glGetError(); assert(err == GL_NO_ERROR);
    glBindTexture(GL_TEXTURE_2D, tex);
    err = glGetError(); assert(err == GL_NO_ERROR);
    
    // load the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 w, h, 0,
                 pxlFormat, GL_UNSIGNED_BYTE, pixelData);
    err = glGetError(); assert(err == GL_NO_ERROR);
    
    // set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // Linear Filtering
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // Linear Filtering

    return tex;
}

//--------------------------------------------------------------
void Tools::useTexture(GLuint tex) {
    glBindTexture(GL_TEXTURE_2D, tex);
}

//--------------------------------------------------------------
void Tools::deleteTexture(GLuint tex) {
    glDeleteTextures(1, &tex);
}

//--------------------------------------------------------------
GLuint Tools::createModelFromObj(string objFile) {
    char* fileNameBuffer;
    copyStringToBuf( ofDirectory(objFile).getAbsolutePath(), &fileNameBuffer );
    
	GLMmodel* mdl;
    
	//load model and unify
	mdl = glmReadOBJ(fileNameBuffer);
    assert(mdl != NULL);
	glmUnitize(mdl);
    
	//creat normals
	glmFacetNormals(mdl);
    glmVertexNormals(mdl, 90.0);
    
	//create call list
    GLuint list = glmList(mdl, GLM_SMOOTH | GLM_TEXTURE);
    
    // delete model
	glmDelete(mdl);

    return list;
}

//--------------------------------------------------------------
void Tools::drawModel(GLuint list) {
    glCallList(list);
}

//--------------------------------------------------------------
void Tools::deleteModel(GLuint list) {
    glDeleteLists(list, 1);
}