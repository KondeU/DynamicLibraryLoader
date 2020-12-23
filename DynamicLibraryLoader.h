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

#ifndef _DYNAMIC_LIBRARY_LOADER_H_
#define _DYNAMIC_LIBRARY_LOADER_H_

#include <string>
#include <functional>
#include <unordered_map>

#ifdef _WIN32
#include <Windows.h>
#define LIB_LIBRARY          HMODULE
#define LIB_PROCESS          FARPROC
#define LIB_INVALID_LIBRARY  NULL
#define LIB_INVALID_PROCESS  NULL
#define LIB_LOAD(file)       LoadLibraryA(file)
#define LIB_UNLD(inst)       !FreeLibrary(inst)
#define LIB_SYMB(inst, func) GetProcAddress(inst, func)
#define LIB_NAME             "dll"
#else
#include <dlfcn.h>
#define LIB_LIBRARY          void*
#define LIB_PROCESS          void*
#define LIB_INVALID_LIBRARY  nullptr
#define LIB_INVALID_PROCESS  nullptr
#define LIB_LOAD(file)       dlopen(file, RTLD_LAZY)
#define LIB_UNLD(inst)       dlclose(inst)
#define LIB_SYMB(inst, func) dlsym(inst, func)
#define LIB_NAME             "so"
#endif

class DynamicLibraryLoader final {
public:
    DynamicLibraryLoader();
    ~DynamicLibraryLoader();

    bool Load(const std::string& file);
    bool UnLoad();

    template <typename FuncAddr>
    FuncAddr* GetFunction(const std::string& function)
    {
        LIB_PROCESS addr = LIB_INVALID_PROCESS;
        auto it = cache.find(function);
        if (it == cache.end()) {
            LIB_PROCESS proc = LIB_SYMB(library, function.c_str());
            it = cache.emplace(function, proc).first;
        }
        addr = it->second;
        return reinterpret_cast<FuncAddr*>(addr);
    }

    template <typename Func, typename ...Args>
    typename std::result_of<std::function<Func>(Args...)>::type
        ExecuteFunction(const std::string& func, Args&& ...args)
    {
        auto fn = GetFunction<Func>(func);
        if (fn == nullptr) {
            std::string except = "Cannot find function: " + func;
            throw std::exception(except.c_str());
        }
        return fn(std::forward<Args>(args)...);
    }

private:
    LIB_LIBRARY library;
    std::unordered_map<std::string, LIB_PROCESS> cache;
};

#endif
