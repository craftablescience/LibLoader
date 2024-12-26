#include "../include/libloader.hpp"
#include <iostream>

int main() {
    auto lib = libloader::library("libtest_lib");
    if (!lib.isLoaded()) {
        // MSVC
        lib = libloader::library("test_lib");
    }

    int result = *lib.call<int>("getValue");
    std::cout << result << ' ';
    lib.call<void>("noReturn");
    result = *lib.call<int>("getValue");
    std::cout << result << '\n';

    result = *lib.call<int>("add", 2, 5);
    std::cout << result << '\n';

    auto noReturn = lib.get<void>("noReturn");
    auto getValue = lib.get<int>("getValue");
    noReturn();
    std::cout << *getValue() << '\n';

    auto add = lib.get<int, int, int>("add");
    std::cout << *add(13, 15) << '\n';
}
