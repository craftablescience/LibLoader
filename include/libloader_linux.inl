libloader::library::library(const std::string& path, const std::string& extension) {
    auto fullPath = path + extension;
    this->libraryHandle = dlopen(fullPath.c_str(), RTLD_LAZY);
}

bool libloader::library::isLoaded() const {
    return this->libraryHandle != nullptr;
}

libloader::library::~library() {
    dlclose(this->libraryHandle);
}
