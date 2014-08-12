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
#if CONF_INFO_SIDE == 1
    bottom  = 0;
    top     = CONF_INFO_HEIGHT;
    pos     = bottom;
    gifPos  = (CONF_CAM_H - CONF_INFO_SIZE) / 2;
#elif CONF_INFO_SIDE == 2
    bottom  = CONF_CAM_W;
    top     = bottom - CONF_INFO_HEIGHT;
    pos     = bottom;
    gifPos  = (CONF_CAM_H - CONF_INFO_SIZE) / 2;
#else
    bottom  = CONF_CAM_H;
    top     = bottom - CONF_INFO_HEIGHT;
    pos     = bottom;
    gifPos  = (CONF_CAM_W - CONF_INFO_SIZE) / 2;
#endif
    
        
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
}

//--------------------------------------------------------------
void InfoAnimation::update() {
#if CONF_INFO_SIDE == 1
    if (movement < 0) {
        if (pos >= bottom) {
            pos += movement;
        }
    } else {
        if (pos <= top ) {
            pos += movement;
        }
    }
#else
    if (movement < 0) {
        if (pos >= top) {
            pos += movement;
        }
    } else {
        if (pos <= bottom ) {
            pos += movement;
        }
    }
#endif
    player->update();
}

//--------------------------------------------------------------
void InfoAnimation::draw() {
    ofDisableDepthTest();
    ofEnableAlphaBlending();
    
        ofPushMatrix();
            ofSetColor(0, 0, 0, 150);
    
#if CONF_INFO_SIDE == 1
            ofRect(0, 0, pos, CONF_CAM_H);
#elif CONF_INFO_SIDE == 2
            ofRect(pos, 0, bottom, CONF_CAM_H);
#else
            ofRect(0, pos, CONF_CAM_W, bottom - pos);
#endif
    
            ofSetColor(255, 255, 255);
    
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);
    
#if CONF_INFO_SIDE == 1
            // Hier dann erst translate, dann evtl. rotate und dann draw
            ofTranslate(pos, gifPos);
            ofRotate(90, 0, 0, 1);
            player->draw(0, 0, CONF_INFO_SIZE, CONF_INFO_SIZE);
#elif CONF_INFO_SIDE == 2
            ofTranslate(pos, gifPos + CONF_INFO_SIZE);
            ofRotate(270, 0, 0, 1);
            player->draw(0, 0, CONF_INFO_SIZE, CONF_INFO_SIZE);
#else
            player->draw(gifPos, pos, CONF_INFO_SIZE, CONF_INFO_SIZE);
#endif
            
            glDisable(GL_BLEND);
            glBlendFunc(GL_ONE,GL_ZERO);
        ofPopMatrix();
    
    ofDisableAlphaBlending();
    ofEnableDepthTest();
}

//--------------------------------------------------------------
void InfoAnimation::moveIn() {
#if CONF_INFO_SIDE == 1
    if (movement < 0) {
        movement *= -1;
    }
#else
    if (movement > 0) {
        movement *= -1;
    }
#endif
}

//--------------------------------------------------------------
void InfoAnimation::moveOut() {
#if CONF_INFO_SIDE == 1
    if (movement > 0) {
        movement *= -1;
    }
#else
    if (movement < 0) {
        movement *= -1;
    }
#endif
    
}


