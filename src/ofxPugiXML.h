// =============================================================================
//
// Copyright (c) 2014 Christopher Baker <http://christopherbaker.net>
// Copyright (c) 2020-2024 Daan de Lange <https://daandelange.com>
// Copyright (c) 2020 Emanuele Mazza aka n3m3da <https://d3cod3.org>
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

// Before of 0.9.0 PugiXML was not included !
// OpenFrameworks uses pugiXml for ofxXML since 0.9.0.

// Clarify where pugiXML comes from.
#if OF_VERSION_MAJOR <= 8
#pragma message "You're using OpenFrameworks < 0.9.0, you need to comment out the OF9 line in `ofxPugiXML/addon_config.mk`."
#else
#pragma message "You're using Openframeworks > 0.9.0, using PugiXML contained within !"
#endif

// Todo : How could we include a more recent version of pugiXML than the one shipped within OpenFrameworks ?
// OpenFrameworks 0.9.0 comes without PugiXML
// OpenFrameworks 0.9.0 comes with PugiXML 1.7.0
// OpenFrameworks 0.11.0 comes with PugiXML 1.9.0
// OpenFrameworks 0.12.0 comes with PugiXML 1.9.0

#include "pugixml.hpp"

// Also include our custom OF glue !
#include "ofxPugiXMLHelpers.h"
#include "ofxPugiXMLSettings.h"
