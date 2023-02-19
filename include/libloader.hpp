#pragma once

#include <algorithm>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>

#if defined(_WIN32)
    #define LIBLOADER_WINDOWS
    #include <Windows.h>
    #undef ERROR
#elif defined(__APPLE__) && defined(__MACH__)
    #define LIBLOADER_APPLE
    #include <dlfcn.h>
#elif defined(__unix__)
    #define LIBLOADER_LINUX
    #include <dlfcn.h>
#else
    #error Platform unsupported!
#endif

namespace libloader {

#if defined(LIBLOADER_WINDOWS)
    using InternalLibraryHandle = HINSTANCE;
    constexpr const char* DEFAULT_LIBRARY_FILE_TYPE = ".dll";
#elif defined(LIBLOADER_APPLE)
    using InternalLibraryHandle = void*;
    constexpr const char* DEFAULT_LIBRARY_FILE_TYPE = ".dylib";
#elif defined(LIBLOADER_LINUX)
    using InternalLibraryHandle = void*;
    constexpr const char* DEFAULT_LIBRARY_FILE_TYPE = ".so";
#else
    #error Platform unsupported!
#endif

namespace detail {

template<std::size_t N>
struct string_literal {
    // NOLINTNEXTLINE(google-explicit-constructor)
    constexpr string_literal(const char(&str)[N]) {
        std::copy_n(str, N, value);
    }

    [[nodiscard]] const char* data() const {
        return &this->value[0];
    }

    char value[N] {};
};

} // namespace detail

class library {
public:
    explicit library(const std::string& path, const std::string& extension = DEFAULT_LIBRARY_FILE_TYPE);
    library(const library& other) = delete;
    library& operator=(const library& other) = delete;
    library(library&& other) noexcept;
    library& operator=(library&& other) noexcept;
    ~library();
    [[nodiscard]] bool isLoaded() const;

    template<typename T, typename... Params>
    auto call(std::string_view functionName, Params... params) const;

    template<typename T, detail::string_literal FunctionName, typename... Params>
    auto get() const;

private:
    InternalLibraryHandle libraryHandle;
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

template<typename T, detail::string_literal FunctionName, typename... Params>
auto library::get() const {
    static const auto functionTemplate = [&] (Params... params) {
        return this->call<T, Params...>(FunctionName.data(), params...);
    };
    return functionTemplate;
}

#if defined(LIBLOADER_WINDOWS)

library::library(const std::string& path, const std::string& extension /*= DEFAULT_LIBRARY_FILE_TYPE*/) {
    auto fullPath = path + extension;
    this->libraryHandle = LoadLibrary(TEXT(fullPath.c_str()));
}

library::~library() {
    if (this->isLoaded()) {
        FreeLibrary(this->libraryHandle);
    }
}

template<typename T, typename... Params>
auto library::call(std::string_view functionName, Params... params) const {
    if constexpr (std::is_same_v<T, void>) {
        typedef void(__cdecl* FUNC)(Params...);
        if (!this->isLoaded())
            return false;
        const auto func = reinterpret_cast<FUNC>(GetProcAddress(this->libraryHandle, functionName.data()));
        if (!func)
            return false;
        (*func)(std::forward<Params>(params)...);
        return true;
    } else {
        typedef T(__cdecl* FUNC)(Params...);
        if (!this->isLoaded())
            return std::optional<T>{std::nullopt};
        const auto func = reinterpret_cast<FUNC>(GetProcAddress(this->libraryHandle, functionName.data()));
        if (func)
            return std::make_optional<T>((*func)(std::forward<Params>(params)...));
        return std::optional<T>{std::nullopt};
    }
}

#elif defined(LIBLOADER_APPLE) || defined(LIBLOADER_LINUX)

library::library(const std::string& path, const std::string& extension /*= DEFAULT_LIBRARY_FILE_TYPE*/) {
    auto fullPath = path + extension;
    this->libraryHandle = dlopen(fullPath.c_str(), RTLD_LAZY);
}

library::~library() {
    if (this->isLoaded()) {
        dlclose(this->libraryHandle);
    }
}

template<typename T, typename... Params>
auto library::call(std::string_view functionName, Params... params) const {
    dlerror(); // Clear existing errors

    if constexpr (std::is_same_v<T, void>) {
        void(*func)(Params...);
        *(void**)(&func) = dlsym(this->libraryHandle, functionName.data());
        if (dlerror())
            return false;
        (*func)(std::forward<Params>(params)...);
        return true;
    } else {
        T(*func)(Params...);
        *(void**)(&func) = dlsym(this->libraryHandle, functionName.data());
        if (dlerror())
            return std::optional<T>{std::nullopt};
        return std::make_optional<T>((*func)(std::forward<Params>(params)...));
    }
}

#else

#error Platform unsupported!

#endif

} // namespace libloader
