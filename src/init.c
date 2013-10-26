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

int scroll(s_bg *backg, SDL_Surface *screen,
        s_spaceship *s)
{
    int offset = SDL_GetTicks() / 5000 + 1;
    backg->x -= 1 * offset;
    if (backg->x <= -backg->surf->w)
        backg->x = 0;
    apply_surface(backg->x, backg->y, backg->surf, screen);
    apply_surface(backg->x + backg->surf->w, backg->y, backg->surf, screen);
    //apply_surface(s->rect.x, s->rect.y, s->surf, screen);
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

void draw(SDL_Surface *screen, s_spaceship *s, s_asteroid **list_asteroid,
        s_bg *backg)
{
    //SDL_Surface *background = load_image("../check/background.bmp");
    //SDL_Surface *spaceship = load_image("../check/spaceship.bmp");
    //SDL_Color black = {0, 0, 0};
    SDL_SetColorKey(s->surf, SDL_SRCCOLORKEY, SDL_MapRGB(s->surf->format,
                0, 0, 0));
    apply_surface(0, 0, backg->surf, screen);
    //SDL_Rect pos;
    //pos.x = 0;
    //pos.y = 0;
    //TTF_Init();
    //char score[100];
    //int points = 0;
    //sprintf(score, "score : %d", points);
    //TTF_Font *police = TTF_OpenFont("leadcoat.ttf", 50);
    //SDL_Surface *text = TTF_RenderText_Blended(police, "score = 0", black);
    int offset = scroll(backg, screen, s);
    *list_asteroid = addelt(*list_asteroid, offset);
    drawelt(&screen, *list_asteroid);
    apply_surface(s->rect.x, s->rect.y, s->surf, screen);
    SDL_Flip(screen);
    //SDL_FreeSurface(backg->surf);
    //SDL_FreeSurface(s->surf);
    //sprintf(score, "score : %d", points);
    //SDL_FreeSurface(text);
    //pos.x = 0;
    //pos.y = 0;
    //text = TTF_RenderText_Blended(police, score, black);
    //points += offset;
    //apply_surface(pos.x, pos.y, text,background);

}

void init_spaceship(s_spaceship *s)
{
    SDL_Rect ship_rect;
    ship_rect.x = 10;
    ship_rect.y = 240;
    s->surf = load_image("../check/spaceship.bmp");
    s->rect = ship_rect;
}

void init_background(s_bg *background)
{
    background->surf = load_image("../check/background.bmp");
    background->x = 0;
    background->y = 0;
}

void handle_keys(const uint8_t *state, s_spaceship *s)
{
      if (state[SDLK_DOWN])
      {
        if (s->rect.y > (SCREEN_HEIGHT - SHIP_HEIGHT))
            s->rect.y = 400;
        s->rect.y += 2;
      }
      if (state[SDLK_UP])
      {
        if (s->rect.y > 15)
            s->rect.y -= 2;
      }
      if (state[SDLK_LEFT])
      {
        if (s->rect.x > 15)
            s->rect.x -= 2;
      }
      if (state[SDLK_RIGHT])
      {
        if (s->rect.x > (SCREEN_WIDTH - SHIP_WIDTH))
            s->rect.x = 520;
        s->rect.x += 2;
      }
}

int main(void)
{
    SDL_EnableKeyRepeat(100, 100);
    int quit = 0;
    SDL_Event event;
    SDL_Surface *screen = NULL;
    init(&screen);
    s_asteroid *list_asteroid = NULL;
    s_spaceship *spaceship = malloc(sizeof (s_spaceship));
    s_bg *backg = malloc(sizeof (s_bg));
    init_spaceship(spaceship);
    init_background(backg);
    const uint8_t *state = SDL_GetKeyState(NULL);

    while (!quit)
    {
        handle_keys(state, spaceship);
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                quit = 1;
        }
        draw(screen, spaceship, &list_asteroid, backg);
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
