// =============================================================================
//
// Copyright (c) 2020 Emanuele Mazza aka n3m3da <https://d3cod3.org>
// Copyright (c) 2024 Daan de Lange <https://daandelange.com>
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


#include "ofxPugiXMLSettings.h"


ofxPugiXmlSettings::ofxPugiXmlSettings() {

}

ofxPugiXmlSettings::ofxPugiXmlSettings(const std::string& xmlFile) {
    this->loadFile(xmlFile.c_str());
}

ofxPugiXmlSettings::~ofxPugiXmlSettings() {

}

pugi::xml_parse_result ofxPugiXmlSettings::loadFile(const std::string& xmlFile){
    this->filepath = xmlFile;

    std::string buffer = ofBufferFromFile(xmlFile).getText();

    this->isFileLoaded = doc.load_string(buffer.c_str());

    if(this->isFileLoaded){
        this->currentNode = this->doc.root();
    }

    return this->isFileLoaded;
}

bool ofxPugiXmlSettings::saveFile(const std::string& xmlFile){
    return this->doc.save_file(xmlFile.c_str());
}

bool ofxPugiXmlSettings::saveFile(){
    return this->doc.save_file(this->filepath.c_str());
}

pugi::xml_parse_result ofxPugiXmlSettings::load(const std::string & path) {
    return loadFile(path);
}

bool ofxPugiXmlSettings::save(const std::string & path) {
    return saveFile(path);
}


void ofxPugiXmlSettings::removeTag(const std::string& tag, int which){
    int counter = 0;

    if(which < 0) which = 0;

    for (pugi::xml_node currentTag : this->currentNode.children(tag.c_str())){
        if(which == counter){
            this->currentNode.remove_child(currentTag);
            break;
        }
        counter++;
    }
}

int ofxPugiXmlSettings::getValue(const std::string& tag, int defaultValue, int which) const {
    int counter = 0;

    if(which < 0) which = 0;

    for (pugi::xml_node currentTag: this->currentNode.children(tag.c_str())){
        if(which == counter){
            return currentTag.text().as_int();
        }
        counter++;
    }

    return 0;
}

double ofxPugiXmlSettings::getValue(const std::string&tag, double defaultValue, int which) const{
    int counter = 0;

    if(which < 0) which = 0;

    for (pugi::xml_node currentTag: this->currentNode.children(tag.c_str())){
        if(which == counter){
            return currentTag.text().as_double();
        }
        counter++;
    }

    return 0;
}

std::string ofxPugiXmlSettings::getValue(const std::string& tag, const string& defaultValue, int which) const{
    int counter = 0;

    if(which < 0) which = 0;

    for (pugi::xml_node currentTag: this->currentNode.children(tag.c_str())){
        if(which == counter){
            return currentTag.text().as_string();
        }
        counter++;
    }

    return "";
}

void ofxPugiXmlSettings::setValue(const std::string& tag, int value){
    pugi::xml_node checkNode = this->currentNode.child(tag.c_str());
    if(!checkNode){
        pugi::xml_node newNode = this->currentNode.append_child(tag.c_str());
        newNode.set_value(ofToString(value).c_str());
    }else{
        this->currentNode.child(tag.c_str()).set_value(ofToString(value).c_str());
    }
}
void ofxPugiXmlSettings::setValue(const std::string& tag, double value){
    pugi::xml_node checkNode = this->currentNode.child(tag.c_str());
    if(!checkNode){
        pugi::xml_node newNode = this->currentNode.append_child(tag.c_str());
        newNode.set_value(ofToString(value).c_str());
    }else{
        this->currentNode.child(tag.c_str()).set_value(ofToString(value).c_str());
    }
}
void ofxPugiXmlSettings::setValue(const std::string& tag, const std::string& value){
    pugi::xml_node checkNode = this->currentNode.child(tag.c_str());
    if(!checkNode){
        pugi::xml_node newNode = this->currentNode.append_child(tag.c_str());
        newNode.set_value(value.c_str());
    }else{
        this->currentNode.child(tag.c_str()).set_value(value.c_str());
    }
}

//advanced

//-- pushTag/popTag
//pushing a tag moves you inside it which has the effect of
//temporarily treating the tag you are in as the document root
//all setValue, readValue and getValue commands are then be relative to the tag you pushed.
//this can be used with addValue to create multiple tags of the same name within
//the pushed tag - normally addValue only lets you create multiple tags of the same
//at the top most level.

bool ofxPugiXmlSettings::pushTag(const std::string& tag, int which){
    bool found = false;

    int counter = 0;
    if(which < 0) which = 0;

    for (pugi::xml_node currentTag: this->currentNode.children(tag.c_str())){
        if(counter == which){
            this->currentNode = currentTag;
            found = true;
            break;
        }
        counter++;
    }

    return found;
}
void ofxPugiXmlSettings::popTag(){
    pugi::xml_node parentTag = this->currentNode.parent();
    this->currentNode = parentTag;
}

int ofxPugiXmlSettings::getNumTags(const std::string& tag) const{
    int counter = 0;
    for (pugi::xml_node currentTag: this->currentNode.children(tag.c_str())){
        counter++;
    }
    return counter;
}

//adds an empty tag at the current level
void ofxPugiXmlSettings::addTag(const std::string& tag){
    this->currentNode.append_child(tag.c_str());
}

// Attribute-related methods
void ofxPugiXmlSettings::addAttribute(const std::string& tag, const std::string& attribute, int value){
    this->currentNode.child(tag.c_str()).append_attribute(attribute.c_str()) = ofToString(value).c_str();
}

void ofxPugiXmlSettings::addAttribute(const std::string& tag, const std::string& attribute, double value){
    this->currentNode.child(tag.c_str()).append_attribute(attribute.c_str()) = ofToString(value).c_str();
}

void ofxPugiXmlSettings::addAttribute(const std::string& tag, const std::string& attribute, const std::string& value){
    this->currentNode.child(tag.c_str()).append_attribute(attribute.c_str()) = value.c_str();
}

void ofxPugiXmlSettings::removeAttribute(const std::string& tag, const std::string& attribute){
    this->currentNode.child(tag.c_str()).remove_attribute(attribute.c_str());
}

int ofxPugiXmlSettings::getNumAttributes(const std::string& tag, int which) const{
    int counter = 0;
    int numAttributes = 0;

    if(which < 0) which = 0;

    for (pugi::xml_node currentTag: this->currentNode.children(tag.c_str())){
        if(which == counter){
            for(pugi::xml_attribute currentAttr: currentTag.attributes()){
                numAttributes++;
            }
            break;
        }
        counter++;
    }

    return numAttributes;
}

int ofxPugiXmlSettings::getAttribute(const std::string& tag, const std::string& attribute, int defaultValue) const{
    return this->currentNode.child(tag.c_str()).attribute(attribute.c_str()).as_int();
}

double ofxPugiXmlSettings::getAttribute(const std::string& tag, const std::string& attribute, double defaultValue) const{
    return this->currentNode.child(tag.c_str()).attribute(attribute.c_str()).as_double();
}

std::string ofxPugiXmlSettings::getAttribute(const std::string& tag, const std::string& attribute, const std::string& defaultValue) const{
    return this->currentNode.child(tag.c_str()).attribute(attribute.c_str()).as_string();
}

void ofxPugiXmlSettings::setAttribute(const std::string& tag, const std::string& attribute, int value){
    this->currentNode.child(tag.c_str()).attribute(attribute.c_str()) = ofToString(value).c_str();
}

void ofxPugiXmlSettings::setAttribute(const std::string& tag, const std::string& attribute, double value){
    this->currentNode.child(tag.c_str()).attribute(attribute.c_str()) = ofToString(value).c_str();
}

void ofxPugiXmlSettings::setAttribute(const std::string& tag, const std::string& attribute, const std::string& value){
    this->currentNode.child(tag.c_str()).attribute(attribute.c_str()) = value.c_str();
}
