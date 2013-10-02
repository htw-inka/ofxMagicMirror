//
//  GLView.cpp
//  ARMagicMirror
//
//  Created by Markus Konrad on 14.08.13.
//  Copyright (c) 2013 INKA Research Group. All rights reserved.
//

#include "GLView.h"
#include "Tools.h"

#include "MagicMirrorApp.h"

void GLView::init(MagicMirrorApp* mirrorApp) {
    ctrl = mirrorApp;
    contents = ContentLoader::getInstance();
    
    width = height = 0;
    
    modelRotAngle = 0.0f;
    
#ifdef DEBUG
    prevMs = 0;
    fpsNumFrames = FPS_NUM_FRAMES;
    fpsDispFont.loadFont("arial.ttf", 16);
#endif
    
    // anti-alias
#if CONF_ANTIALIAS > 0
    ofGetCurrentRenderer()->enableAntiAliasing();
#endif
    
    // clear stuff
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearDepth(1.0);
    
    // depth buffer
    glEnable(GL_DEPTH_TEST);
    
    // normalizing
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
//    glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);
    
    // light stuff
	GLfloat fLightAmbient[]= { 0.9f, 0.9f, 0.6f, 1.0f };
	GLfloat fLightDiffuse[]= { 1.0f, 1.0f, 0.9f, 1.0f };
	GLfloat fLightPosition[]= { -1.0f, 1.0f, 0.0f, 1.0f };
    
	//glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);
	glLightfv(GL_LIGHT1, GL_AMBIENT, fLightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, fLightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, fLightPosition);
    
	glLightfv(GL_LIGHT0, GL_AMBIENT, fLightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, fLightDiffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, fLightPosition);
    
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glDisable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
}

void GLView::resize(const unsigned int w, const unsigned int h) {
    cout << "resized to " << w << "x" << h << endl;
    
    width = w;
    height = h;
    
    // set the matrix for orthogonal view (2D camera image)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    glGetFloatv(GL_PROJECTION_MATRIX, projMatOrtho);    // save the matrix
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(width, 0.0f, 0.0f);
    glScalef(-1.0f, 1.0f, 1.0f);
    glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatOrtho);
    
    // set the matrix for perspective projection
    cv::Size size(width, height);
    ctrl->camParams.glGetProjectionMatrix(size, size, projMatPersp, 0.05, 10);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(-1.0f, 1.0f, 1.0f);
    glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatPersp);
}

void GLView::draw(cv::Mat &camFrame, vector<aruco::Marker> &markers) {
#if CONF_STOPWATCH == 1
    Tools::stopwatchStart();
#endif
    
    // get current time in ms
    unsigned long nowMs = Tools::getMicros() / 1000L;
    
    ///clear
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    // set to orthogonal projection
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(projMatOrtho);
    
    // display camera image as texture
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(modelViewMatOrtho);
    
    glColor3f(1.0f, 1.0f, 1.0f);
    
    glDisable(GL_DEPTH_TEST);
    
    // draw texture    
    Tools::drawTempTextureQuad2D(width, height, width, height, camFrame.ptr());
    
#ifdef DEBUG
    glLoadIdentity();
    if (fpsNumFrames-- == 0) {
        snprintf(fpsString, 4, "%d", (int)(FPS_NUM_FRAMES * 1000.0f / (float)(nowMs - prevMs)));
        fpsNumFrames = FPS_NUM_FRAMES;
        prevMs = nowMs;
    }
    
    fpsDispFont.drawString(fpsString, 10, 30);
#endif
    
    ///Set the appropriate projection matrix so that rendering is done in a enrvironment
    //like the real camera (without distorsion)
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(projMatPersp);
    
    //now, for each marker
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(modelViewMatPersp);
    
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    
    for (vector<aruco::Marker>::iterator markerIt = markers.begin();
         markerIt != markers.end();
         ++markerIt) {
        // go through all the found markers
        cout << "found new marker with id: " << markerIt->id << endl;
        
        // check if we have an content element for this marker id
        ContentElem *elem = contents->getContent(markerIt->id);
        if (elem) { // yes we have an element
            if (curDetectedElems.find(elem) == curDetectedElems.end()) {    // add a new element
                curDetectedElems.insert(elem);
                
                cout << "> associated with element of type: " << elem->type << endl;
                
                if (elem->type == VID) {
                    ((VIDData *)elem->extraData)->startPlayback = true;
                }
            }
            
            // update model view matrix for the marker to display it later
            markerIt->glGetModelViewMatrix(elem->modelViewMat);
            
            // update the time
            elem->lastDrawnTime = nowMs;
        }
    }
    
    // update and display the content elements
    for (set<ContentElem *>::iterator elemIt = curDetectedElems.begin();
         elemIt != curDetectedElems.end();
         ++elemIt) {
        ContentElem *elem = *elemIt;
        
        // check if the marker has timed out
        if (nowMs >= elem->lastDrawnTime + CONF_MARKER_RECOGNITION_TIMEOUT_MILLISEC) {
            if (elem->type == VID) {    // pause the video at first
                VIDData *elemVidData = ((VIDData *)elem->extraData);
                
                if (!elemVidData->vidPlayer->isPaused()) {
                    elemVidData->vidPlayer->setPaused(true);
                
                    cout << "video paused" << endl;
                }
                
                // now it's really time to stop the video
                if (nowMs >= elem->lastDrawnTime + CONF_RESTART_VIDEO_AFTER_MILLISEC) {
                    cout << "video stopped" << endl;
                    
                    elemVidData->vidPlayer->stop();
                    elemVidData->vidPlayer->setFrame(0);
                    elemVidData->startPlayback = true;
                    
                    cout << "marker with id " << elem->id << " timed out" << endl;
                    curDetectedElems.erase(elemIt);
                }
            } else {
                cout << "marker with id " << elem->id << " timed out" << endl;
                curDetectedElems.erase(elemIt); // erase if it's not for video
            }
        } else {    // display the marker with its content
            glPushMatrix();

            // multiply our mirrored model view with the marker's model view matrix
            // to get the correct position
            glMultMatrixf(elem->modelViewMat);
            
            // the content itself may not be mirrored!
            // so set it back to normal display
            glScalef(-1.0f, 1.0f, 1.0f);
            
            // draw the content
            drawContentElem(elem, nowMs);
            
            glPopMatrix();
        }
    }
    
#if CONF_STOPWATCH == 1
    Tools::stopwatchStop();
#endif
}

void GLView::drawContentElem(ContentElem * const elem, unsigned long nowMs) {
    // we assign this value to the element at the end
    if (elem->type == IMG) {                // display image directly on marker
        IMGData *imgData = (IMGData *)elem->extraData;
        if (!imgData) return;
        
        // use texture
        Tools::useTexture(imgData->texId);
        
        // rotate, translate, scale
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);                         // let it face to the camera
        glScalef(markerSize, markerSize, markerSize);               // scale down to marker units
        glTranslatef(imgData->offX, imgData->offY, 0.0f);           // put it directly on the marker
        
        if (imgData->chan == 4) {   // we have an alpha-channel texture
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);  // take the alpha channel of the image and
                                                // blend it with the full background
        }
        
        // draw the image as a plane
        Tools::drawTexturedPlane(imgData->quadW, imgData->quadH, 1.0f, 1.0f);
        
        if (imgData->chan == 4) {  // we had an alpha-channel texture
            glDisable(GL_BLEND);
            glBlendFunc(GL_ONE,GL_ZERO);
        }
    } else if (elem->type == VID) {
        VIDData *vidData = (VIDData *)elem->extraData;
        if (!vidData) return;

        // video was not shown for a certain time. rewind the video
        if (vidData->vidPlayer->isPaused()) {
            cout << "resuming video player playback" << endl;
            vidData->vidPlayer->setPaused(false);
        } else if (vidData->startPlayback) {
            vidData->vidPlayer->play();
            vidData->startPlayback = false;
        }
        
        // update the video frame texture if necessary
        vidData->vidPlayer->update();
        if (vidData->vidPlayer->isFrameNew()) {
            uint8_t *framePixels = vidData->vidPlayer->getPixels();
            uint8_t *maskPixels = NULL;
            
            if (vidData->alpha) {   // process alpha video
                cv::Mat completeFrame(vidData->h, vidData->w, CV_8UC3, framePixels);
                
                cv::Mat origFrame = completeFrame(vidData->origFrameRect);
                cv::Mat maskFrame = completeFrame(vidData->maskFrameRect);
                framePixels = origFrame.ptr();
                maskPixels = maskFrame.ptr();
            }
            
            // update textures
            if (vidData->frameTex > 0) {
                Tools::deleteTexture(vidData->frameTex);
            }
            
            vidData->frameTex = Tools::createTexture(framePixels, vidData->frameW, vidData->frameH);
            
            if (vidData->maskTex > 0) {
                Tools::deleteTexture(vidData->maskTex);
            }
            
            vidData->maskTex = Tools::createTexture(maskPixels, vidData->frameW, vidData->frameH);
        }
        
        // rotate, translate, scale
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);                         // let it face to the camera
        glScalef(markerSize, markerSize, markerSize);               // scale down to marker units
        glTranslatef(vidData->offX, vidData->offY, 0.0f);           // put it directly on the marker
        
        // draw the frame
        if (vidData->alpha) {   // we have an alpha-channel mask
            glEnable(GL_BLEND);
            glBlendFunc(GL_DST_COLOR, GL_ONE);
        }
        
        if (vidData->frameTex > 0) {
            Tools::useTexture(vidData->frameTex);
            Tools::drawTexturedPlane(vidData->quadW, vidData->quadH);
        }
        
        if (vidData->alpha && vidData->maskTex > 0) {   // we have an alpha-channel mask
            
            Tools::useTexture(vidData->maskTex);
            Tools::drawTexturedPlane(vidData->quadW, vidData->quadH);
            glDisable(GL_BLEND);
            glBlendFunc(GL_ONE,GL_ZERO);
        }
    } else if (elem->type == OBJ) {        // display 3D model directly on marker
        OBJData *objData = (OBJData *)elem->extraData;
        if (!objData) return;
        
        Tools::useTexture(objData->texId);
        glScalef(markerSize, markerSize, markerSize);
        glEnable(GL_LIGHTING);
        Tools::drawModel(objData->modelId);
        glDisable(GL_LIGHTING);
    }
}