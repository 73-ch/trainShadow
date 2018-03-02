//
//  townLight.h
//  trainShadow
//
//  Created by nami on 2018/01/08.
//
//

#pragma once

#include "ofMain.h"
#include "ofxShaderTex.h"

using namespace glm;

class townLight {
public:
    townLight(ofFbo* g_fbo, ofxShaderTex* g_shader);
    bool update();
    void begin();
    void end();
    ofTexture getTexture();
    
    ofCamera light;
    float create_at;
    ofFbo fbo;
    ofxShaderTex* shader;
    
    vec3 position;
    
    mat4 btm;
    mat4 tm;
};
