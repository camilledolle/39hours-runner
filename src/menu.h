#ifndef MENU_H
# define MENU_H

# include <stdlib.h>
# include <SDL/SDL.h>
# include <SDL/SDL_ttf.h>
# include <SDL/SDL_mixer.h>

# define SCREEN_WIDTH 640
# define SCREEN_HEIGHT 480

void display_menu(SDL_Surface *screen, int *opt, SDL_Event event);

#endif /* !MENU_H */
