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


#pragma once

#include "pugixml.hpp"

#include "ofMain.h"


// A compatibility layer for ofxXmlSettings (which uses libTinyXML)
// So you can easily replace `ofxXmlSettings mySettings` by `ofxPugiXmlSettings mySettings` to stitch your XML engine.
// Implementation by d3cod3

class ofxPugiXmlSettings {

public:

    ofxPugiXmlSettings();
    ofxPugiXmlSettings(const std::string& xmlFile);

    ~ofxPugiXmlSettings();


    pugi::xml_parse_result loadFile(const std::string& xmlFile);

    bool saveFile(const std::string& xmlFile);

    bool saveFile();

    pugi::xml_parse_result load(const std::string & path);

    bool save(const std::string & path);

    void removeTag(const std::string& tag, int which = 0);

    int getValue(const std::string& tag, int defaultValue, int which = 0) const;

    double getValue(const std::string& tag, double defaultValue, int which = 0) const;

    std::string getValue(const std::string& tag, const string& defaultValue, int which = 0) const;

    void setValue(const std::string& tag, int value);
    void setValue(const std::string& tag, double value);
    void setValue(const std::string& tag, const std::string& value);

    //advanced

    //-- pushTag/popTag
    //pushing a tag moves you inside it which has the effect of
    //temporarily treating the tag you are in as the document root
    //all setValue, readValue and getValue commands are then be relative to the tag you pushed.
    //this can be used with addValue to create multiple tags of the same name within
    //the pushed tag - normally addValue only lets you create multiple tags of the same
    //at the top most level.

    bool pushTag(const std::string& tag, int which = 0);
    void popTag();

    int getNumTags(const std::string& tag) const;

    //adds an empty tag at the current level
    void addTag(const std::string& tag);

    // Attribute-related methods
    void addAttribute(const std::string& tag, const std::string& attribute, int value);

    void addAttribute(const std::string& tag, const std::string& attribute, double value);

    void addAttribute(const std::string& tag, const std::string& attribute, const std::string& value);

    void removeAttribute(const std::string& tag, const std::string& attribute);

    int getNumAttributes(const std::string& tag, int which = 0) const;

    int getAttribute(const std::string& tag, const std::string& attribute, int defaultValue=0) const;

    double getAttribute(const std::string& tag, const std::string& attribute, double defaultValue=0) const;

    std::string getAttribute(const std::string& tag, const std::string& attribute, const std::string& defaultValue="") const;

    void setAttribute(const std::string& tag, const std::string& attribute, int value);

    void setAttribute(const std::string& tag, const std::string& attribute, double value);

    void setAttribute(const std::string& tag, const std::string& attribute, const std::string& value);

    pugi::xml_parse_result isFileLoaded;
    std::string filepath;

protected:

    pugi::xml_document doc;
    pugi::xml_node currentNode;

};
