//
//  Data.h
//  ARMagicMirrorARTP
//
//  Author: Philipp Jährling
//  Copyright (c) 2013 INKA Research Group. All rights reserved.
//
//  Abstract parent class (non instantiatable) for the different kinds of Data.
//  We need this to use the different types of Content Data in one attribute of a ContentElement Object.
//

#pragma once

#include "ofMain.h"

class Data {
    public:
        virtual ~Data() {}
        virtual void draw() = 0;
};
