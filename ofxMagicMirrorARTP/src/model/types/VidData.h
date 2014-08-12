//
//  VidData.h
//  ARMagicMirror
//
//  Author: Philipp Jährling
//  Copyright (c) 2013 INKA Research Group. All rights reserved.
//
//  The Video content specific data. With <Data> class as parent class.
//

#pragma once

#include "Data.h"

class VidData : public Data {
    
    public:
        /**
         * The Constructor will set and calculate all the attributes
         */
        VidData(ofVideoPlayer* vidPlayer, bool alpha, float vol = 1.0f);
        
        /**
         * Cleanup videoplayer etc.
         */
        ~VidData();
        
        /**
         * Draw the content Element. (Called in GLView)
         */
        void draw();
        
        /**
         * Pause the encapsulated viedo player
         */
        void pauseVideo();
        
        /**
         * Resume a paused Video
         */
        void resumeVideo();
        
        /**
         * Stop and rewind the encapsulated video player
         */
        void stopVideo();
    
        /**
         * "Getter" Functions
         */
        int getWidth();
        int getHeight();
        bool isAlpha();
        bool isPaused();
    
    private:
        ofVideoPlayer* vidPlayer;   // Ofx video capturerer to read video frames
        int w;                      // full video frame width
        int h;                      // full video frame height
        ofTexture vidTex;           // texture for the original frame
        bool alpha;                 // is this an video with alpha mask?
        bool startPlayback;         // should the video be started on the next draw loop?
};

