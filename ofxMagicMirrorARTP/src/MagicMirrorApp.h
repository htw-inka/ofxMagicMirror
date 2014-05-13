//
//  MagicMirrorApp.h
//  ARMagicMirrorARTP
//
//  Author: Philipp Jährling, Markus Konrad
//  Copyright (c) 2013 INKA Research Group. All rights reserved.
//
//  OpenFrameworks App which implements the lifecycle and holds references for the Camera Stream, Marker Tracker, ...
//

#pragma once

#include "ofMain.h"
#include "TrackerSingleMarker.h"
#include "ContentLoader.h"
#include "GLView.h"
#include "InfoAnimation.h"

class MagicMirrorApp : public ofBaseApp{
	public:
        /**
         * setup the camera connection and init the marker-tracker
         */
		void setup();
    
        /**
         * load camera input, track marker and set Elements for new Marker
         */
		void update();
    
        /**
         * add Element to the visible Elements for detected Marker, if not already included
         */
        void addElementForFoundMarker(unsigned long nowMs);
    
        /**
         * update the visible Elements, check for timeouts etc.
         */
        void updateVisibleElements(unsigned long nowMs);
    
        /**
         * draw background and draw the content if a marker was found
         */
		void draw();
		
        /**
         * any keyboard actions ("f" key for fullscreen mode)
         */
		void keyReleased(int key);
    
        /**
         * onfigure the tracker according to the options set in conf.h
         */
        void configureTracker();
    
        /**
         * just a log function to log detected marker count and id's
         */
        void logFoundMarkerIDs();
    
    private:
        ofVideoGrabber* camGrabber;         // camera connection
        unsigned char* camBuffer;           // camera input buffer
        ofTexture backgroundTexture;        // uses the buffer data to create the background texture on each new frame
        GLView* view;                       // object to handle all the drawing
        ContentLoader* content;             // ContentLolder singleton reference.
        ARToolKitPlus::TrackerSingleMarker* tracker;    // the marker tracker object
    
        int detMarkerCount;                         // detected Marker count, calculated each update-loop
        ARToolKitPlus::ARMarkerInfo* detMarker;     // array for all found markers, will include makrer entrys which have id = -1
        set<ContentElement*> visibleElements;       // currently visible content elements
        unsigned long lastTimeMarkerFound;          // timestamp in millisec. of the last time a marker was found
    
        InfoAnimation* infoAnimation;       // information overlay for the 
};
