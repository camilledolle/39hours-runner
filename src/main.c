#include "objects.h"

static SDL_Surface *initscreen(void)
{
    SDL_Surface *screen = NULL;
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        printf (" SDL_Init :␣%s\n", SDL_GetError ());
        return NULL;
    }

    SDL_WM_SetIcon(SDL_LoadBMP("../../asteroid.BMP"), NULL);
    screen = SDL_SetVideoMode (WIDTH, HEIGHT, 32, SDL_HWSURFACE);

    if (!screen)
    {
        printf (" SDL_SetVideoMode :␣%s\n", SDL_GetError ());
        SDL_Quit ();
        return NULL;
    }

    SDL_WM_SetCaption(" \\o/ Epirun \\o/", NULL);
    SDL_Flip(screen);

    return screen;
} 

int main(void)
{
    SDL_Surface *screen = initscreen();
    SDL_Surface *background = addsurface(screen, 0, 0, "../../space.bmp");
    SDL_Event event;
    int cont = 1;
    s_asteroid *listaste = malloc(500 * sizeof (s_asteroid));
    for (int i = 0; i < 500; i++)
        listaste[i].posy = -1;
    if (!screen)
        return 1;
    while (cont)
    {
        SDL_PollEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                cont = 0;
                break;
        }
    listaste = addelt(screen, listaste);
    }
    return 0;
}
