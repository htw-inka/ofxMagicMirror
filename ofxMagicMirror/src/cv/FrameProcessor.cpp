//
//  FrameProcessor.cpp
//  ARMagicMirror
//
//  Created by Markus Konrad on 14.08.13.
//  Copyright (c) 2013 INKA Research Group. All rights reserved.
//

#include "FrameProcessor.h"
#include "MagicMirrorApp.h"

void FrameProcessor::init(float markerSizeP, MagicMirrorApp* mirrorApp) {
    app = mirrorApp;
    markerSize = markerSizeP;
}

void FrameProcessor::processFrame(cv::Mat &inFrame, cv::Mat &outFrame) const {
#if CONF_FRAME_UNDISTORT == 1
    cv::Mat undistFrame;
    undistFrame.create(inFrame.size(),CV_8UC3);
#endif
    
    //transform color that by default is BGR to RGB because windows systems do not allow reading BGR images with opengl properly
    //        cv::cvtColor(TheInputImage,TheInputImage,CV_BGR2RGB);
    
    //remove distorion in image
#if CONF_FRAME_UNDISTORT == 1
    cv::undistort(inFrame,undistFrame, ctrl->camParams.CameraMatrix, ctrl->camParams.Distorsion);
#endif
    
    //detect markers
    app->detector.detect(inFrame, app->markers, app->camParams.CameraMatrix, cv::Mat(), markerSize);
        
    //resize the image to the size of the GL window
#if CONF_FRAME_RESIZE == 1
    cv::resize(inFrame, outFrame, cv::Size(ofGetWidth(), ofGetHeight()));
#else
    outFrame = inFrame.clone();
#endif
}