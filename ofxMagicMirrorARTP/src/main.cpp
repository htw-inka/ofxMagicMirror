#include "ofMain.h"
#include "app/ofAppGLFWWindow.h"
#include "conf.h"
#include "MagicMirrorApp.h"

//========================================================================
int main( ){
    // use "GLFW" window, which lets us control the GL buffers bit depth
    ofAppGLFWWindow *window = new ofAppGLFWWindow();
    window->setStencilBits(8);  // enables stencil buffer
    window->setDepthBits(8);    // enables depth buffer
	ofSetupOpenGL(window, CONF_CAM_W, CONF_CAM_H, OF_WINDOW);
    
    //ofSetupOpenGL(CONF_CAM_W, CONF_CAM_H, OF_FULLSCREEN); // <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new MagicMirrorApp() );

    // cleanup
    delete window;
}
