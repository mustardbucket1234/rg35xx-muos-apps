#include <SDL/SDL_ttf.h>
#include "font.h"

int swWidth = 640;
int swHeight = 480;
int hwWidth = 640;
int hwHeight = 480;

SDL_Color defaultTextColor = {0, 0, 0, 255};
SDL_Surface *hwscreen = NULL;
SDL_Surface *screen = NULL;
TTF_Font *defaultFont = NULL;
TTF_Font *titleFont = NULL;
SDL_Joystick *joystick = NULL;

void initSDL()
{
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Connect joystick 0
    SDL_Init(SDL_INIT_JOYSTICK);
    SDL_JoystickEventState(SDL_ENABLE);
    SDL_JoystickOpen(0);

    hwscreen = SDL_SetVideoMode(640, 480, 16, SDL_HWSURFACE);
    screen = SDL_CreateRGBSurface(SDL_SWSURFACE, swWidth, swHeight, 16, 0xF800, 0x7E0, 0x1F, 0);

    TTF_Init();
    defaultFont = TTF_OpenFont("assets/CourierPrime-Regular.ttf", 24);
    titleFont = TTF_OpenFont("assets/Allerta-Regular.ttf", 90);
}

void startRender()
{
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
}

void applyRender()
{
    SDL_Color textColor = {255, 25, 150, 0};
    drawTextWrapped("Hello World!", titleFont, screen, 32, 150, textColor);

    drawTextWrapped("Press any button to exit...", defaultFont, screen, 4, 450, defaultTextColor);

    // Scale then blit
    SDL_Rect src = {0, 0, swWidth, swHeight};
    SDL_Rect dst = {0, 0, 640, 480};
    SDL_SoftStretch(screen, &src, hwscreen, &dst);

    // Update the screen
    SDL_Flip(hwscreen);
}

int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);
    printf("Starting Hello World App!\n");
    initSDL();
    // Wait for a key press before quitting
    SDL_Event event;

    startRender();
    applyRender();

    int quit = 0;
    while (quit == 0)
    {
        startRender();
        while (SDL_WaitEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                printf("SDL_QUIT event received\n");
                quit = 1;
                break;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                quit = 1;
                break;
            }
            else if (event.type == SDL_JOYBUTTONDOWN)
            {
                quit = 1;
                break;
            }
        }

        if (quit == 1)
        {
            break;
        }
        applyRender();
        SDL_Delay(30);
    }

    SDL_Quit();

    printf("Exiting Hello World App...\n");
    return 0;
}