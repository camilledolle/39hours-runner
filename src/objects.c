#include "objects.h"

SDL_Surface *addsurface(SDL_Surface *screen, int posx, int posy, char *path)
{
    SDL_Rect posbackground;
    posbackground.x = posx;
    posbackground.y = posy;
    SDL_Surface *background = SDL_LoadBMP(path);
    SDL_BlitSurface(background, NULL, screen, &posbackground);
    SDL_Flip(screen);
    return background;
}

static void apply_surface(int x, int y, SDL_Surface *src, SDL_Surface *dst)
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface(src, NULL, dst, &offset);
}

static SDL_Surface *init_image(SDL_Surface *asteroid, int type)
{
    if (!asteroid && type == 1)
        asteroid = SDL_LoadBMP("../check/asteroids1.bmp");
    else if (!asteroid && type == 3)
        asteroid = SDL_LoadBMP("../check/forcefield.bmp");
    else if (!asteroid && type == 4)
        asteroid = SDL_LoadBMP("../check/battery.bmp");
    return asteroid;
}
static s_asteroid *initas(SDL_Rect pos, s_asteroid *list, int type)
{
    SDL_Surface *asteroid = NULL;
    asteroid = init_image(asteroid, type);
    s_asteroid *new = malloc(sizeof (s_asteroid));
    new->posx = pos.x;
    new->posy = pos.y;
    new->prev = list;
    if (list)
        list->next = new;
    new->next = NULL;
    new->surf = asteroid;
    new->type = type; 
    SDL_SetColorKey(new->surf, SDL_SRCCOLORKEY, SDL_MapRGB(new->surf->format,
                0, 0, 0));
    return new;
}

static void updateaste(s_asteroid **listaste, int offset)
{
    s_asteroid *tmp = *listaste;
    while (tmp)
    {
        if (tmp->posx > 2 * SCREEN_WIDTH && tmp->posx < offset)
        {
            if (tmp->next)
                tmp->next->prev = tmp->prev;
            if (tmp->prev)
                tmp->prev->next = tmp->next;
            if (!tmp->prev && !tmp->next)
            {
                free(tmp);
                *listaste = NULL;
            }
            free(tmp);
        }
        else
            tmp->posx -= offset;
        tmp = tmp->prev;
    }
}

s_asteroid *addelt(s_asteroid *listaste, int offset)
{
    int rando = rand();
    SDL_Rect pos;
    pos.x = SCREEN_WIDTH + rando % (SCREEN_WIDTH - 50);
    pos.y =  rando % (SCREEN_HEIGHT - 59);
    if (rando % 100 < offset / 3)
        listaste = initas(pos, listaste, 1);
    if (rando % 1000 >= 995 && rando % 1000 < 999)
        listaste = initas(pos, listaste, 4);
    if (rando % 1000 == 999)
        listaste = initas(pos,listaste, 3);
    updateaste(&listaste, offset);
    return listaste;
}

void drawelt(SDL_Surface **screen, s_asteroid *listaste)
{
    s_asteroid *tmp = listaste;
    while (tmp)
    {
        apply_surface(tmp->posx, tmp->posy, tmp->surf, *screen);
        tmp = tmp->prev;
    }
}
