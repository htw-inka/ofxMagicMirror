//
//  FrameProcessor.h
//  ARMagicMirror
//
//  Created by Markus Konrad on 14.08.13.
//  Copyright (c) 2013 INKA Research Group. All rights reserved.
//
//  Frame processor class with process an input frame (e.g. from the camera)
//  and generate an output frame (e.g. to display).
//

#ifndef __ARMagicMirror__FrameProcessor__
#define __ARMagicMirror__FrameProcessor__

#include <iostream>

#include "ofMain.h"
#include "ofxOpenCv.h"

// Forward declarations
class MagicMirrorApp;

/**
 * Frame processor class with process an input frame (e.g. from the camera)
 * and generate an output frame (e.g. to display).
 * The detector will be called to detect markers in the input frame.
 */
class FrameProcessor {
    
public:
    /**
     * init the frame processor with marker size <markerSizeP>.
     */
    void init(float markerSizeP, MagicMirrorApp* mirrorApp);
    
    /**
     * Process the input frame <inFrame> and generate output frame <outFrame>.
     */
    void processFrame(cv::Mat &inFrame, cv::Mat &outFrame) const;
    
private:
    MagicMirrorApp *app; // ref. to the main app
    float markerSize;   // marker size in meters
};

#endif /* defined(__ARMagicMirror__FrameProcessor__) */
