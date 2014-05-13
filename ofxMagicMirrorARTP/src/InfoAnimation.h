//
//  InfoAnimation.h
//  ofxMagicMirrorARTP
//
//  Created by Philipp on 4/23/14.
//
//
#pragma once

#include "ofMain.h"

class InfoAnimation {
    public:
        /**
         * 
         */
        InfoAnimation();
    
        /**
         *
         */
        ~InfoAnimation();
    
        /**
         * 
         */
        void update();
        
        /**
         * draw
         */
        void draw();
    
        /**
         *
         */
        void moveIn();
    
        /**
         * 
         */
        void moveOut();

    private:
        int movement;
        int y;
        int top;
        int bottom;
    
        ofVideoPlayer* player;
};
