#ifndef FONT_H
#define FONT_H

#include <SDL/SDL_ttf.h>

extern SDL_Surface *screen;

void setFontScreen(SDL_Surface *s);
void drawText(char *text, TTF_Font *font, SDL_Surface *screen, int x, int y, SDL_Color color);
void drawTextWrapped(char *text, TTF_Font *font, SDL_Surface *screen, int x, int y, SDL_Color color);
void drawCharEmbedded(SDL_Surface *surface, unsigned char symbol, int x, int y, unsigned short color);
void drawStringEmbedded(SDL_Surface *surface, const char *text, int orig_x, int orig_y, unsigned short color);

#endif