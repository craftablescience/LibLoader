libloader::library::library(const std::string& path, const std::string& extension) {
    auto fullPath = path + extension;
    this->libraryHandle = LoadLibrary(TEXT(fullPath.c_str()));
}

bool libloader::library::isLoaded() const {
    return this->libraryHandle;
}

libloader::library::~library() {
    FreeLibrary(this->libraryHandle);
}
