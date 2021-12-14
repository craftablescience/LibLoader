libloader::library::library(const std::string& path) {
    this->libraryHandle = LoadLibrary(TEXT(path.c_str()));
}

bool libloader::library::isLoaded() const {
    return this->libraryHandle;
}

libloader::library::~library() {
    FreeLibrary(this->libraryHandle);
}
