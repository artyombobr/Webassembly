#include <stdio.h>
#include <emscripten/emscripten.h>

//int main(int argc, char ** argv) {
//    printf("Hello World\n");
//}

void EMSCRIPTEN_KEEPALIVE myFunction(int x, int y) {
    printf("%d\n", x*y);
}