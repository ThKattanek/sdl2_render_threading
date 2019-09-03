//////////////////////////////////////////////////
//                                              //
// SDL2 Render Threading                        //
// von Thorsten Kattanek                        //
//                                              //
// #file: main.cpp                              //
//                                              //
// Dieser Sourcecode ist Copyright geschützt!   //
// Geistiges Eigentum von Th.Kattanek           //
//                                              //
// Letzte Änderung am 06.09.2019                //
//                                  //
//                                              //
//////////////////////////////////////////////////

#include <iostream>
#include <SDL2/SDL.h>

using namespace std;

static SDL_Thread *thread1;
static SDL_Renderer *ren;
static SDL_Texture *tex;

static bool quit = false;

static int RenderThread(void *userdata);
static int RenderThread(void*)
{
    while(!quit)
    {
        //Clear the renderer
        SDL_RenderClear(ren);

        //Draw the texture
        SDL_RenderCopy(ren, tex, nullptr, nullptr);

        //Update the screen
        SDL_RenderPresent(ren);
    }
    return 0;
}

#undef main
int main()
{
    // SLD initialize only video
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create a SDL Window
    SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, 300, 200, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    if (win == nullptr){
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create a SDL Renderer
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr){
        SDL_DestroyWindow(win);
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create a SDL Surface with load the test.bmp image
    SDL_Surface *bmp = SDL_LoadBMP("test.bmp");
    if (bmp == nullptr){
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create a Textur from Surfacee
    tex = SDL_CreateTextureFromSurface(ren, bmp);
    SDL_FreeSurface(bmp);
    if (tex == nullptr){
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create and running the Render Thread
    thread1 = SDL_CreateThread(RenderThread, "RenderThread", nullptr);

    // main loop with event handling
    SDL_Event e;
    while (!quit)
    {
        while (SDL_PollEvent(&e)){
            //If user closes the window
            if (e.type == SDL_QUIT){
                quit = true;
            }
            //If user presses any key
            if (e.type == SDL_KEYDOWN){
                quit = true;
            }
            //If user clicks the mouse
            if (e.type == SDL_MOUSEBUTTONDOWN){
                quit = true;
            }
        }
        SDL_Delay(1);
    }

    // Destroys all created objects
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);

    // SDL Quit
    SDL_Quit();

    // Bye
    return 0;
}
