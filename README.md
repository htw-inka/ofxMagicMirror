# Magic Mirror Augmented Reality Application

## Overview

_Magic Mirror_ is an augmented reality application designed for showing different kinds of media on a display, while a marker is tracked. The display shows the mirrored output of a camera attached to that display. The following kinds of media can be displayed:

* images (also with an alpha-channel, for example in PNG images)
* video (also with an alpha mask [like here](http://jakearchibald.com/scratch/alphavid/)
* 3D models (in Wavefront OBJ format)

These contents have to be added to the `res/contents` folder using the following naming convention to associate the content with a marker ID and a content type:

* `res/contents/<id>_<type>_<name>/<name>.<ext>`
* image example: `res/contents/42_img_helloworld/helloworld.png`

Where _id_ is the marker ID and _type_ can be either _img_, _vid_ or _obj_. You can set _name_ as you wish.

## License

The program is released under BSD 3-clause license which is contained in the file `LICENSE`.

The included [_ArUco_ library](http://www.uco.es/investiga/grupos/ava/node/26) is developed by the [Universidad de Córdoba](http://www.uco.es/).