//
//  InfoAnimation.cpp
//  ofxMagicMirrorARTP
//
//  Created by Philipp on 4/23/14.
//
//

#include "InfoAnimation.h"
#include "conf.h"

//--------------------------------------------------------------
InfoAnimation::InfoAnimation() {
    bottom  = CONF_CAM_H;
    top     = bottom - CONF_INFO_HEIGHT;
    y       = bottom;
#if CONF_INFO_SHOW_ON_START == 1
    movement = - CONF_INFO_SPEED;
#else
    movement = CONF_INFO_SPEED;
#endif
    player  = new ofVideoPlayer;
    if (player->loadMovie(CONF_INFO_ANIM_PATH)) {
        player->play();
        cout << "INFO: Loaded Info-Gif from: " << CONF_INFO_ANIM_PATH << endl;
    } else {
        cout << "ERROR: Loading Info-Gif from " << CONF_INFO_ANIM_PATH << " faild" << endl;
    }
}

//--------------------------------------------------------------
InfoAnimation::~InfoAnimation() {
    if (player) delete player;
    // nothing to do here by now
}

//--------------------------------------------------------------
void InfoAnimation::update() {
    if (movement < 0) {
        if (y > top && y <= bottom ) {
            y += movement;
        }
    } else {
        if (y < bottom ) {
            y += movement;
        }
    }
    player->update();
}

//--------------------------------------------------------------
void InfoAnimation::draw() {
    ofDisableDepthTest();
    ofEnableAlphaBlending();
    
        ofPushMatrix();
            //ofTranslate(0, y);
            ofSetColor(0, 0, 0, 150);
            ofRect(0, y, CONF_CAM_W, bottom - y);
            ofSetColor(255, 255, 255);
    
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);
            
            player->draw(200, y, 250, 250);
            
            glDisable(GL_BLEND);
            glBlendFunc(GL_ONE,GL_ZERO);
        ofPopMatrix();
    
    ofDisableAlphaBlending();
    ofEnableDepthTest();
}

//--------------------------------------------------------------
void InfoAnimation::moveIn() {
    if (movement > 0) {
        movement *= -1;
    }
}

//--------------------------------------------------------------
void InfoAnimation::moveOut() {
    if (movement < 0) {
        movement *= -1;
    }
}


