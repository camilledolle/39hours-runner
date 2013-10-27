#ifndef OBJECTS_H
# define OBJECTS_H

# include "init.h"

typedef struct asteroid s_asteroid;
struct asteroid
{
    int type; //1 = asteroid; 2 = ennemi; 3 = bonus points; 4 = bonus life
              //5 = bouclier; 6 = bombe
    int posx;
    int posy;
    s_asteroid *prev;
    s_asteroid *next;
    SDL_Surface *surf;
};


void drawelt(SDL_Surface **screen, s_asteroid *listaste);
s_asteroid *addelt(s_asteroid *listaste, int offset);
SDL_Surface * addsurface(SDL_Surface *screen, int posx, int posy, char *path);
void freelist(s_asteroid **listaste);

#endif /* !OBJECTS_H */
