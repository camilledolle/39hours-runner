#ifndef INIT_H
# define INIT_H

# include <stdlib.h>
# include <stdio.h>
# include <SDL/SDL.h>
# include <SDL/SDL_ttf.h>
# include <SDL/SDL_mixer.h>
# include <SDL/SDL_image.h>
# include <time.h>
# include <string.h>
# include "objects.h"
# include "menu.h"

# define SCREEN_WIDTH 800
# define SCREEN_HEIGHT 480
# define SCREEN_BPP 32
# define SHIP_HEIGHT 60
# define SHIP_WIDTH 88
/*
typedef struct all s_all
struct all
{
    s_spaceship *spaceship;
    s_bg *background;
    s_asteroid *items;
    SDL_Surface *screen;
}
*/

typedef struct spaceship s_spaceship;
struct spaceship
{
    int invincible;
    int points;
    int coll;
    int life;
    SDL_Surface *surf;
    SDL_Surface *bou;
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
