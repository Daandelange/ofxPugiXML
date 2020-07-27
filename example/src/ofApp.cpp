// =============================================================================
//
// Copyright (c) 2014 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


#include "ofApp.h"


void ofApp::setup()
{
    /*pugi::xml_document doc;

    std::string buffer = ofBufferFromFile("message.xml").getText();

    pugi::xml_parse_result result = doc.load_string(buffer.c_str());

    if (result)
    {
        pugi::xml_node messageNode = doc.child("message");

        ofLogNotice("ofApp::setup") << "     To: " << messageNode.child("to").text().as_string();
        ofLogNotice("ofApp::setup") << "   From: " << messageNode.child("from").text().as_string();
        ofLogNotice("ofApp::setup") << "Subject: " << messageNode.child("subject").text().as_string();
        ofLogNotice("ofApp::setup") << "   Body: " << messageNode.child("body").text().as_string();
    }
    else
    {
        ofLogError("ofApp::setup") << "XML [" << buffer << "] parsed with errors.";
        ofLogError("ofApp::setup") << "Error description: " << result.description();
        ofLogError("ofApp::setup") << "Error offset: " << result.offset << " (error at [..." << (result.offset) << "]";
        ofLogNotice("ofApp::setup") << "   Body: " << result.description();
    }*/

    XML.load("message.xml");

    if(XML.isFileLoaded){
        if(XML.pushTag("message")){

            ofLogNotice("ofApp::setup") << "     To: " << XML.getValue("to","");
            ofLogNotice("ofApp::setup") << "   From: " << XML.getValue("from","");
            ofLogNotice("ofApp::setup") << "Subject: " << XML.getValue("subject","");
            ofLogNotice("ofApp::setup") << "   Body: " << XML.getValue("body","");

            XML.popTag();
        }

    }else{
        std::string buffer = ofBufferFromFile(XML.filepath).getText();
        ofLogError("ofApp::setup") << "XML [" << buffer << "] parsed with errors.";
        ofLogError("ofApp::setup") << "Error description: " << XML.isFileLoaded.description();
        ofLogError("ofApp::setup") << "Error offset: " << XML.isFileLoaded.offset << " (error at [..." << (XML.isFileLoaded.offset) << "]";
        ofLogNotice("ofApp::setup") << "   Body: " << XML.isFileLoaded.description();
    }
}


void ofApp::draw()
{
    ofBackgroundGradient(ofColor::white, ofColor::black);

    ofDrawBitmapStringHighlight("See console for output.", ofPoint(30, 30));

}

