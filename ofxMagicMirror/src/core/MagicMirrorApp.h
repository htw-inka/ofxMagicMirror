#pragma once

#include "ofMain.h"

#include <iostream>
#include <vector>

#include <fstream>
#include <sstream>

#include "conf.h"
#include "aruco.h"
#include "FrameProcessor.h"
#include "ContentLoader.h"

#include "GLView.h"

class GLView;

/**
 * Main controller class. Handles the setup, draw and update cycles.
 */
class MagicMirrorApp : public ofBaseApp {

	public:
        /**
         * Default deconstructor.
         */
        ~MagicMirrorApp();
    
        /**
         * Setup the application.
         */
		void setup();
    
        /**
         * Update status: Get a new camera frame and process it.
         */
		void update();
    
        /**
         * Tell GLView to draw the frame.
         */
		void draw();

        /**
         * Handles key input.
         * 'f' is for toggling fullscreen display mode.
         */
		void keyReleased(int key);
    
        /**
         * Return the marker size in meters.
         */
        float getMarkerSize() const { return markerSize; };
    
    
        /**
         * Aruco camera parameter with intrinsic camera information.
         */
        aruco::CameraParameters camParams;
    
        /**
         * Aruco marker detector.
         */
        aruco::MarkerDetector detector;
    
        /**
         * Currently detected markers.
         */
        vector<aruco::Marker> markers;
    
    private:
        /**
         * Init the camera with id <camID> and camera XML configuration file <camConf>.
         * <camConf> contains OpenCV camera parameters (intrinsic matrix and distortion).
         */
        void initCam(const int camId, const char * const camConf) throw (cv::Exception);
    
        /**
         * Init the marker detection with marker size <arucoMarkerSize> in meters.
         */
        void initDetection(const float arucoMarkerSize) throw (cv::Exception);
    
        /**
         * Delete markers from the Marker vector, which are older than the defined timeperiod.
         */
        void deleteOldMarkers(unsigned long milliTimestamp);
    
        /**
         * Content holder singleton reference.
         */
        ContentLoader *content;
    
        /**
         * Frame processor to process camera input frames.
         */
        FrameProcessor frameProc;
    
        /**
         * View to render camera image and content objects to
         * an OpenGL context.
         */
        GLView view;
    
        /**
         * OpenFramworks camera capturing object
         */
        ofVideoGrabber camCap;
    
        cv::Mat curFrame;   // raw input frame
        cv::Mat dispFrame;  // display frame
    
        float markerSize;   // marker size in meters
};
