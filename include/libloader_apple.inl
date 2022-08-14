library::library(const std::string& path, const std::string& extension) {
    auto fullPath = path + extension;
    this->libraryHandle = dlopen(fullPath.c_str(), RTLD_LAZY);
}

library::~library() {
    if (this->isLoaded()) {
        dlclose(this->libraryHandle);
    }
}

template<typename T, typename... Params>
bool library::call(const std::string &functionName, T &out, Params... params) const {
    dlerror(); // Clear existing errors
    T(*APPLEFUNC)(decltype(params)...);
    *(void**)(&APPLEFUNC) = dlsym(this->libraryHandle, functionName.c_str());
    if (dlerror())
        return false;
    out = (*APPLEFUNC)(params...);
    return true;
}

template<typename... Params>
bool library::callVoid(const std::string &functionName, Params... params) const {
    dlerror(); // Clear existing errors
    void(*APPLEFUNC)(decltype(params)...);
    *(void**)(&APPLEFUNC) = dlsym(this->libraryHandle, functionName.c_str());
    if (dlerror())
        return false;
    (*APPLEFUNC)(params...);
    return true;
}
