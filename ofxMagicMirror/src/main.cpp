#include "ofMain.h"

#include "MagicMirrorApp.h"

//========================================================================
int main( ){
    // can be OF_WINDOW or OF_FULLSCREEN
	//ofSetupOpenGL(1024,768, OF_WINDOW);
    ofSetupOpenGL(320, 240, OF_WINDOW); // resize after cam. init
	ofRunApp(new MagicMirrorApp());
}
