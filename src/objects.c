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

static s_asteroid *initas(SDL_Surface *bg, int px, int py,  s_asteroid *list)
{
    int i = 0;
    while (i < 500 && list[i].posy >= 0)
        i++;
    printf("%d\n", i);
    if (i == 500)
        return list;
    s_asteroid new;
    new.posx = px;
    new.posy = py;
    new.rect.x = px;
    new.rect.y = py;
    new.surf = SDL_LoadBMP("../../asteroid.BMP");
    SDL_SetColorKey(new.surf, SDL_SRCCOLORKEY, SDL_MapRGB(new.surf->format,
                0, 0, 0));
    apply_surface(px, py, new.surf, bg);
    SDL_Flip(bg);
    list[i] = new;
    //SDL_BlitSurface(new->surf, NULL, sc, &new->rect);
    return list;
}

static void updateaste(SDL_Surface *screen, s_asteroid *listaste)
{
    int i = 0;
    while (i < 500)
    {
        if (listaste[i].posy >= 0 && listaste[i].posx < 1)
        {
            listaste[i].posy = -1;
            SDL_FreeSurface(listaste[i].surf);
        }
        i++;
    }
}

s_asteroid *addelt(SDL_Surface *screen, s_asteroid *listaste)
{
    srand(time(NULL));
    int rando = rand();
    if (rando % 10 <= 3)
        listaste = initas(screen, WIDTH - 10 ,rando % (HEIGHT - 59), listaste);
    updateaste(screen, listaste);
    return listaste;
}
