//
//  townLight.cpp
//  trainShadow
//
//  Created by nami on 2018/01/08.
//
//

#include "townLight.h"
townLight::townLight(ofFbo* g_fbo, ofShader* g_shader) {
//    fbo = g_fbo;
    
    float length = 4096;

    ofFbo::Settings settings;
    settings.width = length;
    settings.height = length;
    settings.useDepth = true;
    settings.useStencil = true;
    settings.depthStencilAsTexture = true;
    fbo.allocate(settings);
    shader = g_shader;

    
    create_at = ofGetElapsedTimef();
    
    position = vec3(150, 100, -500);
    
    light.setNearClip(0.1f);
    light.setFarClip(1000.0f);
    light.setPosition(vec3(150, 100, -500));
    light.setFov(50.0f);
    light.lookAt(light.getGlobalPosition() + vec3(-100, 0,0));
    light.rotateDeg(20.0f, vec3(0,0,1));
}

bool townLight::update() {
    float time = (ofGetElapsedTimef() - create_at)/10;
    if (time >= 1) {
        return true;
    } else {
        light.setPosition(vec3(position.x, position.y, time*1000 - 500));
        return false;
    }
}

void townLight::begin() {
    glEnable(GL_CULL_FACE);
    
//    ofEnableDepthTest();
    fbo.begin();
    ofClear(255, 255, 255, 255);
//    ofPushMatrix();
    
    // depth fbo init
    glClearDepth(1.0);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, fbo.getWidth(), fbo.getHeight());
    //
    
    shader->begin();
}

void townLight::end() {
    shader->end();
    fbo.end();
//    ofPopMatrix();
    
//    tex = fbo.getDepthTexture();
    tex = fbo.getTexture();
    tex.setTextureWrap(GL_CLAMP_TO_BORDER_ARB, GL_CLAMP_TO_BORDER_ARB);
    
}

