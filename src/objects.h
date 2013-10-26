#ifndef OBJECTS_H
# define OBJECTS_H

# include <stdlib.h>
# include <stdio.h>
# include <SDL/SDL.h>
# include <time.h>

# define WIDTH 564
# define HEIGHT 386

typedef struct asteroid s_asteroid;

struct asteroid
{
    int posx;
    int posy;
    SDL_Surface *surf;
    SDL_Rect rect;
};

s_asteroid *addelt(SDL_Surface *screen, s_asteroid *listaste);
 SDL_Surface * addsurface(SDL_Surface *screen, int posx, int posy, char *path);

#endif /* !OBJECTS_H */
