library::library(const std::string& path, const std::string& extension) {
    auto fullPath = path + extension;
    this->libraryHandle = LoadLibrary(TEXT(fullPath.c_str()));
}

library::~library() {
    if (this->isLoaded()) {
        FreeLibrary(this->libraryHandle);
    }
}

template<typename T, typename... Params>
bool library::call(const std::string &functionName, T &out, Params... params) const {
    typedef T(__cdecl* WINFUNC)(decltype(params)...);
    if (!this->isLoaded())
        return false;
    if (auto func = reinterpret_cast<WINFUNC>(GetProcAddress(this->libraryHandle, functionName.c_str())))
        out = (func)(params...);
    else
        return false;
    return true;
}

template<typename... Params>
bool library::callVoid(const std::string &functionName, Params... params) const {
    typedef void(__cdecl* WINFUNC)(decltype(params)...);
    if (!this->isLoaded())
        return false;
    if (auto func = reinterpret_cast<WINFUNC>(GetProcAddress(this->libraryHandle, functionName.c_str())))
        (func)(params...);
    else
        return false;
    return true;
}
