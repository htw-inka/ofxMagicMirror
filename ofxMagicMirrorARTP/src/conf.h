//
//  conf.h
//  ARMagicMirrorARTP
//
//  Author: Markus Konrad, Philipp Jährling
//  Copyright (c) 2013 INKA Research Group. All rights reserved.
//
//  Static configuration
//

#pragma once

#define CONF_DEBUG              0                   // set Debug true (1) or false (0)
#define CONF_FONT_PATH          "./res/fonts/arial.ttf"
#define CONF_CONTENT_DIR        "./res/contents"    // folder where all the contents reside

#define CONF_INFO_ANIM_PATH     "./res/contents/infoAnimation/info.gif"
#define CONF_INFO_SHOW_ON_START 1                   // true (1) or false (0)
#define CONF_INFO_SPEED         8
#define CONF_INFO_HEIGHT        250
#define CONF_INFO_SHOW_ANI_AFTER_MILSEC   6000     // time without a marker recognised, after which a Animation is shown
#define CONF_INFO_SIDE          0                  // rotate the info animation (0 = bottom, 1 = left, 2 = right)
#define CONF_INFO_SIZE          250

// request cam resolution in pixel (width, height), number of camera and the camera config file
// ***********************************
// for old MacBook (Pro)

#define CONF_CAM_W              640
#define CONF_CAM_H              480
#define CONF_CAM_ID             0
#define CONF_CAM_FILE           "./res/cams/MacBook_intrinsic.xml"


// Apple Screen or new MacBook Build in Cam
/*
#define CONF_CAM_W               1280
#define CONF_CAM_H               720
#define CONF_CAM_ID              0
#define CONF_CAM_FILE           "./res/cams/AppleScreen_intrinsic.xml"
*/
 
 // Logitec HD-Cam
/*
#define CONF_CAM_W               1920
#define CONF_CAM_H               1080
#define CONF_CAM_ID              0
#define CONF_CAM_FILE           "./res/cams/Logitec_intrinsic.xml"
*/
// ***********************************

#define CONF_MIRROR_CAM_IMG     1                    // mirror the camera image (1) or leave it as it is (0)
#define CONF_PROCESS_HALF_RES   0                    // take half resoultion image of camera image (1) or full res. (0)
#define CONF_MARKER_USE_SIMPLE  0                    // use simple marker templates (thick border) (1) or BCH (0)
#define CONF_FRAME_UNDISTORT    0                    // 1 = STD, 0 = NONE
#define CONF_ANTIALIAS          1                    // set anti-aliasing enabled (1) or disabled (0)
        
#define CONF_RECOGNITION_TIMEOUT_MILLISEC    500     // marker timout after which it will not be displayed anymore
#define CONF_RESTART_VIDEO_AFTER_MILLISEC    3000    // timeout when a video will be completely stopped
#define CONF_SMOTHING_FILTER_WINDOW          10       // how many values should be considered smothing the rotation and translation