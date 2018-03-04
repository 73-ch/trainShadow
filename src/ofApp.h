#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxAssimpModelLoader.h"
#include "townLight.h"
#include "ofxShaderTex.h"

using namespace glm;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
    
    ofEasyCam cam;
    ofCamera light;
    
    vector <townLight> lights;
    int light_count;
    int light_max;
    
    vector<ofBoxPrimitive> boxes;
    
    ofVboMesh vbo;
    
    ofBoxPrimitive box;
    
    ofxShaderTex shader;
    ofxShaderTex d_shader;
    ofxShaderTex t_shader;
    
    ofFbo fbo;
    
    ofxOscReceiver receiver;
    
    ofxAssimpModelLoader loader;
    
    mat4 tmpm;
    mat4 btm;
    mat4 tm;
    mat4 dvpm;
    mat4 mmm;
    
    ofMesh mesh;
    
    float tolerate;
};
