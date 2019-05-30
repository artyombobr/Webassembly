#include <stdio.h>
#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>
#include <emscripten.h>

#define REC_SQRT2 0.7071067811865475

enum input_state
{
    NOTHING_PRESSED = 0,
    UP_PRESSED = 1,
    DOWN_PRESSED = 1<<1,
    LEFT_PRESSED = 1<<2,
    RIGHT_PRESSED = 1<<3
};

struct context
{
    SDL_Renderer *renderer;
    //SDL_Texture *owl_tex;
    SDL_Rect cars[15000];
    enum input_state active_state;
    int owl_vx;
    int owl_vy;
};

//int get_owl_texture(struct context * ctx)
//{
//    SDL_Surface *image = IMG_Load("assets/owl.png");
//    if (!image)
//    {
//        printf("IMG_Load: %s\n", IMG_GetError());
//        return 0;
//    }
//    ctx->owl_tex = SDL_CreateTextureFromSurface(ctx->renderer, image);
//    SDL_FreeSurface (image);
//    return 1;
//}

int numFrames = 0;
int startTime = 0;

void printFps()
{
    int elapsedMS = SDL_GetTicks() - startTime;
    ++numFrames;
    if (elapsedMS) {
        double elapsedSeconds = elapsedMS / 1000.0;
        double fps = numFrames / elapsedSeconds;
        printf("%s %f\n", "FPS: ", fps);
    }
}

void process_input(struct context *ctx)
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        //printf("%d\n", event.key.keysym.sym);
        //printf("%u\n", event.key.type);
        //printf("%d\n", SDLK_UP);
        switch (event.key.keysym.sym)
        {
            case SDLK_UP:
                if (event.key.type == SDL_KEYDOWN)
                    ctx->active_state = UP_PRESSED;
                else if (event.key.type == SDL_KEYUP)
                    ctx->active_state = UP_PRESSED;
                break;
            case SDLK_DOWN:
                if (event.key.type == SDL_KEYDOWN)
                    ctx->active_state = DOWN_PRESSED;
                else if (event.key.type == SDL_KEYUP)
                    ctx->active_state = DOWN_PRESSED;
                break;
            case SDLK_LEFT:
                if (event.key.type == SDL_KEYDOWN)
                    ctx->active_state = LEFT_PRESSED;
                else if (event.key.type == SDL_KEYUP)
                    ctx->active_state = LEFT_PRESSED;
                break;
            case SDLK_RIGHT:
                if (event.key.type == SDL_KEYDOWN)
                    ctx->active_state = RIGHT_PRESSED;
                else if (event.key.type == SDL_KEYUP)
                    ctx->active_state = RIGHT_PRESSED;
                break;
            default:
                break;
        }
    }

    ctx->owl_vy = 0;
    ctx->owl_vx = 0;
    if (ctx->active_state & UP_PRESSED)
        ctx->owl_vy = -5;
    if (ctx->active_state & DOWN_PRESSED)
        ctx->owl_vy = 5;
    if (ctx->active_state & LEFT_PRESSED)
        ctx->owl_vx = -5;
    if (ctx->active_state & RIGHT_PRESSED)
        ctx->owl_vx = 5;

    if (ctx->owl_vx != 0 && ctx->owl_vy != 0)
    {
        ctx->owl_vx *= REC_SQRT2;
        ctx->owl_vy *= REC_SQRT2;
    }
}

void loop_handler(void *arg)
{
     context *ctx = static_cast<context*>(arg);
     SDL_SetRenderDrawColor(ctx->renderer, 255, 255, 255, 255);
     process_input(ctx);
     ctx->active_state = NOTHING_PRESSED;
     SDL_RenderClear(ctx->renderer);
     for (int i = 0; i < 15000; ++i) {
         ctx->cars[i].x += ctx->owl_vx;
         ctx->cars[i].y += ctx->owl_vy;
         SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 255, 255);
         SDL_RenderFillRect(ctx->renderer, &ctx->cars[i]);
     }
   SDL_RenderPresent(ctx->renderer);
   //printFps();
}

int main()
{
    int startPos[15000][2];
    for (int i = 0; i < 15000; ++i) {
        startPos[i][0] = rand() % 1000;
        startPos[i][1] = rand() % 500;
    }
    SDL_Window *window;
    context ctx;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(1000, 500, 0, &window, &ctx.renderer);
    SDL_SetRenderDrawColor(ctx.renderer, 255, 255, 255, 255);
    //get_owl_texture(&ctx);
    ctx.active_state = NOTHING_PRESSED;
    for (int i = 0; i < 15000; ++i) {
        ctx.cars[i].x = startPos[i][0];
        ctx.cars[i].y = startPos[i][1];
        ctx.cars[i].w = 3;
        ctx.cars[i].h = 3;
    }
    emscripten_set_main_loop_arg(loop_handler, &ctx, -1, 1);
    return 0;
}
