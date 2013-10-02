//
//  conf.h
//  ARMagicMirror
//
//  Created by Markus Konrad on 14.08.13.
//  Copyright (c) 2013 INKA Research Group. All rights reserved.
//
//  Static configuration
//

#ifndef ARMagicMirror_conf_h
#define ARMagicMirror_conf_h

#define CONF_CAM_ID              0                              // number of the camera
#define CONF_CAM_FILE_FORMAT     "./res/cams/intrinsic_%d.xml"  // %d is CAM_ID
#define CONF_CAM_W               640                            // requested cam resolution width in pixels
#define CONF_CAM_H               480                            // requested cam resolution height in pixels
#define CONF_MARKER_SIZE_METERS  0.05f                          // marker size in meters

#define CONF_FRAME_UNDISTORT    0      // undistort the image? (only useful with wide angle lenses
#define CONF_FRAME_RESIZE       0      // resize the camera image according to view size?
#define CONF_ANTIALIAS          0      // set anti-aliasing enabled (1) or disabled (0)

#define CONF_VERTICAL_SYNC  1       // enable vertical sync for graphic output?

#define CONF_CONTENT_DIR    "./res/contents"    // folder where all the contents reside

#define CONF_STOPWATCH      0       // enable benchmarking output

#define CONF_MARKER_RECOGNITION_TIMEOUT_MILLISEC    250     // marker timout after which it will not be displayed anymore
#define CONF_RESTART_VIDEO_AFTER_MILLISEC           2500    // time since last marker tracking after which a video will be completely stopped

#endif
