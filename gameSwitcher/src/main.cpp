#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include <thread>
#include <chrono>

#include "font.h"
#include "gameInfo.h"

#include <sys/types.h>
#include <sys/reboot.h>

using namespace std;
namespace fs = std::filesystem;

bool needExit = false;
bool needShutdown = false;
int swWidth = 640;
int swHeight = 480;
int hwWidth = 640;
int hwHeight = 480;

string ROM_GO = "/tmp/rom_go";
string MUOS_HISTORY_DIR = "/mnt/mmc/MUOS/info/history";

SDL_Color defaultTextColor = {0, 0, 0, 255};
SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
TTF_Font *defaultFont = nullptr;
TTF_Font *titleFont = nullptr;
SDL_Joystick *joystick = nullptr;

enum RGPad
{
    RGPAD_UP = 1,
    RGPAD_RIGHT = 2,
    RGPAD_DOWN = 4,
    RGPAD_LEFT = 8,
    RGPAD_CENTER = 0,
};

enum RGButton
{
    RGBUTTON_VOL_DOWN = 1,
    RGBUTTON_VOL_UP = 2,
    RGBUTTON_A = 3,
    RGBUTTON_B = 4,
    RGBUTTON_Y = 5,
    RGBUTTON_X = 6,
    RGBUTTON_L1 = 7,
    RGBUTTON_R1 = 8,
    RGBUTTON_SELECT = 9,
    RGBUTTON_START = 10,
    RGBUTTON_MENU = 11,
    RGBUTTON_L2 = 13,
    RGBUTTON_R2 = 14,
};

void initSDL()
{
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);

    // Connect joystick 0
    SDL_JoystickEventState(SDL_ENABLE);
    joystick = SDL_JoystickOpen(0);

    window = SDL_CreateWindow("SDL2 Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, hwWidth, hwHeight, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

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
    drawTextWrapped("Game Loader", titleFont, renderer, 15, 150, 640, textColor);
    drawTextWrapped("Test Project\nPress any key or button to launch game...", defaultFont, renderer, 4, 424, 640, defaultTextColor);

    // Update the screen
    SDL_RenderPresent(renderer);
}

void startSDLPhase()
{
    initSDL();
    // Wait for a key press before quitting
    SDL_Event event;

    startRender();
    applyRender();

    bool startNextPhase = false;
    while (!startNextPhase)
    {
        while (SDL_WaitEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                needExit = true;
                startNextPhase = 1;
                break;
            }
            else if ((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) ||
                     (event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == RGBUTTON_MENU))
            {
                needExit = true;
                startNextPhase = 1;
                break;
            }
            else if ((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) ||
                     (event.type == SDL_JOYBUTTONDOWN && (event.jbutton.button == RGBUTTON_START || event.jbutton.button == RGBUTTON_A)))
            {
                startNextPhase = 1;
                break;
            }
        }

        if (startNextPhase == 1)
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
}

int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);

    for (int i = 0; i < 5; i++)
    {

        cout << "Starting game switcher. Iteration: " << i << endl;
        startSDLPhase();

        if (needExit)
        {
            // Quit Immediately
            break;
        }

        GameInfoData romInfo = {
            "Legend of Zelda, The - Link's Awakening DX (USA, Europe) (Rev 2) (SGB Enhanced) (GB Compatible)",
            "mgba_libretro.so",
            "Nintendo Game Boy Color",
            "/mnt/mmc/roms/",
            "GBC",
            "Legend of Zelda, The - Link's Awakening DX (USA, Europe) (Rev 2) (SGB Enhanced) (GB Compatible).gbc"};

        string launchPath = fs::current_path().string() + "/assets/sh/mylaunch.sh";
        printf("Launch Path: %s\n", launchPath.c_str());
        printf("Name: %s\n", romInfo.name.c_str());

#ifndef DEBUG
        printf("Writing Game Info\n");
        writeGameInfo(ROM_GO, romInfo);

        string historyPath = MUOS_HISTORY_DIR + "/" + romInfo.name + ".cfg";
        writeGameInfo(historyPath, romInfo);

        printf("Finished writing Game Info\n\n");
#endif

        break;
    }
    if (needShutdown)
    {
#ifndef DEBUG
        reboot(RB_POWER_OFF);
#endif

        return 1;
    }

    if (needExit)
    {
        return 1;
    }

    return 0;
}
