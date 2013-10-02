//
//  Tools.cpp
//  ARMagicMirror
//
//  Created by Markus Konrad on 15.08.13.
//  Copyright (c) 2013 INKA Research Group. All rights reserved.
//

#include <sys/time.h>

#include "Tools.h"

#include "glm.h"

#if CONF_STOPWATCH == 1
uint64_t Tools::swLastMs = 0;
#endif

vector<string> &Tools::splitString(const string &s, const char delim, vector<string> &tokens) {
    stringstream sstream(s);
    string item;
    
    while (std::getline(sstream, item, delim)) {
        tokens.push_back(item);
    }
    
    return tokens;
}

void Tools::copyStringToBuf(const string& s, char **buf) {
    const size_t len = s.length();
    *buf = new char[len + 1];
    s.copy(*buf, len);
    (*buf)[len] = '\0'; // set string termination character
}

uint64_t Tools::getMicros() {
    struct timeval tv;
    
    gettimeofday(&tv, NULL);
    
    return tv.tv_sec * 1000000UL + tv.tv_usec;
}

#if CONF_STOPWATCH == 1

void Tools::stopwatchStart() {
    swLastMs = getMicros();
}

void Tools::stopwatchStop() {
    uint64_t now = getMicros();

    cout << "stopwatch: " << (double)((uint64_t)(now - swLastMs) / 1000.0) << "ms" << endl;
    
    swLastMs = now;
}

#endif

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

GLuint Tools::createTexture(const char *imgFile, unsigned int *w, unsigned int *h, GLenum pxlFormat, unsigned int *numChan) {
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

void Tools::useTexture(GLuint tex) {
    glBindTexture(GL_TEXTURE_2D, tex);
}

void Tools::deleteTexture(GLuint tex) {
    glDeleteTextures(1, &tex);
}

GLuint Tools::createModelFromObj(char *objFile) {
	GLMmodel* mdl;
    
	//load model and unify
	mdl = glmReadOBJ(objFile);
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

void Tools::drawModel(GLuint list) {
    glCallList(list);
}

void Tools::deleteModel(GLuint list) {
    glDeleteLists(list, 1);
}