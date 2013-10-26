#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>

int main(int argc, char *argv[])
{
  SDL_Surface *screen = NULL;
  SDL_Surface *SpaceShip = NULL;
  SDL_Rect positionShip;

  SDL_Event event;
  int continuer = 1;


  SDL_Init(SDL_INIT_VIDEO);
  screen = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
  SDL_WM_SetCaption("EPISPACE", NULL);

  SpaceShip = SDL_LoadBMP("Spaceship_tut.bmp");
  
  positionShip.x = 100;
  positionShip.y = 100;

  SDL_EnableKeyRepeat(15, 15);
  while (continuer) /* TANT QUE la variable ne vaut pas 0 */
  {
    SDL_WaitEvent(&event);  /* On attend un événement qu'on récupère dans event */
    switch(event.type)  /* On teste le type d'événement */
    {
      case SDL_QUIT:  /* Si c'est un événement QUITTER */
        continuer = 0;  /* On met le booléen à 0, donc la boucle va s'arrêter */
        break;
      case SDL_KEYDOWN: //evenement touche enfoncee
        switch (event.key.keysym.sym)  //variable pour designer les touches
        {
          case SDLK_DOWN:
            if (positionShip.y > (480 - 80))
            {
              positionShip.y = 400;
            }
            positionShip.y += 15;
            break;
          case SDLK_UP:
            if (positionShip.y > 15 )
           // {
           //   positionShip.y = 10;
           // }
            positionShip.y -= 15;
            break;
          case SDLK_LEFT:
            if (positionShip.x > 15)
           // {
           //   positionShip.x = 10;
           // }
            positionShip.x -= 15;
            break;
          case SDLK_RIGHT:
            if (positionShip.x > (640 - 120))
            {
              positionShip.x = 520;
            }
            positionShip.x += 15;
            break;
        }
        break;
    }
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
    SDL_BlitSurface(SpaceShip, NULL, screen, &positionShip);

    SDL_Flip(screen);
  }

  
 // SDL_Delay(3000);

  SDL_FreeSurface(SpaceShip);
  SDL_Quit();

  return EXIT_SUCCESS;
}
