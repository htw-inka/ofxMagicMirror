//
//  ContentLoader.h
//  ARMagicMirror
//
//  Created by Markus Konrad on 19.08.13.
//  Copyright (c) 2013 INKA Research Group. All rights reserved.
//
//  ContentLoader holds all content objects that are associated with a
//  marker ID.
//
//  The content information will be loaded by analyzing the directory names
//  in a "contents" directory. The folders there need to be named in the
//  format:
//      <markerID>_<contentType>_<contentName>
//  where <contentType> can be "img", "vid", "obj".
//  In the folders themselves are the content files (a video, a picture, etc.)
//

#ifndef __ARMagicMirror__ContentLoader__
#define __ARMagicMirror__ContentLoader__

#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

#include "Tools.h"

#include "ContentTypes.h"

using namespace std;

/**
 * Define a map that associates a marker id with a content element pointer.
 */
typedef map<int, ContentElem *> ContentMap;

/**
 * ContentLoader holds all content objects that are associated with a
 * marker ID.
 * Singleton class.
 */
class ContentLoader {
public:
    /**
     * Get singleton instance.
     */
    static ContentLoader *getInstance();
    
    /**
     * Destroy the singleton instance.
     */
    static void destroy();

    /**
     * Load all contents from a directory at <path>.
     */
    void loadContents(const char *path) throw (runtime_error);
    
    /**
     * Return a content element pointer for a marker <id>.
     */
    ContentElem *getContent(const int id);
    
    /**
     * Return the complete contents map.
     */
    ContentMap &getAllContents() { return contents; };
    
private:
    static ContentLoader *instance;         // Singleton instance or NULL.
    
    ContentLoader() {};                      // Empty. Is a Singleton!
    ContentLoader(ContentLoader const&);     // Don't Implement
    void operator=(ContentLoader const&);    // Don't implement
    
    /**
     * Deallocate used objects.
     * Called before singleton instance destruction.
     */
    void dealloc();
    
    /**
     * Identify a content type by lowercase string <s> and return it.
     * <s> can be "img", "vid" or "obj".
     */
    ContentType getType(string &s);
    
    /**
     * Configure a content element <elem> at <basePath> according to its type:
     * Will set the associated files and optionally add "extra data" (e.g.
     * 3D object data)
     */
    void configureContentElem(const string &basePath, ContentElem *elem, vector<string> &folderParts);
    
    /**
     * Calculate the size and the offsets for an GL quad described by <quadW>, <quadH>,
     * <offX> and <offY> from image dimensions <w> and <h>.
     * Output units are in marker units (e.g. 1.0f is full width or height of a marker).
     */
    inline void calcQuadSizeAndOffsetsFromImageDimensions(GLuint w, GLuint h, GLfloat *quadW, GLfloat *quadH, GLfloat *offX, GLfloat *offY);

    /**
     * map containing all content elements (as pointers) associated with a
     * marker ID as key.
     */
    ContentMap contents;
};

#endif /* defined(__ARMagicMirror__ContentLoader__) */
