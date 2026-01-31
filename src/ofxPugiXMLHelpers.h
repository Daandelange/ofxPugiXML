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
//#include "glm.hpp" // of 0.11.2 and below ?
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "ofColor.h"
#include <type_traits>
#include <cstring> // std::strlen

// if defined, don't check for duplicates, speeding up execution times in large trees.
//#define ofxPugiXML_NODUPLICATES_CHECKS

using namespace pugi;


namespace ofxPugiXml {
    // Helpers to return the existing attr/node or create a new one.
    // Note: creates lots of comparisons, don't use if your store lots of data !
    inline pugi::xml_attribute getOrAppendAttribute(pugi::xml_node& _node, const char* _attrName){
#ifdef ofxPugiXML_NODUPLICATES_CHECKS
        return _node.append_attribute(_attrName);
#else
        pugi::xml_attribute attr = _node.attribute(_attrName);
        if(!attr) attr = _node.append_attribute(_attrName);
        return attr;
#endif
    }
    inline pugi::xml_node getOrAppendNode(pugi::xml_node& _parentNode, const char* _nodeName){
#ifdef ofxPugiXML_NODUPLICATES_CHECKS
        return _parentNode.append_child(_attrName);
#else
        pugi::xml_node node = _parentNode.child(_nodeName);
        if(!node) node = _parentNode.append_child(_nodeName);
        return node;
#endif
    }

    // Retrieve a variable from a node's text value (or attributes for complex data)
    // Template helper to shorten some code for retrieving values
    template<typename TYPE>
    inline bool getNodeValue(pugi::xml_node& _node, TYPE& _value);

    template<>
    inline bool getNodeValue(pugi::xml_node& _node, float& _value){
        if(!_node) return false;
        _value = _node.text().as_float(_value);
        return true;
    }
    template<>
    inline bool getNodeValue(pugi::xml_node& _node, int& _value){
        if(!_node) return false;
        _value = _node.text().as_int(_value);
        return true;
    }
    template<>
    inline bool getNodeValue(pugi::xml_node& _node, unsigned int& _value){
        if(!_node) return false;
        _value = _node.text().as_uint(_value);
        return true;
    }
    template<>
    inline bool getNodeValue(pugi::xml_node& _node, bool& _value){
        if(!_node) return false;
        _value = _node.text().as_bool(_value);
        return true;
    }
    template<>
    inline bool getNodeValue(pugi::xml_node& _node, double& _value){
        if(!_node) return false;
        _value = _node.text().as_double(_value);
        return true;
    }
    template<>
    inline bool getNodeValue(pugi::xml_node& _node, long long& _value){
        if(!_node) return false;
        _value = _node.text().as_llong(_value);
        return true;
    }
    template<>
    inline bool getNodeValue(pugi::xml_node& _node, const char*& _value){
        if(!_node) return false;
        _value = _node.text().as_string(_value);
        return true;
    }
    template<>
    inline bool getNodeValue(pugi::xml_node& _node, unsigned long long& _value){
        if(!_node) return false;
        _value = _node.text().as_ullong(_value);
        return true;
    }


    // Getter for single attribute values
    // Helps with templated code to retrieve a value from an attribute. If default is passed, writes that value as fallback, otherwise leaves the value as is.
    template<typename TYPE>
    inline bool getAttributeValue(pugi::xml_attribute& _attr, TYPE& _value, const TYPE* _defaultValue=nullptr);
    // Base type specialisations
    template<>
    inline bool getAttributeValue(pugi::xml_attribute& _attr, float& _value, const float* _defaultValue){
        if(_attr) _value = _attr.as_float(_defaultValue ? *_defaultValue : _value);
        else if(_defaultValue != nullptr) _value = *_defaultValue;
        return _attr;
    }
    template<>
    inline bool getAttributeValue(pugi::xml_attribute& _attr, int& _value, const int* _defaultValue){
        if(_attr) _value = _attr.as_int(_defaultValue ? *_defaultValue : _value);
        else if(_defaultValue != nullptr) _value = *_defaultValue;
        return _attr;
    }
    template<>
    inline bool getAttributeValue(pugi::xml_attribute& _attr, bool& _value, const bool* _defaultValue){
        if(_attr) _value = _attr.as_bool(_defaultValue ? *_defaultValue : _value);
        else if(_defaultValue != nullptr) _value = *_defaultValue;
        return _attr;
    }
    template<>
    inline bool getAttributeValue(pugi::xml_attribute& _attr, unsigned int& _value, const unsigned int* _defaultValue){
        if(_attr) _value = _attr.as_uint(_defaultValue ? *_defaultValue : _value);
        else if(_defaultValue != nullptr) _value = *_defaultValue;
        return _attr;
    }
    template<>
    inline bool getAttributeValue(pugi::xml_attribute& _attr, double& _value, const double* _defaultValue){
        if(_attr) _value = _attr.as_double(_defaultValue ? *_defaultValue : _value);
        else if(_defaultValue != nullptr) _value = *_defaultValue;
        return _attr;
    }
    template<>
    inline bool getAttributeValue(pugi::xml_attribute& _attr, std::string& _value, const std::string* _defaultValue){
        if(_attr) _value = _attr.as_string(_defaultValue ? _defaultValue->c_str() : _value.c_str());
        else if(_defaultValue != nullptr) _value = *_defaultValue;
        return _attr;
    }

    // Todo: make this private ?
    inline std::string formatAttrName(const char* _baseName, const char* _end=nullptr, const char* separator="_"){
        // Fixme: add `_` when 1st char is a number (illegal name)
        // Insert base
        std::string name = std::string(_baseName==nullptr?"":_baseName);
        // If base exists, add separator
        if( _end!=nullptr && std::strlen(_end)>0 ){
            // Only separate if base exists
            if(name.length()>0 && std::strlen(separator)>0) name.append(separator);
            name.append(_end);
        }
        // Ensure we don't return an empty string
        if(name.length()==0) name.append("value");
        return name;
    }

    // Set a variable as node attributes
    // Default template for standard types with only 1 value to store
    template<typename TYPE>
    inline bool setNodeAttribute(pugi::xml_node& _node, const char* _attributeName, const TYPE& _value);
    template<typename TYPE>
    inline bool setNodeAttribute(pugi::xml_node& _node, const char* _attributeName, const TYPE& _value){
        bool ret = true;
        if(_attributeName==nullptr || std::strlen(_attributeName)==0) _attributeName = "value"; // todo: rather assert on misusage ?
        pugi::xml_attribute attr = getOrAppendAttribute(_node, _attributeName);
        attr.set_value(_value);
        return ret;
    }
    // Custom type implementations
    template<>
    inline bool setNodeAttribute(pugi::xml_node& _node, const char* _attributeName, const glm::vec2& _value){
        bool ret = true;
        ret *= setNodeAttribute(_node, formatAttrName(_attributeName, "x").c_str(), _value.x);
        ret *= setNodeAttribute(_node, formatAttrName(_attributeName, "y").c_str(), _value.y);
        //_node.append_attribute(formatAttrName(_attribute, "x").c_str()).set_value(_value.x);
        //_node.append_attribute(formatAttrName(_attribute, "y").c_str()).set_value(_value.y);
        return ret;
    }
    template<>
    inline bool setNodeAttribute(pugi::xml_node& _node, const char* _attributeName, const glm::vec4& _value){
        bool ret = true;
        ret *= setNodeAttribute(_node, formatAttrName(_attributeName, "x").c_str(), _value.x);
        ret *= setNodeAttribute(_node, formatAttrName(_attributeName, "y").c_str(), _value.y);
        ret *= setNodeAttribute(_node, formatAttrName(_attributeName, "z").c_str(), _value.z);
        ret *= setNodeAttribute(_node, formatAttrName(_attributeName, "w").c_str(), _value.w);
        return ret;
    }
    template<>
    inline bool setNodeAttribute(pugi::xml_node& _node, const char* _attributeName, const ofFloatColor& _value){
        bool ret = true;
        ret *= setNodeAttribute(_node, formatAttrName(_attributeName, "r").c_str(), _value.r);
        ret *= setNodeAttribute(_node, formatAttrName(_attributeName, "g").c_str(), _value.g);
        ret *= setNodeAttribute(_node, formatAttrName(_attributeName, "b").c_str(), _value.b);
        ret *= setNodeAttribute(_node, formatAttrName(_attributeName, "a").c_str(), _value.a);
        return ret;
    }
    template<typename TYPE>
    inline bool setNodeAttribute(pugi::xml_node& _node, const char* _attributeName, const TYPE (&_value)[4]){
        bool ret = true;
        ret *= setNodeAttribute(_node, formatAttrName(_attributeName, "v0").c_str(), _value[0]);
        ret *= setNodeAttribute(_node, formatAttrName(_attributeName, "v1").c_str(), _value[1]);
        ret *= setNodeAttribute(_node, formatAttrName(_attributeName, "v2").c_str(), _value[2]);
        ret *= setNodeAttribute(_node, formatAttrName(_attributeName, "v3").c_str(), _value[3]);
        return ret;
    }
    template<>
    inline bool setNodeAttribute(pugi::xml_node& _node, const char* _attributeName, const std::string& _value){
        return setNodeAttribute(_node, _attributeName, _value.c_str() );
    }
    template<typename TYPE>
    inline bool setNodeAttribute(pugi::xml_node& _node, const char* _attributeName, const TYPE (&_value)[2]){
        bool ret = true;
        ret *= setNodeAttribute(_node, formatAttrName(_attributeName, "v0").c_str(), _value[0]);
        ret *= setNodeAttribute(_node, formatAttrName(_attributeName, "v1").c_str(), _value[1]);
        return ret;
    }

    // Retrieve a variable from node attributes
    // Base template (for base types, have to be specialised)
    template<typename TYPE>
    inline bool getNodeAttributeValue(pugi::xml_node& _node, const char* _attributeName, TYPE& _value, const TYPE* _defaultValue=nullptr);

    // Custom type implementations
    template<>
    inline bool getNodeAttributeValue(pugi::xml_node& _node, const char* _attributeName, glm::vec2& _value, const glm::vec2* _defaultValue){
        bool ret = true;
        ret *= getNodeAttributeValue<float>(_node, formatAttrName(_attributeName, "x").c_str(), _value.x,  _defaultValue ? &_defaultValue->x : &_value.x);
        ret *= getNodeAttributeValue<float>(_node, formatAttrName(_attributeName, "y").c_str(), _value.y,  _defaultValue ? &_defaultValue->y : &_value.y);
        return ret;
    }
    template<>
    inline bool getNodeAttributeValue(pugi::xml_node& _node, const char* _attributeName, glm::vec4& _value, const glm::vec4* _defaultValue){
        bool ret = true;
        ret *= getNodeAttributeValue<float>(_node, formatAttrName(_attributeName, "x").c_str(), _value.x, _defaultValue ? &_defaultValue->x : &_value.x);
        ret *= getNodeAttributeValue<float>(_node, formatAttrName(_attributeName, "y").c_str(), _value.y, _defaultValue ? &_defaultValue->y : &_value.y);
        ret *= getNodeAttributeValue<float>(_node, formatAttrName(_attributeName, "z").c_str(), _value.z, _defaultValue ? &_defaultValue->z : &_value.z);
        ret *= getNodeAttributeValue<float>(_node, formatAttrName(_attributeName, "w").c_str(), _value.w, _defaultValue ? &_defaultValue->w : &_value.w);
        return ret;
    }
    template<>
    inline bool getNodeAttributeValue(pugi::xml_node& _node, const char* _attributeName, ofFloatColor& _value, const ofFloatColor* _defaultValue){
        bool ret = true;
        ret *= getNodeAttributeValue<float>(_node, formatAttrName(_attributeName, "r").c_str(), _value.r, _defaultValue ? &_defaultValue->r : &_value.r);
        ret *= getNodeAttributeValue<float>(_node, formatAttrName(_attributeName, "g").c_str(), _value.g, _defaultValue ? &_defaultValue->g : &_value.g);
        ret *= getNodeAttributeValue<float>(_node, formatAttrName(_attributeName, "b").c_str(), _value.b, _defaultValue ? &_defaultValue->b : &_value.b);
        ret *= getNodeAttributeValue<float>(_node, formatAttrName(_attributeName, "a").c_str(), _value.a, _defaultValue ? &_defaultValue->a : &_value.a);
        return ret;
    }

    // Default pure types
    template<typename TYPE>
    bool getNodeAttributeValue(pugi::xml_node& _node, const char* _attributeName, TYPE& _value, const TYPE* _defaultValue){
        if(pugi::xml_attribute attr = _node.attribute(_attributeName)){
            return getAttributeValue<TYPE>(attr, _value, _defaultValue);
        }
        return false;
    }

    template<typename TYPE>
    inline bool getNodeAttributeValue(pugi::xml_node& _node, const char* _attributeName, TYPE (&_value)[4], const TYPE (*_defaultValue)[4] ){
        bool ret = true;
        ret *= getNodeAttributeValue<TYPE>(_node, formatAttrName(_attributeName, "v0").c_str(), _value[0], _defaultValue ? &(*_defaultValue)[0] : &_value[0]);
        ret *= getNodeAttributeValue<TYPE>(_node, formatAttrName(_attributeName, "v1").c_str(), _value[1], _defaultValue ? &(*_defaultValue)[1] : &_value[1]);
        ret *= getNodeAttributeValue<TYPE>(_node, formatAttrName(_attributeName, "v2").c_str(), _value[2], _defaultValue ? &(*_defaultValue)[2] : &_value[2]);
        ret *= getNodeAttributeValue<TYPE>(_node, formatAttrName(_attributeName, "v3").c_str(), _value[3], _defaultValue ? &(*_defaultValue)[3] : &_value[3]);
        return ret;
    }
    template<typename TYPE>
    inline bool getNodeAttributeValue(pugi::xml_node& _node, const char* _attributeName, TYPE (&_value)[2], const TYPE (*_defaultValue)[2] ){
        bool ret = true;
        ret *= getNodeAttributeValue<TYPE>(_node, formatAttrName(_attributeName, "v0").c_str(), _value[0], _defaultValue ? &(*_defaultValue)[0] : &_value[0]);
        ret *= getNodeAttributeValue<TYPE>(_node, formatAttrName(_attributeName, "v1").c_str(), _value[1], _defaultValue ? &(*_defaultValue)[1] : &_value[1]);
        return ret;
    }

    // Sadly, these have to be hardcoded here. (uncomplete list)
    // The inline compiler can't inline them (templated *[2] & *[4] specialisations above) when dynamically instantiated from usercode.
    // Inline functions can't be explicitly instantiated... neither `extern`alised
    template<> inline bool getNodeAttributeValue<int[2]>(pugi::xml_node& _node, const char* _attributeName, int (&_value)[2], const int (*_defaultValue)[2]){
        return getNodeAttributeValue(_node, _attributeName, _value, _defaultValue);
    }
    template<> inline bool getNodeAttributeValue<float[2]>(pugi::xml_node& _node, const char* _attributeName, float (&_value)[2], const float (*_defaultValue)[2]){
        return getNodeAttributeValue(_node, _attributeName, _value, _defaultValue);
    }
    template<> inline bool getNodeAttributeValue<int[4]>(pugi::xml_node& _node, const char* _attributeName, int (&_value)[4], const int (*_defaultValue)[4]){
        return getNodeAttributeValue(_node, _attributeName, _value, _defaultValue);
    }

    // Allow binding hardcoded default values
    template<typename TYPE>
    inline bool getNodeAttributeValue(pugi::xml_node& _node, const char* _attributeName, TYPE& _value, const TYPE _defaultValue){
        return getNodeAttributeValue<TYPE>(_node, _attributeName, _value, &_defaultValue);
    }


    // Functions below might depreciate soon !
    // Stores a vector in a node, returns the created node
    template<typename TYPE>
    inline xml_node setNodeValueToAttribute(xml_node& _parent, const char* _childName, const TYPE& _value, const char* _attrName=""){
        xml_node tNode = getOrAppendNode(_parent, _childName);//_parent.append_child(_childName);
        setNodeAttribute(tNode, _attrName, _value);
        return tNode;
    }

    template<typename TYPE>
    inline bool getNodeValueFromAttribute(xml_node& _parent, const char* _childName, TYPE& _value, const char* _attrName=""){
        if(pugi::xml_node tNode = _parent.child(_childName)){
            return getNodeAttributeValue(tNode, _attrName, _value);
        }
        return false;
    }

    template<typename TYPE>
    bool getNodeValue(xml_node& _parent, const char* _childName, TYPE& _value){
        if(pugi::xml_node tNode = _parent.child(_childName)){
            getNodeValue(tNode, _value);
            return true;
        }
        return false;
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
            return glm::floor((PUGIXML_VERSION % 100)/10);
        }
        return glm::floor((PUGIXML_VERSION % 1000)/10);
    }
    inline int versionPatch(){
        return glm::floor((PUGIXML_VERSION % 10));
    }

} // namespace ofxPugiXml
