#pragma once

#include "ofMain.h"
//#include "ofxPostProcessing.h"
#include "ofxSyphon.h"
#include "ofxDOF.h"

class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void shuffleBoxes();
    
        vector<ofBoxPrimitive> boxes;
        ofLight pointLight;
        ofLight pointLight2;
        ofLight pointLight3;
        ofMaterial material;
    
//        ofxDOF depthOfField;
        ofEasyCam camera;
    
        // For Syphon Recording
        ofxSyphonServer mainOutputSyphonServer;
        ofxSyphonServer individualTextureSyphonServer;
    
        // For Post-Processing
//        ofxPostProcessing post;
};
