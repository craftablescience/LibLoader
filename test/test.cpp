#include "../include/libloader.hpp"
#include <iostream>

int main() {
    auto lib = libloader::library("libtest_lib");

    lib.call("noReturn");

    int result = 0;
    lib.call<int>("add", result, 2, 5);
    std::cout << result << '\n';
}
