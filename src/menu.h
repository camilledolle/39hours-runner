#ifndef MENU_H
# define MENU_H

# include <stdlib.h>
# include <SDL/SDL.h>
# include <SDL/SDL_ttf.h>
# include <SDL/SDL_mixer.h>
# include "init.h"

void display_menu(SDL_Surface *screen, int *opt, SDL_Event event);

#endif /* !MENU_H */
