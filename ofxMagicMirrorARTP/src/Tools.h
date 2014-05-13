//
//  Tools.h
//  ARMagicMirrorARTP
//
//  Author: Markus Konrad, Alexander Godoba
//  Copyright (c) 2013 INKA Research Group. All rights reserved.
//
//  Common utility functions.
//

#pragma once

#include <iostream>
#include <vector>

#include "ofMain.h"

using namespace std;

/**
 * Class with common static utility functions for:
 * - string handling / manipulation
 * - timing
 * - OpenGL texture management
 * - OpenGL OBJ model management (using glm library)
 */
class Tools {
public:
    /**
     * Split string <s> by character <delim> and put the substrings into <tokens> as well as return it.
     */
    static vector<string> &splitString(const string &s, const char delim, vector<string> &tokens);
    
    /**
     * Copy string <s> completely to buffer <buf>. <buf> is a pointer to an empty char* array.
     * <buf> will be allocated in this function with <s.length() + 1> bytes.
     */
    static void copyStringToBuf(const string& s, char **buf);

    /**
     * Return the number of microseconds (NOT millisec.!) since Unix Epoch.
     */
    static uint64_t getMicros();
    
    /**
     * Swap a texture
     */
    static void swapRGBPixelBuffer(unsigned char* inBuffer, unsigned char* outBuffer, int width, int height);
    
    /**
     * Get the avarage angle between an array of angles
     */
    static float getAverageAngle(float* angles, int count);
    
    /**
     * calculate a ModelView Matrix from camera rotation and position (vectors)
     */
    static void composeModelViewMatrix(float* camPos, float* camRot, float* matrixColumns);
    
    /**
     * calculate camera rotation and position (vectors) from a ModelView Matrix 
     */
    static void decomposeModelViewMatrix(float* matrixColumns, float* camPos, float* camRot);
    
    /**
     * Draw temporary texture from <pixelData> in a 2D quad of dimensions <quadW> x <quadH>.
     * <pixelData> is 3-channel RGB unsigned byte data of dimensions <texW> x <texH>.
     * The texture memory will be cleared. This is method is good for drawing animated image
     * sequences (e.g. video frames).
     */
    static void drawTempTextureQuad2D(float quadW, float quadH, float texW, float texH, const void *pixelData);
    static void drawTempTextureQuad3D(float quadW, float quadH, float texW, float texH, const void *pixelData);
    
    /**
     * Draws a quad of size <quadW> x <quadH> using a texture with texture coordinates
     * <texCoordW>, <texCoordH>. You must specify the texture to use in advance
     * (see Tools::useTexture)
     */
    static void drawTexturedPlane(float quadW, float quadH, float texCoordW = 1.0f, float texCoordH = 1.0f);
    
    /**
     * Create a texture from <imgFile> and return the GL texture ID.
     * If <w> and <h> are != NULL, save the image dimensions to these parameters.
     * If <numChan> is != NULL, save the number of channels in the image to this parameter.
     * <pxlFormat> defines the pixel format if the input image. If <pxlFormat> is 0, we will try
     * to guess the format.
     *
     * Uses cv::imread() to load the image data.
     */
    static GLuint createTexture(const string imgFile, unsigned int *w = NULL, unsigned int *h = NULL, GLenum pxlFormat = GL_RGB, unsigned int *numChan = NULL);
    
    /**
     * Create a texture from <pixelData> and return the GL texture ID.
     * <pixelData> is usually 3-channel RGB unsigned byte data of dimensions <w> x <h> but
     * other pixel format can be specified via <pxlFormat>.
     */
    static GLuint createTexture(unsigned char * const pixelData, unsigned int w, unsigned int h, GLenum pxlFormat = GL_RGB);
    
    /**
     * Use the texture with GL texture ID <tex> now.
     */
    static void useTexture(GLuint tex);
    
    /**
     * Delete the texture with GL texture ID <tex>.
     */
    static void deleteTexture(GLuint tex);
    
    
    /**
     * Create a model (as GL display list) from a .obj-file <objFile> and return
     * the GL display list ID.
     */
    static GLuint createModelFromObj(string objFile);
    
    /**
     * Draw a model specified by GL display list ID <list>.
     */
    static void drawModel(GLuint list);
    
    /**
     * Destroy a model specified by GL display list ID <list>.
     */
    static void deleteModel(GLuint list);
    
private:
};