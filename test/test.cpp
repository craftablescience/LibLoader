#include "../include/libloader.hpp"
#include <iostream>

int main() {
    auto lib = libloader::library("libtest_lib");
    if (!lib.isLoaded()) {
        // MSVC
        lib = libloader::library("test_lib");
    }

    int result = 0;
    lib.call<int>("getValue", result);
    std::cout << result << ' ';
    lib.callVoid("noReturn");
    lib.call<int>("getValue", result);
    std::cout << result << '\n';

    lib.call<int>("add", result, 2, 5);
    std::cout << result << '\n';
}
