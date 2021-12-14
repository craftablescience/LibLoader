#include "../include/libloader.hpp"
#include <iostream>

int main() {
#ifdef __unix__
    auto lib = libloader::library("libtest_lib.so");
#endif
    int result = 0;
    lib.call<int>("add", result, 2, 5);
    std::cout << result << '\n';
}
