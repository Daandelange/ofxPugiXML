// =============================================================================
//
// Copyright (c) 2023-2024 Daan de Lange <https://daandelange.com>
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

// GLUE
// This header provides bindings for easier integration with OF types
// To be used optionally with pugixml.hpp. Maybe use always with ofxPugiXML.h ?

#pragma once

//#include "ofMain.h"
#include "pugixml.hpp"
#include "glm.hpp"
#include "ofColor.h"


using namespace pugi;

// Stores a vector in a node
// Returns the created node
inline xml_node ofxPugiXmlAppendVec2Node(xml_node& _parent, const glm::vec2& _vec, const char* _name="pos"){
    xml_node vecNode =_parent.append_child(_name);
    vecNode.append_attribute("x").set_value(_vec.x);
    vecNode.append_attribute("y").set_value(_vec.y);
    return vecNode;
}

// Adds the position to the node as an attribute
// Returns the modified node
inline xml_node ofxPugiXmlAppendVec2Attr(xml_node& _node, const glm::vec2& _vec, const char* _nameX="pos-x", const char* _nameY="pos-y"){
    _node.append_attribute(_nameX).set_value(_vec.x);
    _node.append_attribute(_nameY).set_value(_vec.y);
    return _node;
}

inline glm::vec2 ofxPugiXmlGetVec2FromNodeAttr(xml_node& _node, const char* _xName="x", const char* _yName="y" ){
    return glm::vec2(
        _node.attribute(_xName).as_float(),
        _node.attribute(_yName).as_float()
    );
}

inline xml_node ofxPugiXmlAppendOfColor(xml_node& _parent, const ofFloatColor& _col, const char* _name){
    xml_node colNode =_parent.append_child(_name);
    colNode.append_attribute("r").set_value(_col.r);
    colNode.append_attribute("g").set_value(_col.g);
    colNode.append_attribute("b").set_value(_col.b);
    colNode.append_attribute("a").set_value(_col.a);
    return colNode;
}

inline bool ofxPugiXmlRetrieveOfColor(xml_node& _parent, ofFloatColor& _col, const char* _childName){
    if(pugi::xml_node _cNode = _parent.child(_childName)){
        _col.r = _cNode.attribute("r").as_float();
        _col.g = _cNode.attribute("g").as_float();
        _col.b = _cNode.attribute("b").as_float();
        _col.a = _cNode.attribute("a").as_float();
        return true;
    }
    return false;
}

namespace ofxPugiXml {
    // Get Node value
    // Template helper to shorten some code for retrieving values
    template<typename TYPE>
    inline TYPE getNodeValue(pugi::xml_node& _node);
    template<>
    inline float getNodeValue(pugi::xml_node& _node){
        return _node.text().as_float();
    }
    template<>
    inline int getNodeValue(pugi::xml_node& _node){
        return _node.text().as_int();
    }
    template<>
    inline unsigned int getNodeValue(pugi::xml_node& _node){
        return _node.text().as_uint();
    }
    template<>
    inline bool getNodeValue(pugi::xml_node& _node){
        return _node.text().as_bool();
    }
    template<>
    inline double getNodeValue(pugi::xml_node& _node){
        return _node.text().as_double();
    }
    template<>
    inline long long getNodeValue(pugi::xml_node& _node){
        return _node.text().as_llong();
    }
    template<>
    inline const char* getNodeValue(pugi::xml_node& _node){
        return _node.text().as_string();
    }
    template<>
    inline unsigned long long getNodeValue(pugi::xml_node& _node){
        return _node.text().as_ullong();
    }
    template<>
    inline ofFloatColor getNodeValue(pugi::xml_node& _node){
        ofFloatColor col;
        if(_node){
            col.r = _node.attribute("r").as_float();
            col.g = _node.attribute("g").as_float();
            col.b = _node.attribute("b").as_float();
            col.a = _node.attribute("a").as_float();
        }
        return col;
    }
    template<>
    inline glm::vec2 getNodeValue(pugi::xml_node& _node){
        glm::vec2 vec;
        if(_node){
            vec.x = _node.attribute("x").as_float();
            vec.y = _node.attribute("y").as_float();
        }
        return vec;
    }


    // Getter for attribute values
    template<typename TYPE>
    inline TYPE getAttributeValue(pugi::xml_attribute& _attr);
    // Base type specialisations
    template<>
    inline float getAttributeValue(pugi::xml_attribute& _attr){
        return _attr.as_float();
    }
    template<>
    inline int getAttributeValue(pugi::xml_attribute& _attr){
        return _attr.as_int();
    }
    template<>
    inline bool getAttributeValue(pugi::xml_attribute& _attr){
        return _attr.as_bool();
    }
    template<>
    inline unsigned int getAttributeValue(pugi::xml_attribute& _attr){
        return _attr.as_uint();
    }
    template<>
    inline double getAttributeValue(pugi::xml_attribute& _attr){
        return _attr.as_double();
    }

    // Works for base types
    template<typename TYPE>
    inline void setNodeAttribute(pugi::xml_node& _node, const char* _attribute, const TYPE& _value){
        _node.append_attribute(_attribute).set_value(_value);
    }
    // Custom type implementations
    template<>
    inline void setNodeAttribute(pugi::xml_node& _node, const char* _attribute, const glm::vec2& _value){
        _node.append_attribute(std::string(_attribute).append("_x").c_str()).set_value(_value.x);
        _node.append_attribute(std::string(_attribute).append("_y").c_str()).set_value(_value.y);
    }
    template<>
    inline void setNodeAttribute(pugi::xml_node& _node, const char* _attribute, const glm::vec4& _value){
        _node.append_attribute(std::string(_attribute).append("_x").c_str()).set_value(_value.x);
        _node.append_attribute(std::string(_attribute).append("_y").c_str()).set_value(_value.y);
        _node.append_attribute(std::string(_attribute).append("_z").c_str()).set_value(_value.z);
        _node.append_attribute(std::string(_attribute).append("_w").c_str()).set_value(_value.w);
    }
    template<>
    inline void setNodeAttribute(pugi::xml_node& _node, const char* _attribute, const ofFloatColor& _value){
        _node.append_attribute(std::string(_attribute).append("_r").c_str()).set_value(_value.r);
        _node.append_attribute(std::string(_attribute).append("_g").c_str()).set_value(_value.g);
        _node.append_attribute(std::string(_attribute).append("_b").c_str()).set_value(_value.b);
        _node.append_attribute(std::string(_attribute).append("_a").c_str()).set_value(_value.a);
    }

    // Getter
    // Base template (for base types, have to be specialised)
    template<typename TYPE>
    inline bool getNodeAttribute(pugi::xml_node& _node, const char* _attribute, TYPE& _value){
        if(pugi::xml_attribute attr = _node.attribute(_attribute)){
            _value = getAttributeValue<TYPE>(attr);
            return true;
        }
        return false;
    }
    // Custom type implementations
    template<>
    inline bool getNodeAttribute(pugi::xml_node& _node, const char* _attribute, glm::vec2& _value){
        bool ret = true;
        ret *= getNodeAttribute<float>(_node, std::string(_attribute).append("_x").c_str(), _value.x);
        ret *= getNodeAttribute<float>(_node, std::string(_attribute).append("_y").c_str(), _value.y);
        return true;
    }
    template<>
    inline bool getNodeAttribute(pugi::xml_node& _node, const char* _attribute, glm::vec4& _value){
        bool ret = true;
        ret *= getNodeAttribute<float>(_node, std::string(_attribute).append("_x").c_str(), _value.x);
        ret *= getNodeAttribute<float>(_node, std::string(_attribute).append("_y").c_str(), _value.y);
        ret *= getNodeAttribute<float>(_node, std::string(_attribute).append("_z").c_str(), _value.z);
        ret *= getNodeAttribute<float>(_node, std::string(_attribute).append("_w").c_str(), _value.w);
        return ret;
    }
    template<>
    inline bool getNodeAttribute(pugi::xml_node& _node, const char* _attribute, ofFloatColor& _value){
        bool ret = true;
        ret *= getNodeAttribute<float>(_node, std::string(_attribute).append("_r").c_str(), _value.r);
        ret *= getNodeAttribute<float>(_node, std::string(_attribute).append("_g").c_str(), _value.g);
        ret *= getNodeAttribute<float>(_node, std::string(_attribute).append("_b").c_str(), _value.b);
        ret *= getNodeAttribute<float>(_node, std::string(_attribute).append("_a").c_str(), _value.a);
        return ret;
    }

    // Version getters
    // Define version macro; evaluates to major * 1000 + minor * 10 + patch so that it's safe to use in less-than comparisons
    // Note: pugixml used major * 100 + minor * 10 + patch format up until 1.9 (which had version identifier 190); starting from pugixml 1.10, the minor version number is two digits
    inline int versionMajor(){
        if(PUGIXML_VERSION<=190){
            return glm::floor(PUGIXML_VERSION/100);
        }
        return glm::floor(PUGIXML_VERSION/1000);
    }
    inline int versionMinor(){
        if(PUGIXML_VERSION<=190){
            return glm::floor(glm::mod(PUGIXML_VERSION, 100)/10);
        }
        return glm::floor(glm::mod(PUGIXML_VERSION, 1000)/10);
    }
    inline int versionPatch(){
        return glm::floor(glm::mod(PUGIXML_VERSION, 10));
    }
}
