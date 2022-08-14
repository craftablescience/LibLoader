#include <iostream>

#if defined(_WIN32) && !defined(__MINGW32__)
    #define DllExport __declspec(dllexport)
#else
    #define DllExport
#endif

extern "C" {

DllExport void noReturn() {
    std::cout << "hi" << std::endl;
}

DllExport int getValue() {
    static int z = 0;
    return z++;
}

DllExport int add(int a, int b) {
    return a + b;
}

} // extern "C"
