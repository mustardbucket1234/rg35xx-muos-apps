#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "font.h"

int swWidth = 640;
int swHeight = 480;
int hwWidth = 640;
int hwHeight = 480;

SDL_Color defaultTextColor = {0, 0, 0, 255};
SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
TTF_Font *defaultFont = nullptr;
TTF_Font *titleFont = nullptr;
SDL_Joystick *joystick = nullptr;

void initSDL()
{
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);

    // Connect joystick 0
    SDL_JoystickEventState(SDL_ENABLE);
    joystick = SDL_JoystickOpen(0);

    window = SDL_CreateWindow("SDL2 Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, hwWidth, hwHeight, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Init();
    defaultFont = TTF_OpenFont("assets/font/CourierPrime-Regular.ttf", 24);
    titleFont = TTF_OpenFont("assets/font/CourierPrime-Regular.ttf", 90);
}

void startRender()
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
}

void applyRender()
{
    SDL_Color textColor = {255, 25, 150, 0};
    drawTextWrapped("Hello World", titleFont, renderer, 15, 150, 640, textColor);
    drawTextWrapped("Now in C++ and SDL2!\nPress any key or button to exit...", defaultFont, renderer, 4, 424, 640, defaultTextColor);

    // Update the screen
    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[])
{
    std::cout << "Hello World: Compiled as C++" << std::endl;
    setbuf(stdout, NULL);
    initSDL();
    // Wait for a key press before quitting
    SDL_Event event;

    startRender();
    applyRender();

    int quit = 0;
    while (quit == 0)
    {
        while (SDL_WaitEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                std::cout << "SDL_QUIT event received" << std::endl;
                quit = 1;
                break;
            }
            else if (event.type == SDL_KEYDOWN || event.type == SDL_JOYBUTTONDOWN)
            {
                quit = 1;
                break;
            }
        }

        if (quit == 1)
        {
            break;
        }
        startRender();
        applyRender();
        SDL_Delay(30);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(defaultFont);
    TTF_CloseFont(titleFont);
    TTF_Quit();
    SDL_Quit();

    std::cout << "Exiting Hello World App..." << std::endl;
    return 0;
}
