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

class MagicMirrorApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        ~MagicMirrorApp();
    
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
