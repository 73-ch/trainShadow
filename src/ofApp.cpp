#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // of & gl init
    ofSetVerticalSync(false);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofEnableDepthTest();
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    GLint textureUnits = 0;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
//    cout << "texture units " + ofToString(  cntextureUnits) << endl;
    light_max = textureUnits;
    //
    
    // depth fbo
    float length = 4096;
    ofFbo::Settings settings;
    settings.width = length;
    settings.height = length;
    settings.useDepth = true;
    settings.useStencil = true;
    settings.depthStencilAsTexture = true;
    fbo.allocate(settings);
    //
    
    // shader
    shader.load("vert.vert", "frag.frag");
    d_shader.load("dvert.vert", "dfrag.frag");
    //
    
    // osc
    receiver.setup(12345);
    //
    
    // cam
    cam.setPosition(vec3(0,300,0));
    cam.setUpAxis(vec3(0,1,0));
    cam.setFarClip(1500.);
    //
    
    // light
//    light.setPosition(vec3(150, 100, -500));
//    light.setFov(50.0f);
//    light.lookAt(light.getGlobalPosition() + vec3(-100, 0,0));
//    light.rotateDeg(20.0f, vec3(0,0,1));
    
    light_count = 0;
    //
    
    // box
    box.setPosition(vec3(110, 100,-100));
    box.set(50);
    
    
    // ライトごとになってない
    btm = scale(btm, vec3(fbo.getWidth(), fbo.getHeight(), 1.0f));
    btm = btm * mat4(0.5,0,0,0, 0,0.5,0,0, 0,0,1.0,0, 0.5,0.5,0,1.0);
    //
    
    // model
    loader.setScale(2.0,2.0,2.0);
    loader.loadModel("train_shadow.stl");
    
    mesh = loader.getMesh(0);
    for (int i = 0; i < mesh.getNumVertices(); i++) mesh.addColor(ofColor(255,255,255,255));
    //
    
    // test
    tolerate = 0.;
}

//--------------------------------------------------------------
void ofApp::update(){
    // matrix
    tmpm = cam.getModelViewProjectionMatrix();
    //
    
    // model
    loader.update();
    //
    
    // osc
    while (receiver.hasWaitingMessages()) {
        ofxOscMessage m;
        receiver.getNextMessage(m);
        if (m.getAddress() == "/light") {
            light.setPosition(m.getArgAsFloat(0), m.getArgAsFloat(1), m.getArgAsFloat(2));
        } else if (m.getAddress() == "/box") {
            box.setPosition(vec3(m.getArgAsFloat(0), m.getArgAsFloat(1), m.getArgAsFloat(2)));
        } else if (m.getAddress() == "/create_light") {
            ofCamera cam;
            lights.push_back(*new townLight(&fbo, &d_shader));
            light_count++;
            if (light_count >= light_max) light_count = 0;
        } else if (m.getAddress() == "/test") {
//            lights[0].position = vec3(m.getArgAsFloat(0), m.getArgAsFloat(1), m.getArgAsFloat(2));
        } else if (m.getAddress() == "/tolerate") {
//            tolerate = m.getArgAsFloat(0);
            lights[0].light.setFarClip(m.getArgAsFloat(0));
        }
    }
    //
    
    // lights update
    if (lights.size() > 0) {
        auto itr = lights.begin();
        while (itr != lights.end()) {
            if (lights[std::distance(lights.begin(),itr)].update()) {
                lights.erase(itr);
            } else {
                itr++;
            }
        }
    }
    //
}

//--------------------------------------------------------------
void ofApp::draw(){
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
//    glCullFace(GL_FRONT);
    
    mat4 bmm = box.getGlobalTransformMatrix();
    
    mmm = mat4(loader.getModelMatrix());
    mmm = rotate(mmm, radians(180.0f), vec3(0.0f,0.0f,1.0f));
    
//    fbo.begin();
//    ofClear(255, 255, 255, 255);
//    ofPushMatrix();
    
//    // depth fbo init
//    glClearDepth(1.0);
//    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
//    glViewport(0, 0, fbo.getWidth(), fbo.getHeight());
    
//    d_shader.begin();
//    
//    d_shader.setUniformMatrix4f("lgtMatrix", light.getModelViewProjectionMatrix() * bmm);
//    box.draw();
//    
//    d_shader.setUniformMatrix4f("lgtMatrix", light.getModelViewProjectionMatrix() * mmm);
//    mesh.draw();
//    
//    d_shader.end();
//    fbo.end();
//    ofPopMatrix();
    
//    d_shader.begin();
    
    for (int i = 0; i < lights.size(); i++) {
        lights[0].begin();
        
        d_shader.begin();
        d_shader.setUniform1f("clipD", lights[0].light.getFarClip()- lights[0].light.getNearClip());
        d_shader.setUniformMatrix4f("lgtMatrix", lights[0].light.getModelViewProjectionMatrix() * bmm);
        box.draw();

        d_shader.setUniformMatrix4f("lgtMatrix", lights[0].light.getModelViewProjectionMatrix() * mmm);
        mesh.draw();
        d_shader.end();
        lights[0].end();
    }
    
//    d_shader.end();

    
    
    // render fbo init
    ofClear(180, 180, 180, 255);
    glClearDepth(1.0);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, ofGetWidth(), ofGetHeight());
    //
    
    ofPushMatrix();
    shader.begin();
    cam.begin();
    glEnable(GL_CULL_FACE);
    ofEnableDepthTest();
    
    ofTexture tex;
//    tex = fbo.getDepthTexture();
    tex.setTextureWrap(GL_CLAMP_TO_BORDER_ARB, GL_CLAMP_TO_BORDER_ARB);
    
    shader.setUniformMatrix4f("mMatrix", bmm);
    shader.setUniformMatrix4f("mvpMatrix", tmpm * bmm);
    shader.setUniformMatrix4f("invMatrix", inverse(bmm));
    shader.setUniform1f("tolerate", tolerate);
//    shader.setUniformMatrix4f("tMatrix", btm * light.getModelViewProjectionMatrix());
//    shader.setUniformMatrix4f("lgtMatrix", light.getModelViewProjectionMatrix());
//    shader.setUniform3f("lightPosition", light.getGlobalPosition());
//    shader.setUniformTexture("d_texture", tex, 0);
//    
    
    for (int i = 0; i < 1; i++) {
        if (i >= lights.size()) {
            shader.setUniform1i("active_light[" + ofToString(i) + "]", false);
        } else {
            shader.setUniform1i("active_light[0]", true);
            shader.setUniform1f("clipD", lights[0].light.getFarClip()- lights[0].light.getNearClip());
            shader.setUniformMatrix4f("tMatrix[0]", btm * lights[0].light.getModelViewProjectionMatrix());
            shader.setUniformMatrix4f("lgtMatrix[0]", lights[0].light.getModelViewProjectionMatrix() * bmm);
            shader.setUniform3f("lightPosition[0]", lights[0].light.getGlobalPosition());
            shader.setUniformTexture("d_texture[0]", lights[0].tex, 0);
            
//            shader.setUniform1i("active_light[" + ofToString(i) + "]", true);
//            shader.setUniformMatrix4f("tMatrix[" + ofToString(i) + "]", btm * lights[i].light.getModelViewProjectionMatrix());
//            shader.setUniformMatrix4f("lgtMatrix[" + ofToString(i) + "]", lights[i].light.getModelViewProjectionMatrix() * bmm);
//            shader.setUniform3f("lightPosition[" + ofToString(i) + "]", lights[i].light.getGlobalPosition());
//            shader.setUniformTexture("d_texture[" + ofToString(i) + "]", lights[i].tex, i);
        }
    }
    
    

    ofSetColor(255, 255, 255);
    box.draw();
    
    shader.setUniformMatrix4f("mMatrix", mmm);
    shader.setUniformMatrix4f("mvpMatrix", tmpm * mmm);
    shader.setUniformMatrix4f("invMatrix", inverse(mmm));
    shader.setUniform4f("ambientColor", vec4(0.1,0.1,0.1,1.0));
    shader.setUniform1f("tolerate", tolerate);
    
    for (int i = 0; i < 1; i++) {
        if (i >= lights.size()) {
            shader.setUniform1i("active_light[" + ofToString(i) + "]", false);
        } else {
            mat4 dvpm = lookAt(lights[0].light.getGlobalPosition(), lights[i].light.getGlobalPosition() + lights[i].light.getLookAtDir(), lights[i].light.getUpDir());
            
            
            shader.setUniform1i("active_light[0]", true);
            shader.setUniformMatrix4f("tMatrix[0]", btm * lights[0].light.getModelViewProjectionMatrix());
            shader.setUniformMatrix4f("lgtMatrix[0]", lights[0].light.getModelViewProjectionMatrix() * mmm);
            shader.setUniform3f("lightPosition[0]", lights[0].light.getGlobalPosition());
            shader.setUniformTexture("d_texture[0]", lights[0].tex, 0);
            tex = lights[0].tex;
            //            shader.setUniform1i("active_light[" + ofToString(i) + "]", true);
            //            shader.setUniformMatrix4f("tMatrix[" + ofToString(i) + "]", btm * lights[i].light.getModelViewProjectionMatrix());
            //            shader.setUniformMatrix4f("lgtMatrix[" + ofToString(i) + "]", lights[i].light.getModelViewProjectionMatrix() * mmm);
            //            shader.setUniform3f("lightPosition[" + ofToString(i) + "]", lights[i].light.getGlobalPosition());
            //            shader.setUniformTexture("d_texture[" + ofToString(i) + "]", lights[i].tex, i);
        }
    }

    
    mesh.draw();
    
    cam.end();
    shader.end();
    
    // debug
    cam.begin();
    light.draw();
//    ofDrawSphere(light.getGlobalPosition() + light.getLookAtDir() * 10., 5);
    if (lights.size() > 0) {
        lights[0].light.draw();
//        cout << lights[0].light.getGlobalPosition() << endl;
    }
    cam.end();
    //
    ofPopMatrix();
    
    glDisable(GL_CULL_FACE);
    ofSetColor(0,0,255,255);
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 5, 10);
    
    for (int i = 0; i < lights.size(); i++) {
        ofSetColor(255,255,255,255);
//        ofPushMatrix();
//        lights[i].fbo.draw(i * 100, 0, 100, 100);
        tex.draw(0, 0, 100,100);
//        ofPopMatrix();
//        lights[i].tex.unbind();
    }
    
//    if(!depthBuffer) fbo.draw(0, 0, 150, 150);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
