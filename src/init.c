#include "init.h"

SDL_Surface *load_image(const char *filename)
{
    SDL_Surface *loaded_image = NULL;
    SDL_Surface *optimized_image = NULL;
    loaded_image = SDL_LoadBMP(filename);
    if (loaded_image != NULL)
    {
        optimized_image = SDL_DisplayFormat(loaded_image);
        SDL_FreeSurface(loaded_image);
    }
    return optimized_image;
}

void apply_surface(int x, int y, SDL_Surface* src, SDL_Surface* dst)
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface(src, NULL, dst, &offset);
}

int init(SDL_Surface **screen)
{
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        printf("Can't init SDL:  %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    atexit(SDL_Quit);
    *screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16,
            SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (*screen == NULL)
    {
        printf("Can't set video mode: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    SDL_WM_SetCaption( "EpiRun", NULL);
    return EXIT_SUCCESS;
}

int scroll(int *bg_x, int *bg_y, SDL_Surface *background, SDL_Surface *screen,
        SDL_Surface *spaceship)
{
    int offset = SDL_GetTicks() / 5000 + 1;
    *bg_x -= 1 * offset;
    if (*bg_x <= -background->w)
        *bg_x = 0;
    apply_surface(*bg_x, *bg_y, background, screen);
    apply_surface(*bg_x + background->w, *bg_y, background, screen);
    apply_surface(10, 240, spaceship, screen);
    //if (SDL_Flip(screen) == -1)
     //   return EXIT_FAILURE;
    return offset;
}

int collision(int spaceship_x, int obstacle_x, int offset)
{
    if ((obstacle_x + offset) == spaceship_x)
        return 1;
    return 0;
}

void draw(SDL_Surface *screen, s_asteroid **list_asteroid, int *bg_x, int *bg_y)
{
    SDL_Surface *background = load_image("../check/background.bmp");
    SDL_Surface *spaceship = load_image("../check/spaceship.bmp");
    //SDL_Color black = {0, 0, 0};
    SDL_SetColorKey(spaceship, SDL_SRCCOLORKEY, SDL_MapRGB(spaceship->format,
                0, 0, 0));
    apply_surface(0, 0, background, screen);
    //SDL_Rect pos;
    //pos.x = 0;
    //pos.y = 0;
    //TTF_Init();
    //char score[100];
    //int points = 0;
    //sprintf(score, "score : %d", points);
    //TTF_Font *police = TTF_OpenFont("leadcoat.ttf", 50);
    //SDL_Surface *text = TTF_RenderText_Blended(police, "score = 0", black);
    int offset = scroll(bg_x, bg_y, background, screen, spaceship);
    *list_asteroid = addelt(*list_asteroid, offset);
    drawelt(&screen, *list_asteroid);
    SDL_Flip(screen);
    SDL_FreeSurface(background);
    SDL_FreeSurface(spaceship);
    //sprintf(score, "score : %d", points);
    //SDL_FreeSurface(text);
    //pos.x = 0;
    //pos.y = 0;
    //text = TTF_RenderText_Blended(police, score, black);
    //points += offset;
    //apply_surface(pos.x, pos.y, text,background);

}

int main(void)
{
    int bg_x = 0;
    int bg_y = 0;
    int quit = 0;
    SDL_Event event;
    SDL_Surface *screen = NULL;
    init(&screen);
    s_asteroid *list_asteroid = NULL;
    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                quit = 1;
        }
        draw(screen, &list_asteroid, &bg_x, &bg_y);
        /*if (collision(10, 340, bg_x))
          {
          bg_x = 0;
          bg_y = 0;
          }*/
        //SDL_Delay(3000);
    }
    SDL_Quit();
    return 0;
}
