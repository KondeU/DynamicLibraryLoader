////////////////////////////////////////////////////////////////////////////////
//
// MIT License
//
// Copyright (c) 2020 kongdeyou(https://tis.ac.cn/blog/kongdeyou/)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
////////////////////////////////////////////////////////////////////////////////

#include "DynamicLibraryLoader.h"

DynamicLibraryLoader::DynamicLibraryLoader()
{
    library = LIB_INVALID_LIBRARY;
}

DynamicLibraryLoader::~DynamicLibraryLoader()
{
    UnLoad();
}

bool DynamicLibraryLoader::Load(const std::string& file)
{
    if (library != LIB_INVALID_LIBRARY) {
        return false;
    }

    library = LIB_LOAD(file.c_str());
    if (library == LIB_INVALID_LIBRARY) {
        return false;
    }
    return true;
}

bool DynamicLibraryLoader::UnLoad()
{
    if (library == LIB_INVALID_LIBRARY) {
        return true;
    }

    cache.clear();

    if (LIB_UNLD(library)) {
        return false;
    }
    library = LIB_INVALID_LIBRARY;
    return true;
}
