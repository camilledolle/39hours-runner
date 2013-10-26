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
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1)
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

int scroll(s_bg *backg, SDL_Surface *screen, s_spaceship *s)
{
    int offset = SDL_GetTicks() / 5000 + 1 - (s->coll * 3);
    if (offset < 1)
    {
        offset = 1;
        s->coll -= 1;
    }
    backg->x -= 1 * offset;
    if (backg->x <= -backg->surf->w)
        backg->x = 0;
    apply_surface(backg->x, backg->y, backg->surf, screen);
    apply_surface(backg->x + backg->surf->w, backg->y, backg->surf, screen);
    return offset;
}

s_asteroid *collision(s_spaceship *s, s_asteroid *list)
{
    s_asteroid *tmp = list;
    while (tmp)
    {
        if (((tmp->posx) > (s->rect.x)
            && tmp->posx < (s->rect.x + s->surf->w)
            && tmp->posy + tmp->surf->h > (s->rect.y)
            && tmp->posy + tmp->surf->h < (s->rect.y + s->surf->h))
        || (tmp->posx + tmp->surf->w > (s->rect.x)
            && tmp->posx + tmp->surf->w < (s->rect.x + s->surf->w)
            && tmp->posy > (s->rect.y)
            && tmp->posy < (s->rect.y + s->surf->h))
        || ((tmp->posx) + tmp->surf->w > (s->rect.x)
            && tmp->posx + tmp->surf->w < (s->rect.x + s->surf->w)
            && tmp->posy + tmp->surf->h > (s->rect.y)
            && tmp->posy + tmp->surf->h < (s->rect.y + s->surf->h))
        || ((tmp->posx) > (s->rect.x)
            && tmp->posx < (s->rect.x + s->surf->w)
            && tmp->posy > (s->rect.y)
            && tmp->posy < (s->rect.y + s->surf->h)))
            return tmp;
        tmp = tmp->prev;
    }
    return NULL;
}

void treat_collision(s_asteroid *elt, s_spaceship *s)
{
    switch (elt->type)
    {
        case 1:
        case 2:
            s->coll++;
            s->life--;
            break;
        case 3:
            s->points += 1000;
            break;
        case 4:
            s->life++;
            break;
        default:
            break;
    }
    elt->posx = -110;
}

SDL_Surface *score(s_spaceship *s, int offset, SDL_Surface **screen)
{
    SDL_Color white = {255, 255, 255};
    char score[100];
    TTF_Font *police = TTF_OpenFont("../check/leadcoat.ttf", 50);
    s->points += offset;
    sprintf(score, "score : %d", s->points);
    SDL_Surface *text = TTF_RenderText_Blended(police, score, white);
    apply_surface(0, 0, text, *screen);
    char life[50];
    sprintf(life, "life : %d", s->life);
    text = TTF_RenderText_Blended(police, life, white);
    apply_surface(0, 50, text, *screen);
    return *screen;
}

void draw(SDL_Surface *screen, s_spaceship *s, s_asteroid **list_asteroid,
        s_bg *backg)
{
    SDL_SetColorKey(s->surf, SDL_SRCCOLORKEY, SDL_MapRGB(s->surf->format,
                0, 0, 0));
    apply_surface(0, 0, backg->surf, screen);
    int offset = scroll(backg, screen, s);
    *list_asteroid = addelt(*list_asteroid, offset);
    drawelt(&screen, *list_asteroid);
    apply_surface(s->rect.x, s->rect.y, s->surf, screen);
    SDL_Surface *text = score(s, offset, &screen);
    SDL_Flip(screen);
    SDL_FreeSurface(text);
}

void init_spaceship(s_spaceship *s)
{
    s->points = 0;
    s->life = 3;
    s->coll = 0;
    SDL_Rect ship_rect;
    ship_rect.x = 10;
    ship_rect.y = 240;
    s->surf = load_image("../check/spaceship.bmp");
    s->rect = ship_rect;
    s->life = 3;
    s->coll = 0;
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

int gameover(SDL_Surface **screen, s_spaceship *s)
{
    SDL_Color white = {255, 255, 255};
    s_bg *backg = malloc(sizeof (s_bg));
    init_background(backg);
    apply_surface(0, 0, backg->surf, *screen);
    TTF_Font *police = TTF_OpenFont("../check/leadcoat.ttf", 50);
    char score[100];
    sprintf(score, "score : %d", s->points);
    SDL_Surface *text = TTF_RenderText_Blended(police, score, white);
    apply_surface(SCREEN_WIDTH / 2 - text->w / 2, SCREEN_HEIGHT / 2 + 50, text, *screen);
    char over[25];
    sprintf(over, "GAME OVER");
    text = TTF_RenderText_Blended(police, over, white);
    apply_surface(SCREEN_WIDTH / 2 - text->w / 2, SCREEN_HEIGHT / 2 - 50, text, *screen);
    SDL_Flip(*screen);
    SDL_Delay(5000);
    return 1;
}
void music()
{
  int audio_rate = 22050;
  Uint16 audio_format = AUDIO_S16; /* 16-bit stereo */
  int audio_channels = 2;
  int audio_buffers = 4096;

  Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
  if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers))
  {
      printf("Unable to open audio!\n");
  }

  Mix_Music *music;
  music = Mix_LoadMUS("../check/beep-1.wav");
  Mix_PlayMusic(music, -1);
}

int main(void)
{
    int quit = 0;
    SDL_Event event;
    SDL_Surface *screen = NULL;
    SDL_EnableKeyRepeat(100, 100);
    init(&screen);
    TTF_Init();
    s_asteroid *list_asteroid = NULL;
    s_spaceship *spaceship = malloc(sizeof (s_spaceship));
    s_bg *backg = malloc(sizeof (s_bg));
    init_spaceship(spaceship);
    init_background(backg);
    music();
    const uint8_t *state = SDL_GetKeyState(NULL);
    //uint32_t audioLen = 100;
    //uint32_t audioPos;
    //launch_audio("son.wav");
    while (!quit)
    {
        handle_keys(state, spaceship);
        while (SDL_PollEvent(&event))
            if (event.type == SDL_QUIT)
                quit = 1;
        draw(screen, spaceship, &list_asteroid, backg);
        if (collision(spaceship, list_asteroid))
            treat_collision(collision(spaceship, list_asteroid), spaceship);
        if (spaceship->life <= 0)
            quit = gameover(&screen, spaceship);
    }
    //SDL_Delay(3000);
    SDL_Quit();
    return 0;
}
