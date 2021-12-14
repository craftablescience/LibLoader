libloader::library::library(const std::string& path) {
    this->libraryHandle = dlopen(path.c_str(), RTLD_LAZY);
}

bool libloader::library::isLoaded() const {
    return this->libraryHandle != nullptr;
}

libloader::library::~library() {
    dlclose(this->libraryHandle);
}
