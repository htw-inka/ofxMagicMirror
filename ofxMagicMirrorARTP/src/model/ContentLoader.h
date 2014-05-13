//
//  ContentLoader.h
//  ARMagicMirrorARTP
//
//  Author: Markus Konrad, Philipp Jährling
//  Copyright (c) 2013 INKA Research Group. All rights reserved.
//
//  ContentLoader holds all content objects that are associated with a marker ID.
//
//  The content information will be loaded by analyzing the directory names in a "contents" directory.
//  The folders there need to be named in the format: <markerID>_<contentType>_<contentName> where <contentType> can be "img", "vid", "obj".
//  In the folders themselves are the content files (a video, a picture, etc.)
//

#pragma once

//#include <iostream>
//#include <map>
//#include <stdexcept>
//#include <string>

#include "ContentElement.h"

//using namespace std;

/**
 * Define a map that associates a marker id with a content element pointer.
 */
typedef map<int, ContentElement *> ContentMap;

/**
 * ContentLoader holds all content objects that are associated with a marker ID.
 * Singleton class.
 */
class ContentLoader {
    public:
        /**
         * Get singleton instance.
         */
        static ContentLoader* getInstance();
    
        /**
         * Destroy the singleton instance.
         */
        static void destroy();
    
        /**
         * Return a content element pointer for a marker <id>.
         */
        ContentElement* getContentById(const int id);
    
        /**
         * Return the complete contents map.
         */
        ContentMap getAllContents();

        /**
         * Load all contents from a directory at <path>.
         */
        void loadContents(string path) throw (runtime_error);
    
    private:
        static ContentLoader* instance;          // Singleton instance or NULL.
    
        ContentLoader() {};                      // Empty. Is a Singleton!
        ContentLoader(ContentLoader const&);     // Don't Implement
        void operator=(ContentLoader const&);    // Don't implement
    
        /**
         * Deallocate used objects. Called before singleton instance destruction.
         */
        void dealloc();
    
        /**
         * Identify a content type by lowercase string <s> and return it.
         * <type> can be "img", "vid" or "obj".
         */
        ContentType getType(string type);
    
        /**
         * Fill type date according to type:
         * Will set the associated files and optionally add extra data (e.g. 3D object data)
         */
        void fillTypeDate(ofXml* confXml, ContentElement* elem, string path);
    
        /**
         * Configure a content element. Will set rotation, translation and scaling
         */
        void configureContentElem(ofXml* confXml, ContentElement* elem);

        /**
         * map containing all content elements (as pointers) associated with a
         * marker ID as key.
         */
        ContentMap contents;
};
