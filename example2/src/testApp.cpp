// This is the ofxLibdc example tweaked by Elliot Woods to display a Firefly MV FMVU-13S2CS-CS - 29-06-12
// Your help much appreciated Elliot ;-)
// Requires the alternate libdc1394.a and libusb-1.0.a files recompiled for USB support and 32-bit to be placed in the ofxLibdc > libs > osx > lib folder.
// Delete or archive the existing libdc1394.a file...

#include "testApp.h"

void testApp::setup() {
    
	ofSetVerticalSync(true);
    ofSetLogLevel(OF_LOG_VERBOSE); // so all available data is displayed in console
    
	// If you want to set any non-default parameters like size, format7, blocking
	// capture, etc., you can do it here before setup. They'll be applied to the
	// camera during setup().
	
	// setup() will pick the first camera. Or if there are multiple cameras you
	// can use the number, or the GUID of the camera.
	
    camera.setBayerMode(DC1394_COLOR_FILTER_GBRG);
    camera.setPosition(0,0);
    //camera.setSize(536,964);
    camera.setSize(640,480);
    camera.setFormat7(true, 0);
    camera.setup();
	
	// After setup it's still possible to change a lot of parameters. If you want
	// to change a pre-setup parameter, the camera will auto-restart
	
	// Set the window size to the camera size.
	ofSetWindowShape(camera.getWidth(), camera.getHeight());
    
    rawImage.allocate(camera.getWidth(), camera.getHeight());
    colorImage.allocate(camera.getWidth(), camera.getHeight());
    processed.allocate(camera.getWidth(), camera.getHeight(), OF_IMAGE_COLOR);
}

void testApp::update() {
	// grabVideo() will place the most recent frame into curFrame. If it's a new
	// frame, grabFrame returns true. If there are multiple frames available, it
	// will drop old frames and only give you the newest. This guarantees the
	// lowest latency. If you prefer to not drop frames, set the second argument
	// (dropFrames) to false. By default, capture is non-blocking.
    
    
	if(camera.grabVideo(curFrame)) {
		curFrame.update();
        rawImage.setFromPixels(curFrame.getPixels(), camera.getWidth(), camera.getHeight());
        cvCvtColor(rawImage.getCvImage(), colorImage.getCvImage(), CV_BayerBG2RGB); // this seems to best match the colour mode of the camera
        
        // ofxLibdc doesn't seem to capture from this camera in colour mode - so Elliot used ofxOpenCV to extract the colour data from the raw image..
        // this is a cludge to adjust the blue level...
        processed.setFromPixels(colorImage.getPixelsRef());
        
        unsigned char * blue = processed.getPixels() + 2;
        for (int i=0; i<processed.getWidth()*processed.getHeight(); i++) {
            *blue = pow((float) *blue / 255.0f, 0.9f)  * 255.0f;
            blue += 3;
        }
        processed.update();
	}
}

void testApp::draw() {
	// If the camera isn't ready, the curFrame will be empty.
	if(camera.isReady()) {
		// Camera doesn't draw itself, curFrame does.
		curFrame.draw(0, 0, ofGetWidth(), ofGetHeight());
        //processed.draw(ofGetWidth()/2, 0, ofGetWidth()/2, ofGetHeight());
	}
}
