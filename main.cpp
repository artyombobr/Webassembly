#include <stdio.h>
#include <SDL2/SDL.h>
#include <emscripten/emscripten.h>

SDL_Renderer *renderer;
SDL_Rect temp;
SDL_Rect cars[15000];


extern "C" {
void EMSCRIPTEN_KEEPALIVE loop_handler(int x, int y) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    for (int i = 0; i < 15000; ++i) {
        temp = cars[i];
        temp.x = cars[i].x + x;
        temp.y = cars[i].y + y;
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &temp);
    }
    SDL_RenderPresent(renderer);
}
}

int main()
{
    int startPos[15000][2];
    for (int i = 0; i < 15000; ++i) {
        startPos[i][0] = rand() % 1000;
        startPos[i][1] = rand() % 500;
    }
    SDL_Window *window;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(1000, 500, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < 15000; ++i) {
        cars[i].x = startPos[i][0];
        cars[i].y = startPos[i][1];
        cars[i].w = 3;
        cars[i].h = 3;
    }
    loop_handler(0,0);
    return 0;
}

//extern "C" {
//    void EMSCRIPTEN_KEEPALIVE myFunction(int x, int y) {
//        printf("%d\n", x * y);
//    }
//}