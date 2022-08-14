#pragma once

#include <string>

#if defined(_WIN32)
    #define LIBLOADER_WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)
    #define LIBLOADER_APPLE
#elif defined(__unix__)
    #define LIBLOADER_LINUX
#else
    #error Platform unsupported!
#endif

#if defined(LIBLOADER_WINDOWS)
    #include <Windows.h>
    #define LIBLOADER_LIBRARY_HANDLE HINSTANCE
    #define LIBLOADER_DEFAULT_LIBRARY_FILE_TYPE ".dll"
    #undef ERROR
#elif defined(LIBLOADER_APPLE)
    #include <dlfcn.h>
    #define LIBLOADER_LIBRARY_HANDLE void*
    #define LIBLOADER_DEFAULT_LIBRARY_FILE_TYPE ".dylib"
#elif defined(LIBLOADER_LINUX)
    #include <dlfcn.h>
    #define LIBLOADER_LIBRARY_HANDLE void*
    #define LIBLOADER_DEFAULT_LIBRARY_FILE_TYPE ".so"
#else
    #error Platform unsupported!
#endif

namespace libloader {

class library {
public:
    explicit library(const std::string& path, const std::string& extension = LIBLOADER_DEFAULT_LIBRARY_FILE_TYPE);
    library(const library& other) = delete;
    library& operator=(const library& other) = delete;
    library(library&& other) noexcept;
    library& operator=(library&& other) noexcept;
    ~library();
    [[nodiscard]] bool isLoaded() const;

    template<typename T, typename... Params>
    bool call(const std::string& functionName, T& out, Params... params) const;

    template<typename... Params>
    bool callVoid(const std::string& functionName, Params... params) const;
private:
    LIBLOADER_LIBRARY_HANDLE libraryHandle;
};

library::library(library&& other) noexcept {
    this->libraryHandle = other.libraryHandle;
    other.libraryHandle = nullptr;
}

library& library::operator=(library&& other) noexcept {
    this->libraryHandle = other.libraryHandle;
    other.libraryHandle = nullptr;
    return *this;
}

bool library::isLoaded() const {
    return this->libraryHandle;
}

#if defined(LIBLOADER_WINDOWS)
    #include "libloader_windows.inl"
#elif defined(LIBLOADER_APPLE)
    #include "libloader_apple.inl"
#elif defined(LIBLOADER_LINUX)
    #include "libloader_linux.inl"
#endif

} // namespace libloader
