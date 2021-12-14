#pragma once

#include <string>

#ifdef _WIN32
#include <windows.h>
#define LIBLOADER_LIBRARY_HANDLE HINSTANCE
#define LIBLOADER_DEFAULT_LIBRARY_FILE_TYPE ".dll"
#endif
#ifdef __unix__
#include <dlfcn.h>
#define LIBLOADER_LIBRARY_HANDLE void*
#define LIBLOADER_DEFAULT_LIBRARY_FILE_TYPE ".so"
#endif

namespace libloader {
    class library {
    public:
        explicit library(const std::string& path, const std::string& extension = LIBLOADER_DEFAULT_LIBRARY_FILE_TYPE);

        [[nodiscard]] bool isLoaded() const;

        template<typename T, typename... Params>
        bool call(const std::string& functionName, T& out, Params... params) const {
#ifdef _WIN32
            typedef T(CALLBACK* WINFUNC)(decltype(params)...);
            if (!this->isLoaded())
                return false;
            if (auto func = reinterpret_cast<WINFUNC>(GetProcAddress(this->libraryHandle, functionName.c_str())))
                out = (func)(params...);
            else
                return false;
            return true;
#endif
#ifdef __unix__
            dlerror(); // Clear existing errors
            T(*UNIXFUNC)(decltype(params)...);
            *(void**)(&UNIXFUNC) = dlsym(this->libraryHandle, functionName.c_str());
            if (dlerror() != nullptr)
                return false;
            out = (*UNIXFUNC)(params...);
            return true;
#endif
        }

        ~library();
    private:
        LIBLOADER_LIBRARY_HANDLE libraryHandle;
    };
}

#ifdef _WIN32
#include "libloader_windows.inl"
#endif
#ifdef __unix__
#include "libloader_linux.inl"
#endif
