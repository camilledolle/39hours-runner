#include "menu.h"

void display_menu(SDL_Surface *screen, int *option,
        SDL_Event event)
{
    TTF_Font *optionfont;
    optionfont = TTF_OpenFont("check/leadcoat.ttf", 20);

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color red = {255, 0, 0, 255};
    char *menu[4] = {"New Game", "2-Player Mode", "High Scores", "Quit"};
    int i = 0;
    int done = 0;
    SDL_Surface *s_menu[4];

    for (i = 0; i < 4; i++)
        s_menu[i] = TTF_RenderText_Blended(optionfont, menu[i], white);

    SDL_Rect pos_option;
    while (!done)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_DOWN && *option < 4)
                    (*option)++;
                else if (event.key.keysym.sym == SDLK_UP && *option > 1)
                    (*option)--;
                else if (event.key.keysym.sym == SDLK_RETURN)
                    return;
            }
            else
                break;
        }
    for (i = 0; i < 4; i++)
    {
        pos_option.x = (SCREEN_WIDTH - s_menu[i]->w) / 2;
        pos_option.y = SCREEN_HEIGHT / 2 + 40 * (i - 1);
        if (i == *option - 1)
            s_menu[i] = TTF_RenderText_Blended(optionfont, menu[i], red);
        for (int j = 0; j < 4; j++)
        {
            if (j != (*option - 1))
                s_menu[j] = TTF_RenderText_Blended(optionfont, menu[j], white);
        }
        SDL_BlitSurface(s_menu[i], NULL, screen, &pos_option);
    }
    SDL_Flip(screen);
    }

    for (i = 0; i < 4; i++)
        SDL_FreeSurface(s_menu[i]);
    TTF_CloseFont(optionfont);
}
