//
//  townLight.h
//  trainShadow
//
//  Created by nami on 2018/01/08.
//
//

#pragma once

#include "ofMain.h"

using namespace glm;

class townLight {
public:
    townLight(ofFbo* g_fbo, ofShader* g_shader);
    bool update();
    void begin();
    void end();
    
    ofCamera light;
    float create_at;
    ofFbo fbo;
    ofTexture tex;
    ofShader* shader;
    
    vec3 position;
};
