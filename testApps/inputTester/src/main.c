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
SDL_Joystick *joystick = NULL;

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
    SDL_Init(SDL_INIT_VIDEO);

    // Connect joystick 0
    SDL_Init(SDL_INIT_JOYSTICK);
    SDL_JoystickEventState(SDL_ENABLE);
    SDL_JoystickOpen(0);

    hwscreen = SDL_SetVideoMode(640, 480, 16, SDL_HWSURFACE);
    screen = SDL_CreateRGBSurface(SDL_SWSURFACE, swWidth, swHeight, 16, 0xF800, 0x7E0, 0x1F, 0);

    TTF_Init();
    defaultFont = TTF_OpenFont("assets/CourierPrime-Regular.ttf", 24);
}

void startRender()
{
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
    // drawTextWrapped("Hello World!\n\nThis is a test", defaultFont, screen, 4, 4, SDL_MapRGB(screen->format, 0, 0, 0));
}

void applyRender()
{
    drawTextWrapped("This is a RG35XX Input / Keyboard tester\nPress MENU or ESC to exit", defaultFont, screen, 4, 4, defaultTextColor);

    SDL_Color textColor = {155, 80, 0, 255};
    drawTextWrapped("RG35xx Input Reader (MuOS)", defaultFont, screen, 4, 450, textColor);

    // Scale then blit
    SDL_Rect src = {0, 0, swWidth, swHeight};
    SDL_Rect dst = {0, 0, 640, 480};
    SDL_SoftStretch(screen, &src, hwscreen, &dst);

    // Update the screen
    SDL_Flip(hwscreen);
}

int main(int argc, char *argv[])
{

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
                quit = 1;
                break;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                char text[100];
                sprintf(text, "Key Pressed\nKey: %d\nCode: %d\nUnicode: %d\nModifiers: %d", event.key.keysym.sym, event.key.keysym.scancode, event.key.keysym.unicode, event.key.keysym.mod);

                drawTextWrapped(text, defaultFont, screen, 4, 100, defaultTextColor);

                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    quit = 1;
                }
                break;
            }
            else if (event.type == SDL_JOYBUTTONDOWN)
            {
                char text[100];
                sprintf(text, "Joy Button Pressed\nButton: %d\n", event.jbutton.button);

                drawTextWrapped(text, defaultFont, screen, 4, 100, defaultTextColor);

                // Exit if menu button is pressed
                if (event.jbutton.button == RGBUTTON_MENU)
                {
                    quit = 1;
                }
                break;
            }
            else if (event.type == SDL_JOYHATMOTION)
            {
                char text[100];
                sprintf(text, "Joy Hat Pressed\nHat: %d\nValue: %d\n", event.jhat.hat, event.jhat.value);

                drawTextWrapped(text, defaultFont, screen, 4, 80, defaultTextColor);
                break;
            }
            else if (event.type == SDL_JOYAXISMOTION)
            {
                char text[100];
                sprintf(text, "Joy Axis Pressed\nAxis: %d\nValue: %d\n", event.jaxis.axis, event.jaxis.value);

                drawTextWrapped(text, defaultFont, screen, 4, 60, defaultTextColor);
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
    return 0;
}