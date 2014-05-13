//
//  ImgData.h
//  ARMagicMirror
//
//  Author: Philipp Jährling
//  Copyright (c) 2013 INKA Research Group. All rights reserved.
//
//  The Image content specific data. With <Data> class as parent class.
//

#pragma once

#include "Data.h"

class ImgData : public Data {
    
    public:
        /**
         * The Constructor will set and calculate all the attributes
         */
        ImgData(ofPixels texPixel, int width, int height, bool alpha);
    
        /**
         * Cleanup texture
         */
        ~ImgData();
    
        /**
         * Draw the content Element. (Called in GLView)
         */
        void draw();
    
        /**
         * "Getter" Functions
         */
        int getWidth();
        int getHeight();
        bool isAlpha();
    
    private:
        ofTexture tex;     // texture
        int w;             // image width in pixels
        int h;             // image height in pixels
        bool alpha;        // number of channels in image
};
