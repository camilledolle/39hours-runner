#ifndef INIT_H
# define INIT_H

# include <stdlib.h>
# include <stdio.h>
# include <SDL/SDL.h>
# include <SDL/SDL_ttf.h>
# include "objects.h"

# define SCREEN_WIDTH 640
# define SCREEN_HEIGHT 480
# define SCREEN_BPP 32
# define SHIP_HEIGHT 80
# define SHIP_WIDTH 120

typedef struct spaceship s_spaceship;
struct spaceship
{
    int points;
    int coll;
    int life;
    SDL_Surface *surf;
    SDL_Rect rect;
};

typedef struct background s_bg;
struct background
{
    SDL_Surface *surf;
    int x;
    int y;
};

#endif /* !INIT_H */
