//
//  ContentLoader.cpp
//  ARMagicMirrorARTP
//
//  Author: Markus Konrad, Philipp Jährling
//  Copyright (c) 2013 INKA Research Group. All rights reserved.
//

#include <sstream>

#include "ContentLoader.h"
#include "ImgData.h"
#include "VidData.h"
#include "ObjData.h"
#include "Tools.h"

#pragma mark singleton stuff
//--------------------------------------------------------------
ContentLoader* ContentLoader::instance = NULL;

ContentLoader* ContentLoader::getInstance() {
    if (!instance) instance = new ContentLoader();
    return instance;
}

void ContentLoader::destroy() {
    if (instance) {
        instance->dealloc();
        delete instance;
        instance = NULL;
    }
}


#pragma mark public methods
//--------------------------------------------------------------
ContentElement* ContentLoader::getContentById(const int id) {
    ContentMap::iterator it = contents.find(id);
    if ( it == contents.end() ) {
        return NULL;
    } else {
        return it->second;
    }
}

//--------------------------------------------------------------
ContentMap ContentLoader::getAllContents() {
    return contents;
}

//--------------------------------------------------------------
void ContentLoader::loadContents(string path) throw (runtime_error) {
    // add a "/" at the end if necessary
    if (path.at(path.length() - 1) != '/') {
        path.append("/");
    }
    
    ofXml confXml;
    if ( confXml.load(path + "config.xml") ) {
        confXml.setTo("contents"); // root element
        
        int elementCount = confXml.getNumChildren();
        cout << "INFO: Try to load " << elementCount << " content elements from " << path << endl;
        
        // go through all child elements
        for (int i = 0; i < elementCount; i++) {
            confXml.setToChild(i); // set to child node
            
            // get and check the mandatory fields
            int markerId        = confXml.getIntValue("markerId");
            ContentType type    = getType( confXml.getValue("type") );
            
            if ( (markerId > 0) && (type != _UNKNOWN) && (confXml.exists("files")) ) {
                // create new element
                ContentElement *contentElem = new ContentElement(markerId, confXml.getValue("name"), type);
                // fill type data
                fillTypeDate(&confXml, contentElem, path);
                // configure element
                configureContentElem(&confXml, contentElem, path);
                // insert the new element
                contents.insert(pair<int, ContentElement *>(markerId, contentElem));
            }
            
            // set back to root node
            confXml.setToParent();
        }
        
        confXml.clear();
        
    } else {
        string err("Error accessing path: ");
        err.append(path);
        throw runtime_error(err);
    }
    
}

#pragma mark private methods
//--------------------------------------------------------------
void ContentLoader::dealloc() {
    contents.empty();
}

//--------------------------------------------------------------
ContentType ContentLoader::getType(string s) {
    if (s.compare("img") == 0) {
        return IMG;
    } else if (s.compare("vid") == 0) {
        return VID;
    } else if (s.compare("obj") == 0) {
        return OBJ;
    }
    return _UNKNOWN;
}

//--------------------------------------------------------------
void ContentLoader::fillTypeDate(ofXml* confXml, ContentElement* elem, string path) {
    
    
    
    confXml->setTo("files");
    int numFiles = confXml->getNumChildren();
    
    for (int i = 0; i < numFiles; i++) {
        // set file and path names
        std::ostringstream xmlFilePath;
        xmlFilePath << "file[" << i << "]";
        
        string file = confXml->getValue(xmlFilePath.str());
        string fullPath = path + file;

        // Handle different content types
        if (elem->type == IMG) {            // ********* IMAGE *********
            
            ofPixels pxls;
            bool imgLoaded = ofLoadImage(pxls, fullPath);
            
            if (imgLoaded) {
                bool alpha = ( pxls.getNumChannels() == 4 ) ? true : false;
                elem->typeData = new ImgData( pxls, pxls.getWidth(), pxls.getHeight(), alpha );
                cout << "INFO: loaded image " << fullPath << endl;
            }
            break;  // no more files needed
            
        } else if (elem->type == VID) {     // ********* VIDEO *********
            
            ofVideoPlayer* vidPlayer = new ofVideoPlayer;
            if ( !vidPlayer->loadMovie(fullPath) ) {
                cerr << "ERROR: could not load video data from file: " << fullPath << endl;
                delete vidPlayer;
                continue; // try the next element or break if there is none left
            }
            
            confXml->setToParent();
            bool alpha = confXml->getBoolValue("alpha");
            float vol = 1.0f;
            if (confXml->exists("vol")) {
                vol  = confXml->getFloatValue("vol");
            }
            confXml->setTo("files");
            // create and assign the video objekt
            elem->typeData = new VidData(vidPlayer, alpha, vol);
                
            cout << "INFO: loaded video " << fullPath << " (is alpha: " << alpha << ")" << endl;
            break;  // no more files needed
            
        } else if (elem->type == OBJ) {     // ********* 3D Object *********
            
            string fileExt = file.substr(file.find_last_of('.') + 1);
            
            // create a new empty data object if this is the first file
            if (!elem->typeData) {
                elem->typeData = new ObjData();
            }
            
            ObjData* obj = (ObjData*)elem->typeData;
            
            // fill OBJData with obj-model and png-texture
            if ( fileExt.compare("obj") == 0 && !obj->modelId ) {   // found the obj model file
                obj->modelId = Tools::createModelFromObj(fullPath);
                cout << "loaded obj " << fullPath << endl;
                
            } else if ( (fileExt.compare("png") == 0 || fileExt.compare("jpg") == 0) && !obj->texId) {   // found the texture
                obj->texId = Tools::createTexture(fullPath);
                cout << "loaded obj texture " << fullPath << endl;
            }
            
        }
    }
    confXml->setToParent();
}

//--------------------------------------------------------------
void ContentLoader::configureContentElem(ofXml* confXml, ContentElement* elem, string path) {
    
    // add explanation 
    if (confXml->exists("explanation") && confXml->exists("explanation/file")) {
        ofPixels pxls;
        string file     = confXml->getValue("explanation/file");
        string fullPath = path + file;
        bool imgLoaded  = ofLoadImage(pxls, fullPath);
        
        if (imgLoaded) {
            bool alpha = ( pxls.getNumChannels() == 4 ) ? true : false;
            
            float scale = 1;
            if (confXml->exists("explanation/scaling")) {
                scale = confXml->getFloatValue("explanation/scaling");
            }
            
            elem->setExplanation(
                new ImgData( pxls, pxls.getWidth(), pxls.getHeight(), alpha),
                scale,
                confXml->getFloatValue("explanation/posTranslation/x"),
                confXml->getFloatValue("explanation/posTranslation/y"),
                confXml->getFloatValue("explanation/posTranslation/z")
            );
        }
    }

    // set content init scaling
    float scale = confXml->getFloatValue("scaling");
    if (scale > 0) {
        elem->setElemScaling(scale);
    }
    
    // set content init rotation
    if (confXml->exists("rotation")) {
        int rotX = confXml->getIntValue("rotation/x");
        int rotY = confXml->getIntValue("rotation/y");
        int rotZ = confXml->getIntValue("rotation/z");
        elem->setElemRotation(rotX, rotY, rotZ);
    }
    
    // set content init position translation
    /* IMPORTANT ... this has to be set last. 
     * To center the content according to height and width, 
     * typeData needs to be filled and the scaling has to be set 
     */
    if (confXml->exists("posTranslation")) {
        float transX = confXml->getFloatValue("posTranslation/x");
        float transY = confXml->getFloatValue("posTranslation/y");
        float transZ = confXml->getFloatValue("posTranslation/z");
        elem->setElemTranslation(transX, transY, transZ);
    } else {
        elem->setElemTranslation();
    }
}