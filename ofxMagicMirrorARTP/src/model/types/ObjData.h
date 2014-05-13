//
//  ObjData.h
//  ARMagicMirror
//
//  Author: Philipp Jährling
//  Copyright (c) 2013 INKA Research Group. All rights reserved.
//
//  The 3D-Object content specific data. With <Data> class as parent class.
//

#pragma once

#include "Data.h"

class ObjData : public Data {
    
    public:
        GLuint modelId; // 3d object id
        GLuint texId;   // texture id
    
        /**
         * Create an empty 3D-Object specific data object (the modelId and texId have to be set up after creation)
         */
        ObjData();
    
        /**
         * Cleanup model and texture
         */
        ~ObjData();
    
        /**
         * Draw the content Element. (Called in GLView)
         */
        void draw();
    
    private:
        /**
         * Calculate the size and the offsets
         */
        void calcOffsets();
};




