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
#include <unistd.h>

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

vector<GameInfoData> gameList;
GameInfoData selectedGame;
int selectedGameIndex;

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

    string text = "Game Switcher\n Roms: " + to_string(gameList.size()) + "\n";

    int maxGames = 10;
    if (maxGames > gameList.size())
    {
        maxGames = gameList.size();
    }
    if (maxGames > 0)
    {
        selectedGameIndex = (selectedGameIndex + maxGames * 5) % maxGames;
    }
    else
    {
        selectedGameIndex = 0;
    }
    for (int i = 0; i < maxGames; i++)
    {
        string prettyName = gameList[i].name;
        int maxLen = 40;
        if (prettyName.length() > maxLen)
        {
            prettyName = prettyName.substr(0, maxLen - 2) + "...";
        }
        if (i == selectedGameIndex)
        {
            text += "> " + prettyName + "\n";
        }
        else
        {
            text += prettyName + "\n";
        }
    }
    drawTextWrapped(text, defaultFont, renderer, 4, 12, 640, defaultTextColor);

    drawTextWrapped("X: Exit\nMENU + SELECT: Shutoff\nPress 'A' to start game...", defaultFont, renderer, 4, 424, 640, defaultTextColor);

    // Update the screen
    SDL_RenderPresent(renderer);
}

void startSDLPhase()
{
    initSDL();

    selectedGameIndex = 0;

    startRender();
    applyRender();

    // Wait 300 ms
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    SDL_Event event;
    bool startNextPhase = false;
    while (!startNextPhase)
    {

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                needExit = true;
                startNextPhase = 1;
                break;
            }
        }

        const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);

        // Shutdown if MENU + SELECT is pressed
        // Or if the user presses the Z key
        if ((SDL_JoystickGetButton(joystick, RGBUTTON_MENU) && SDL_JoystickGetButton(joystick, RGBUTTON_SELECT)) ||
            keyboardState[SDL_SCANCODE_Z])
        {
            needExit = true;
            needShutdown = true;
            startNextPhase = 1;
        }

        // Exit if the user presses X or ESC
        if (SDL_JoystickGetButton(joystick, RGBUTTON_X) || keyboardState[SDL_SCANCODE_ESCAPE])
        {
            needExit = true;
            startNextPhase = 1;
        }

        if (SDL_JoystickGetHat(joystick, 0) == RGPAD_DOWN || SDL_JoystickGetHat(joystick, 0) == RGPAD_RIGHT)
        {
            selectedGameIndex++;
        }
        else if (SDL_JoystickGetHat(joystick, 0) == RGPAD_UP || SDL_JoystickGetHat(joystick, 0) == RGPAD_LEFT)
        {
            selectedGameIndex--;
        }
        // Continue if user presses A or SPACE
        if (SDL_JoystickGetButton(joystick, RGBUTTON_A) || keyboardState[SDL_SCANCODE_SPACE])
        {
            startNextPhase = 1;
            if (gameList.size() > 0)
            {
                if (selectedGameIndex >= gameList.size())
                {
                    selectedGameIndex = gameList.size() - 1;
                }
                if (selectedGameIndex < 0)
                {
                    selectedGameIndex = 0;
                }
                selectedGame = gameList[selectedGameIndex];
            }
        }

        startRender();
        applyRender();

        SDL_Delay(30);
        if (startNextPhase == 1)
        {
            break;
        }
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

    cout << "Starting game switcher" << endl;

    for (int i = 0; i < 999; i++)
    {
#ifdef DEBUG
        gameList = loadGameListAtPath("/mnt/e/RG35xx/MuOSDump/.mnt.mmc/MUOS/info/history");
#else
        gameList = loadGameListAtPath("/mnt/mmc/MUOS/info/history");
#endif

        startSDLPhase();

        if (needShutdown)
        {
            printf("User has triggerd a shutdown....\n");
            std::this_thread::sleep_for(std::chrono::milliseconds(750));
#ifndef DEBUG
            reboot(RB_POWER_OFF);
#endif
            return 1;
        }
        else if (needExit)
        {
            printf("User has triggered an exit...\n");
            return 1;
        }

        if (selectedGame.active)
        {

            string launchPath = fs::current_path().string() + "/assets/sh/mylaunch.sh";
            printf("Launch Path: %s\n", launchPath.c_str());
            printf("Name: %s\n", selectedGame.name.c_str());

            printf("Writing Game Info\n");
            string historyPath = MUOS_HISTORY_DIR + "/" + selectedGame.name + ".cfg";

#ifndef DEBUG
            // writeGameInfo(ROM_GO, selectedGame);
            writeGameInfo(historyPath, selectedGame);
#endif

            printf("Finished writing Game Info\n\n");
            printf("Proceeding to game...\n");

            std::this_thread::sleep_for(std::chrono::milliseconds(25));
            sync();
            std::this_thread::sleep_for(std::chrono::milliseconds(25));
            // Execute assets/sh/mylaunch.sh
            // system("/mnt/mmc/MUOS/retroarch");
            /// system(launchPath.c_str());

            string romPath = selectedGame.drive + selectedGame.folder + "/" + selectedGame.fileName;
            string cmd = "/mnt/mmc/MUOS/retroarch -c \"/mnt/mmc/MUOS/.retroarch/retroarch.cfg\" -L \"/mnt/mmc/MUOS/core/" + selectedGame.core + "\" \"" + romPath + "\"";
            printf("Executing Command: %s\n", cmd.c_str());
#ifndef DEBUG
            system(cmd.c_str());
#endif
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
        }
    }
    return 0;
}
