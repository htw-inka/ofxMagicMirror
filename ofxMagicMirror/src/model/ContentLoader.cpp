//
//  ContentLoader.cpp
//  ARMagicMirror
//
//  Created by Markus Konrad on 19.08.13.
//  Copyright (c) 2013 INKA Research Group. All rights reserved.
//

#include "ContentLoader.h"

#pragma mark singleton stuff

ContentLoader *ContentLoader::instance = NULL;

ContentLoader *ContentLoader::getInstance() {
    if (!instance) instance = new ContentLoader;
    return instance;
}

void ContentLoader::destroy() {
    if (instance) {
        instance->dealloc();

        delete instance; instance = NULL;
    }
}

#pragma mark public methods

void ContentLoader::loadContents(const char *path) throw (runtime_error) {
    // open the directory at <path>
    ofDirectory dir(path);
    
    cout << "loading contents at path: " << dir.getAbsolutePath() << endl;

    if (!dir.exists()) {
        string err("Error accessing path: ");
        err.append(path);
        
        dir.close();
        
        throw runtime_error(err);
    }
    
    // convert it to a string and add a "/" if necessary
    string pathStrOrig(path);
    if (pathStrOrig.at(pathStrOrig.length() - 1) != '/') {
        pathStrOrig.append("/");
    }
    
    // go through the elements of the directory
    const int numFiles = dir.listDir();
    for (int i = 0; i < numFiles; i++) {
        ofDirectory curPath(dir.getPath(i));
        if (!curPath.isDirectory()) continue;    // dismiss everything other than folders
        
        string pathStr = pathStrOrig;
        string elemDir = dir.getName(i);    // element name as string
        
        // transform to lower case
        transform(elemDir.begin(), elemDir.end(), elemDir.begin(), ::tolower);
        
        if (elemDir.at(0) == '.') continue;   // no ".", "..", or hidden files, please!
        
        // split string by parts: marker id, content type, content name
        vector<string> elems;
        Tools::splitString(elemDir, '_', elems);
        
        int markerId = atoi(elems[0].c_str());  // get the marker id
        if (contents.find(markerId) != contents.end()) continue;    // dismiss double IDs
        ContentType type = getType(elems[1]);   // get the content type
        
        // create a new content element
        ContentElem *contentElem = new ContentElem(markerId, type);
        
        Tools::copyStringToBuf(elems[2], &contentElem->name);   // set the name
        
        // create the full path and configure the element according its type
        const string fullPath = pathStr.append(elemDir);
        configureContentElem(fullPath, contentElem, elems);
        
        // insert the new element
        contents.insert(pair<int, ContentElem *>(markerId, contentElem));
    }
    
    // clean up
    dir.close();
    
    // debug output
#ifdef DEBUG
    cout << "LOADED CONTENTS:" << endl;
    for (ContentMap::iterator it = contents.begin(); it != contents.end(); ++it) {
        cout << "elem: " << it->second->id << " type:" << it->second->type << " name: " << it->second->name << endl;
        
        if (it->second->mainFile) {
            cout << "> mainFile: " << it->second->mainFile << endl;
        }
        
        if (it->second->extraFiles) {
            for (int i = 0; i < it->second->numExtraFiles; i++) {
                cout << "> extraFile[" << i << "]: " << it->second->extraFiles[i] << endl;
            }
        }
    }
#endif
}

ContentElem *ContentLoader::getContent(const int id) {
    ContentMap::iterator it = contents.find(id);
    
    if (it == contents.end()) return NULL;
    
    return it->second;
}

#pragma mark private methods

void ContentLoader::dealloc() {
    contents.empty();
}

ContentType ContentLoader::getType(string &s) {
    if (s.compare("img") == 0) {
        return IMG;
    } else if (s.compare("vid") == 0) {
        return VID;
    } else if (s.compare("obj") == 0) {
        return OBJ;
    }
    
    return _UNKNOWN;
}

void ContentLoader::configureContentElem(const string &basePath, ContentElem *elem, vector<string> &folderParts) {
    ofDirectory dir(basePath);
    const string absBasePath(dir.getAbsolutePath());
    
    // read the objects in the content element's path
    const int numFiles = dir.listDir();
    for (int i = 0; i < numFiles; i++) {
        // set file and path names
        string file(dir.getName(i));
        string base = absBasePath;
        string fullPath = base.append("/").append(file);
        
        transform(file.begin(), file.end(), file.begin(), ::tolower);   // lowercase!
        
        if (file.at(0) == '.') continue;   // no ".", "..", or hidden files, please!
        
        if (elem->type == IMG || elem->type == VID) {   // there will be only 1 file. take the first
            Tools::copyStringToBuf(fullPath, &elem->mainFile);
            
            if (elem->type == IMG) {    // create extra data: IMGData
                IMGData *imgData = new IMGData;
                
                // create a texture from the image
                imgData->texId = Tools::createTexture(elem->mainFile, &imgData->w, &imgData->h, 0, &imgData->chan);

                // set quad dimensions and offsets
                calcQuadSizeAndOffsetsFromImageDimensions(imgData->w, imgData->h,
                                                          &(imgData->quadW), &(imgData->quadH),
                                                          &(imgData->offX), &(imgData->offY));
                
                // set the extra data
                elem->extraData = imgData;
                
                cout << "loaded image " << fullPath << " of size "
                        << imgData->w
                        << "x" << imgData->h
                        << " with channels: " << imgData->chan
                        << " quad rect: " << imgData->quadW << "," << imgData->quadH << "@" << imgData->offX << "," << imgData->offY
                        << endl;
            } else if (elem->type == VID) { // create extra data: VIDData
                VIDData *vidData = new VIDData;
                
                // create a video capture object and get the first frame
                vidData->vidPlayer = new ofVideoPlayer;
                
                if (!vidData->vidPlayer->loadMovie(fullPath)) {
                    cerr << "could not load video data from file " << fullPath << endl;
                    break;
                }
                
                // set further properties
                vidData->w = vidData->vidPlayer->getWidth();
                vidData->h = vidData->vidPlayer->getHeight();
//                vidData->numFrames = vidData->vidCap->get(CV_CAP_PROP_FRAME_COUNT);
                
                if (folderParts.size() >= 4 && folderParts.at(3).compare("alpha") == 0) {   // will this be an alpha video?
                    vidData->alpha = true;
                    
                    // set rects for frame parts
                    vidData->frameW = vidData->w;
                    vidData->frameH = vidData->h  / 2;
                    vidData->origFrameRect = cv::Rect(0, 0, vidData->frameW, vidData->frameH);
                    vidData->maskFrameRect = cv::Rect(0, vidData->frameH, vidData->frameW, vidData->frameH);
                } else {
                    vidData->frameW = vidData->w;
                    vidData->frameH = vidData->h;
                    vidData->alpha = false;
                }
                
                // set quad dimensions and offsets
                calcQuadSizeAndOffsetsFromImageDimensions(vidData->frameW, vidData->frameH,
                                                          &(vidData->quadW), &(vidData->quadH),
                                                          &(vidData->offX), &(vidData->offY));
                
                cout << "loaded video " << fullPath << " with frames of size "
                        << vidData->w << "x" << vidData->h
                        << " is alpha: " << vidData->alpha << endl;
                
                // set extra data
                elem->extraData = vidData;
            }
            
            break;  // no more files needed
        } else if (elem->type == OBJ) { // we have a 3D object
            // file extension
            string ext = file.substr(file.find_last_of('.') + 1);
            
            if (!elem->extraData) { // create extra data: OBJData
                elem->extraData = new OBJData;
            }
            
            // fill OBJData with obj-model and png-texture
            if (ext.compare("obj") == 0 && !elem->mainFile) { // found the obj model file
                Tools::copyStringToBuf(fullPath, &elem->mainFile);
                ((OBJData *)elem->extraData)->modelId = Tools::createModelFromObj(elem->mainFile);
                
                cout << "loaded obj " << fullPath << endl;
            } else if ((ext.compare("png") == 0 || ext.compare("jpg") == 0) && !elem->extraFiles) {  // found the texture
                elem->extraFiles = new char*;
                elem->numExtraFiles = 1;
                Tools::copyStringToBuf(fullPath, &elem->extraFiles[0]);
                ((OBJData *)elem->extraData)->texId = Tools::createTexture(elem->extraFiles[0]);
                
                cout << "loaded obj texture " << fullPath << endl;
            }
        }
    }
    
    dir.close();
}

void ContentLoader::calcQuadSizeAndOffsetsFromImageDimensions(GLuint w, GLuint h,
                                                              GLfloat *quadW, GLfloat *quadH,
                                                              GLfloat *offX, GLfloat *offY) {
    assert(w > 0 && h > 0);
    
    if (w >= h) {
        *quadW  = 1.0f;
        *quadH = (GLfloat)h / (GLfloat)w;
        *offX = -0.5f;
        *offY = -0.5f + (GLfloat)(*quadW - *quadH) / 2.0f;
    } else {
        *quadW = (GLfloat)w / (GLfloat)h;
        *quadH  = 1.0f;
        *offX = -0.5f + (GLfloat)(*quadH - *quadW) / 2.0f;
        *offY = -0.5f;
    }
}