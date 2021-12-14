extern "C" {
    #include <stdio.h>

    int add(int a, int b) {
        return a + b;
    }

    void noReturn() {
        printf("hello world\n");
    }
}
