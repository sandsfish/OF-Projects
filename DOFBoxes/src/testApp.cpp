#include "testApp.h"

//  ofxSyphon Docs
//  https://github.com/astellato/ofxSyphon

//  How to add Syphon to XCode and Set Up the Copy Build Phase
//  http://forum.openframeworks.cc/t/an-easier-ofxsyphon/9017/13

//  James' Depth of Field Addon
//  https://github.com/obviousjim/ofxDOF

//  Neil's Post-Processing Addon
//  https://github.com/neilmendoza/ofxPostProcessing

ofColor bgColorOne, bgColorTwo;

int cubeSize = 2;
float zPos = -1050.f;
float zPosMult = 0.5f;
float noiseX = 0.01f;
float noiseY = 0.05f;

void testApp::shuffleBoxes() {
    for(int i = 0; i < boxes.size(); i++) {
        boxes[i].setPosition(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()), ofRandom(-1000, 1000));
    }
}

//--------------------------------------------------------------
void testApp::setup(){

    ofSetDataPathRoot("../Resources/data/");
    
    depthOfField.setup(ofGetScreenWidth(), ofGetScreenHeight());
    
    // Setup post-processing chain
//    post.init(ofGetWidth(), ofGetHeight());
//    post.createPass<DofPass>()->setEnabled(true);
//    post.createPass<FxaaPass>()->setEnabled(false);
//    post.createPass<BloomPass>()->setEnabled(false);
//    post.createPass<KaleidoscopePass>()->setEnabled(false);
//    post.createPass<NoiseWarpPass>()->setEnabled(false);
//    post.createPass<PixelatePass>()->setEnabled(false);
//    post.createPass<EdgePass>()->setEnabled(false);
//    post.createPass<VerticalTiltShifPass>()->setEnabled(false);
//    post.createPass<GodRaysPass>()->setEnabled(false);
    
    ofEnableSmoothing();
    ofSetFullscreen(true);
    ofHideCursor();
    
    mainOutputSyphonServer.setName("Screen Output");
	individualTextureSyphonServer.setName("Texture Output");
    
    bgColorOne.set(255, 255, 255);
    bgColorTwo.set(0, 0, 0);
    
    ofSetSmoothLighting(true);
    pointLight.setDiffuseColor( ofFloatColor(.85, .85, .55) );
    pointLight.setSpecularColor( ofFloatColor(1.f, 1.f, 1.f));
    
    pointLight2.setDiffuseColor( ofFloatColor( 238.f/255.f, 57.f/255.f, 135.f/255.f ));
    pointLight2.setSpecularColor(ofFloatColor(.8f, .8f, .9f));
    
    pointLight3.setDiffuseColor( ofFloatColor(19.f/255.f,94.f/255.f,77.f/255.f) );
    pointLight3.setSpecularColor( ofFloatColor(18.f/255.f,150.f/255.f,135.f/255.f) );
    
    // shininess is a value between 0 - 128, 128 being the most shiny
	material.setShininess( 120 );

    // the light highlight of the material //
	material.setSpecularColor(ofColor(255, 255, 255, 255));
    
    boxes.assign(2000, ofBoxPrimitive(2, 10, 75));
    
    for(int i = 0; i < boxes.size(); i++) {
        float spinX = sin(ofGetElapsedTimef()*.35f) + ofRandom(200.f);
        float spinY = cos(ofGetElapsedTimef()*.075f) + ofRandom(200.f);
        
        boxes[i].rotate(spinX, 1.0, 0.0, 0.0);
        boxes[i].rotate(spinY, 0, 1.0, 0.0);
    }

    
    shuffleBoxes();
    
//    cam.begin();
    
    ofEnableAlphaBlending();
    
    vector<string> words;
    words.push_back("news");
    words.push_back("today");
    words.push_back("data");
    
    // Init ofxSpeech
    recognizer.initRecognizer();
    recognizer.loadDictionary(words);
//    ofAddListener(recognizer.speechRecognizedEvent, this, &testApp::speechRecognized);
    recognizer.startListening();
    
    synthesizer.initSynthesizer("Alex");
}

//--------------------------------------------------------------
void testApp::update(){

    ofSetBackgroundColor(0, 0, 0);
    
    //swim the depth of field
//	depthOfField.setFocalDistance(ofMap(sin(ofGetElapsedTimef()/2),-1,1, 20, 150));
    depthOfField.setFocalRange(100.0);
    depthOfField.setBlurAmount(2.0);
    
    pointLight.setPosition((ofGetWidth()*.5)+ cos(ofGetElapsedTimef()*.5)*(ofGetWidth()*.3), ofGetHeight()/2, 500);
    pointLight2.setPosition((ofGetWidth()*.5)+ cos(ofGetElapsedTimef()*.15)*(ofGetWidth()*.3),
                            ofGetHeight()*.5 + sin(ofGetElapsedTimef()*.7)*(ofGetHeight()), -300);
    
    pointLight3.setPosition(
                            cos(ofGetElapsedTimef()*1.5) * ofGetWidth()*.5,
                            sin(ofGetElapsedTimef()*1.5f) * ofGetWidth()*.5,
                            cos(ofGetElapsedTimef()*.2) * ofGetWidth()
                            );
    zPosMult *= 1.004;
    zPos += zPosMult;

    camera.setPosition(ofPoint(ofGetWidth()/2, ofGetHeight()/2, zPos));
}

//--------------------------------------------------------------
void testApp::draw(){
    
    ofBackground(0, 0, 0);
    
//    post.begin();

    depthOfField.begin();
	camera.begin(depthOfField.getDimensions());
    
    ofEnableLighting();
    pointLight.enable();
    pointLight2.enable();
    pointLight3.enable();
    
//    ofBackgroundGradient(bgColorOne, bgColorTwo, OF_GRADIENT_CIRCULAR);
    
    ofFill();
    ofSetColor(200, 100, 200);
    
    material.begin();
    
    for(int i = 0; i < boxes.size(); i++) {
//        float spinX = sin(ofGetElapsedTimef()*.35f);
//        float spinY = cos(ofGetElapsedTimef()*.075f);
        
        noiseX += 0.05f;
        noiseY += 0.03f;

        float spinX, spinY;
        
        if(ofRandom(1) > 0.3) {
            spinX = sin(ofGetElapsedTimef()*.85f)*8;
            spinY = cos(ofGetElapsedTimef()*.175f)*9;
        } else if(ofRandom(1) > 0.6) {
            spinX = sin(ofGetElapsedTimef()*.85f)*10;
            spinY = cos(ofGetElapsedTimef()*.175f)*11;
        } else {
            spinX = sin(ofGetElapsedTimef()*.85f)*12;
            spinY = cos(ofGetElapsedTimef()*.175f)*13;
        }
        
        boxes[i].rotate(spinX, 1.0, 0.0, 0.0);
        boxes[i].rotate(spinY, 0, 1.0, 0.0);
        boxes[i].draw();
    }
    
    material.end();
    
    ofFill();
    ofSetColor(pointLight.getDiffuseColor());
    ofSetColor(pointLight2.getDiffuseColor());
    ofSetColor(pointLight3.getDiffuseColor());

    camera.end();
	depthOfField.end();
//    post.end();
    
	if(ofGetKeyPressed('f')){
		depthOfField.drawFocusAssist(0, 0);
	} else{
		depthOfField.getFbo().draw(0, 0);
	}
    
//	mainOutputSyphonServer.publishScreen();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
//    unsigned idx = key - '0';
//    if (idx < post.size()) post[idx]->setEnabled(!post[idx]->getEnabled());

    if(key == 'r') {
        shuffleBoxes();
//        synthesizer.speakPhrase("shuffling");
        synthesizer.speakPhrase(std::string("shuffling"));
    }
}

// ofxSpeech event handling
void testApp::speechRecognized(string & wordRecognized)
{
    if(wordRecognized == "news") {
        ofBackground(255, 0, 0);
        synthesizer.speakPhrase("here is your news.");
    }
    
    if(wordRecognized == "today") {
        ofBackground(0, 255, 0);
        synthesizer.speakPhrase("here is what your day is like.");
    }
    
    if(wordRecognized == "data") {
        ofBackground(0, 0, 255);
        synthesizer.speakPhrase("i will read out your data for today.");
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

testApp::~testApp() {
    synthesizer.stopSpeaking();
    recognizer.stopListening();
}