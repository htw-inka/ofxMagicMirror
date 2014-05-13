# Magic Mirror Augmented Reality Application

## Overview

_Magic Mirror_ is an augmented reality application designed for showing different kinds of media on a display, while a marker is tracked. The display shows the mirrored output of a camera attached to that display. The following kinds of media can be displayed:

* images (also with an alpha-channel, for example in PNG images)
* video (also with an alpha mask [like here](http://jakearchibald.com/scratch/alphavid/)
* 3D models (in Wavefront OBJ format)

These contents have to be added to the `res/contents` folder.

There are two different versions available, using different libraries and with differing development states.
Both versions are [OpenFrameworks](http://openframeworks.cc)-based applications and use [Nate Robins'](http://user.xmission.com/~nate/) _GLM_ library for OBJ Wavefront fromat model loading.

## ofxMagicMirror

This Version uses the [_ArUco_ library](http://www.uco.es/investiga/grupos/ava/node/26) for marker recognition and OpenGL for display. 

The content folders have to follow naming convention to associate the content with a marker ID and a content type:

* `res/contents/<id>_<type>_<name>/<name>.<ext>`
* image example: `res/contents/42_img_helloworld/helloworld.png`

Where _id_ is the marker ID and _type_ can be either _img_, _vid_ or _obj_. You can set _name_ as you wish.

Get more detailed Information [here](ofxMagicMirror/).

## ofxMagicMirrorARTP

This Version uses the [_ARToolKitPlus_ library](https://launchpad.net/artoolkitplus) for marker recognition and OpenGL for display. 

The content folder includes a _config.xml_ file, where you can add or change the loaded content, associate with a marker ID.
There are several options to specify the content type (_img_, _vid_ or _obj_), the file locations, the scaling factor, an initial rotation and translation in 3D space and if you want the content to be shown with transparency.

Get more detailed Information [here](ofxMagicMirrorARTP/).

## Compiling

To compile the project, you must have a folder `ofx` in the project root containing (or linking to) the openFrameworks library. This project has been successully compiled under Mac OSX 10.8 with openFrameworks v0.8.

Note that you should modify `src/conf.h` for the proper camera device ID and camera resolution. You should also calibrate your camera and save the intrinsic matrix output to `res/cams`.

## License

The program is released under BSD 3-clause license which is contained in the file `LICENSE`.

The included [_ArUco_ library](http://www.uco.es/investiga/grupos/ava/node/26) is developed by the [Universidad de Córdoba](http://www.uco.es/).
