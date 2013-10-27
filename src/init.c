#include "init.h"

long long int timer;
int high_scores[5];
int hscores_len = 0;
s_spaceship *spaceship2 = NULL;
Mix_Chunk *musica[10];

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
    SDL_WM_SetCaption("\\o/ EpiRun \\o/", NULL);
    return EXIT_SUCCESS;
}

int scroll(s_bg *backg, SDL_Surface *screen, s_spaceship *s)
{
    int offset = timer / 300 - (s->coll);
    if (offset < 1)
        offset = 1;
    if (offset > 30)
        offset = 30;
    backg->x -= 1 * offset;
    if (backg->x <= -backg->surf->w)
        backg->x = 0;
    if (s->invincible > 0)
        s->invincible--;
    if (spaceship2 && spaceship2->invincible > 0)
        spaceship2->invincible--;
    apply_surface(backg->x, backg->y, backg->surf, screen);
    apply_surface(backg->x + backg->surf->w, backg->y, backg->surf, screen);
    return offset;
}

s_asteroid *collision(s_spaceship *s, s_asteroid *list)
{
    s_asteroid *tmp = list;
    while (tmp)
    {
        if (((tmp->posx + 5) > (s->rect.x)
                    && tmp->posx + 5 < (s->rect.x + s->surf->w)
                    && tmp->posy + tmp->surf->h - 5 > (s->rect.y)
                    && tmp->posy + tmp->surf->h - 5 < (s->rect.y + s->surf->h))
                || (tmp->posx + tmp->surf->w - 5 > (s->rect.x)
                    && tmp->posx + tmp->surf->w - 5< (s->rect.x + s->surf->w)
                    && tmp->posy + 5 > (s->rect.y)
                    && tmp->posy + 5 < (s->rect.y + s->surf->h))
                || ((tmp->posx) + tmp->surf->w - 5 > (s->rect.x)
                    && tmp->posx + tmp->surf->w - 5 < (s->rect.x + s->surf->w)
                    && tmp->posy + tmp->surf->h - 5 > (s->rect.y)
                    && tmp->posy + tmp->surf->h - 5 < (s->rect.y + s->surf->h))
                || ((tmp->posx + 5 ) > (s->rect.x)
                    && tmp->posx + 5 < (s->rect.x + s->surf->w)
                    && tmp->posy + 5 > (s->rect.y)
                    && tmp->posy + 5 < (s->rect.y + s->surf->h)))
            return tmp;
        tmp = tmp->prev;
    }
    return NULL;
}

static void explode(s_asteroid **listaste, s_spaceship *s)
{
    s_asteroid *tmp = *listaste;
    while (tmp)
    {
        tmp->posx = -110;
        s->points += tmp->type * 10;
        tmp = tmp->prev;
    }

}

void treat_collision(s_asteroid *elt, s_spaceship *s, s_asteroid **listelt)
{
    switch (elt->type)
    {
        case 1:
        case 2:
            if (s->invincible < 1)
            {
                s->coll++;
                s->life--;
                if (s->life > 0)
                {
                    Mix_VolumeChunk(musica[2], MIX_MAX_VOLUME / 2);
                    Mix_PlayChannel(2, musica[2], 0);
                    s->invincible = 50;
                }
                else
                {
                    Mix_VolumeChunk(musica[3], MIX_MAX_VOLUME / 2);
                    Mix_PlayChannel(9, musica[3], 0);
                }
            }
            else
            {
                s->points += 100;
                Mix_VolumeChunk(musica[1], MIX_MAX_VOLUME / 2);
                Mix_PlayChannel(6, musica[1], 0);
            }
            break;
        case 3:
            if (1)
            {
                Mix_VolumeChunk(musica[6], MIX_MAX_VOLUME / 2);
                Mix_PlayChannel(3, musica[6], 0);
                s->points += 1000;
            }
            break;
        case 4:
            if (1)
            {
                Mix_VolumeChunk(musica[4], MIX_MAX_VOLUME / 8);
                Mix_PlayChannel(4, musica[4], 0);
                s->life++;
            }
            break;
        case 5:
            if (1)
            {
                Mix_VolumeChunk(musica[5], MIX_MAX_VOLUME / 8);
                Mix_PlayChannel(5, musica[5], 0);
                s->invincible = 250;
            }
            break;
        case 6:
            if (1)
            {
                Mix_VolumeChunk(musica[3], MIX_MAX_VOLUME / 8);
                Mix_PlayChannel(8, musica[3], 0);
                explode(listelt, s);
            }
        default:
            break;
    }
    elt->posx = -110;
}


TTF_Font *police;
void score(s_spaceship *s, int offset, SDL_Surface **screen)
{
    SDL_Color white = {255, 255, 255, 255};
    char score[100];
    s->points += offset;
    sprintf(score, "score : %d", s->points);
    SDL_Surface *text = TTF_RenderText_Blended(police, score, white);
    apply_surface(0, 0, text, *screen);
    char life[50];
    sprintf(life, "life : %d", s->life);
    text = TTF_RenderText_Blended(police, life, white);
    apply_surface(0, 50, text, *screen);
    if (spaceship2)
    {
        char score2[100];
        spaceship2->points += offset;
        sprintf(score2, "score : %d", spaceship2->points);
        text = TTF_RenderText_Blended(police, score2, white);
        apply_surface(SCREEN_WIDTH - 250, 0, text, *screen);
        char life2[50];
        sprintf(life2, "life : %d", spaceship2->life);
        text = TTF_RenderText_Blended(police, life2, white);
        apply_surface(SCREEN_WIDTH - 150, 50, text, *screen);
    }
    SDL_FreeSurface(text);
}

void draw(SDL_Surface *screen, s_spaceship *s, s_asteroid **list_asteroid,
        s_bg *backg)
{
    apply_surface(0, 0, backg->surf, screen);
    int offset = scroll(backg, screen, s);
    *list_asteroid = addelt(*list_asteroid, offset);
    drawelt(&screen, *list_asteroid);
    apply_surface(s->rect.x, s->rect.y, s->surf, screen);
    if (s->invincible > 50 || (s->invincible > 0 && timer % 4 == 0))
        apply_surface(s->rect.x - 10, s->rect.y - 10, s->bou, screen);
    if (spaceship2)
    {
        apply_surface(spaceship2->rect.x, spaceship2->rect.y, spaceship2->surf,
                screen);
        if (spaceship2->invincible > 50 || (spaceship2->invincible > 0
                    && timer % 4 == 0))
            apply_surface(spaceship2->rect.x - 10, spaceship2->rect.y - 10,
                    spaceship2->bou, screen);
    }
    score(s, offset, &screen);
    SDL_Flip(screen);
}

void init_spaceship(s_spaceship *s)
{
    s->points = 0;
    s->life = 3;
    s->coll = 0;
    SDL_Rect ship_rect;
    ship_rect.x = 20;
    ship_rect.y = 240;
    s->bou = IMG_Load("assets/shieldbig.png");
    SDL_SetColorKey(s->bou, SDL_SRCCOLORKEY, SDL_MapRGB(s->bou->format, 
                0, 0, 0));
    SDL_SetAlpha(s->bou, SDL_SRCALPHA, 128);
    s->surf = load_image("assets/spaceship.bmp");
    SDL_SetColorKey(s->surf, SDL_SRCCOLORKEY, SDL_MapRGB(s->surf->format,
                0, 0, 0));
    s->rect = ship_rect;
}

void init_background(s_bg *background)
{
    background->surf = IMG_Load("assets/background2.png");
    background->x = 0;
    background->y = 0;
}

void handle_keys(const uint8_t *state, s_spaceship *s)
{
    if (state[SDLK_DOWN])
    {
        if (s->rect.y > (SCREEN_HEIGHT - SHIP_HEIGHT))
            s->rect.y = SCREEN_HEIGHT - SHIP_HEIGHT;
        s->rect.y += 4;
    }
    if (state[SDLK_UP])
    {
        if (s->rect.y > 15)
            s->rect.y -= 4;
    }
    if (state[SDLK_LEFT])
    {
        if (s->rect.x > 15)
            s->rect.x -= 4;
    }
    if (state[SDLK_RIGHT])
    {
        if (s->rect.x > (SCREEN_WIDTH - SHIP_WIDTH))
            s->rect.x = SCREEN_WIDTH - SHIP_WIDTH;
        s->rect.x += 4;
    }

    if (spaceship2)
    {
        if (state[SDLK_s])
        {
            if (spaceship2->rect.y > (SCREEN_HEIGHT - SHIP_HEIGHT))
                spaceship2->rect.y = SCREEN_HEIGHT - SHIP_HEIGHT;
            spaceship2->rect.y += 4;
        }
        if (state[SDLK_w])
        {
            if (spaceship2->rect.y > 15)
                spaceship2->rect.y -= 4;
        }
        if (state[SDLK_a])
        {
            if (spaceship2->rect.x > 15)
                spaceship2->rect.x -= 4;
        }
        if (state[SDLK_d])
        {
            if (spaceship2->rect.x > (SCREEN_WIDTH - SHIP_WIDTH))
                spaceship2->rect.x =  SCREEN_WIDTH - SHIP_WIDTH;
            spaceship2->rect.x += 4;
        }

    }
}

void add_hscores(s_spaceship *s, FILE *f)
{
    if (hscores_len < 5)
    {
        high_scores[hscores_len++] = s->points;
        fprintf(f, "%d\n", s->points);
    }
    else
    {
        for (int i = 0; i < 5; i++)
        {
            if (s->points > high_scores[i])
            {
                high_scores[i] = s->points;
                fprintf(f, "%d\n", s->points);
            }
        }
    }
}

void display_hscores(SDL_Surface **screen, FILE *f)
{
    SDL_Color white = {255, 255, 255, 255};
    s_bg *backg = malloc(sizeof (s_bg));
    init_background(backg);
    apply_surface(0, 0, backg->surf, *screen);
    TTF_Font *police = TTF_OpenFont("check/leadcoat.ttf", 20);
    SDL_Rect pos_score;
    char **scores = malloc(sizeof (char *) * 5);
    int j = 0;
    for (int i = 0; i < 5; i++)
    {
        char line[20];
        if (i == 0)
            fseek(f, 0, SEEK_SET);
        else
            fseek(f, 0, SEEK_CUR);
        if (fgets(line, 20, f))
        {
            scores[j] = malloc(sizeof (char) * strlen(line));
            strcpy(scores[j], line);
            scores[j++][strlen(line) - 1] = '\0';
        }
    }
    SDL_Surface *s_scores[j];
    for (int i = 0; i < j; i++)
    {
        s_scores[i] = TTF_RenderText_Blended(police, scores[i], white);
        pos_score.x = (SCREEN_WIDTH - s_scores[i]->w) / 2;
        pos_score.y = SCREEN_HEIGHT / 2 + 40 * (i - 1);
        SDL_BlitSurface(s_scores[i], NULL, *screen, &pos_score);
    }
    SDL_Flip(*screen);
    SDL_Delay(5000);
}

void gameover(SDL_Surface **screen, s_spaceship *s)
{
    SDL_Color white = {255, 255, 255, 255};
    s_bg *backg = malloc(sizeof (s_bg));
    init_background(backg);
    apply_surface(0, 0, backg->surf, *screen);
    TTF_Font *police = TTF_OpenFont("assets/leadcoat.ttf", 50);
    char score[100];
    if (spaceship2)
        sprintf(score, "score J1 : %d - score J2: %d", s->points,
                spaceship2->points);
    else
        sprintf(score, "score : %d", s->points);
    SDL_Surface *text = TTF_RenderText_Blended(police, score, white);
    apply_surface(SCREEN_WIDTH / 2 - text->w / 2, SCREEN_HEIGHT / 2 + 50, text, *screen);
    char over[25];
    sprintf(over, "GAME OVER");
    text = TTF_RenderText_Blended(police, over, white);
    apply_surface(SCREEN_WIDTH / 2 - text->w / 2, SCREEN_HEIGHT / 2 - 50, text, *screen);
    TTF_Font *cont = TTF_OpenFont("assets/leadcoat.ttf", 30);
    char todo[30];
    sprintf(todo, "press enter to restart");
    text = TTF_RenderText_Blended(cont, todo, white);
    apply_surface(SCREEN_WIDTH - text->w, SCREEN_HEIGHT - text->h, text, *screen);
    SDL_Flip(*screen);
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
    Mix_AllocateChannels(10);

    musica[0] = Mix_LoadWAV("assets/sw.wav");
    musica[1] = Mix_LoadWAV("assets/bzi.wav");
    musica[2] = Mix_LoadWAV("assets/crash.wav");
    musica[3] = Mix_LoadWAV("assets/explosion.wav");
    musica[4] = Mix_LoadWAV("assets/pop.wav");
    musica[5] = Mix_LoadWAV("assets/shield.WAV");
    musica[6] = Mix_LoadWAV("assets/splash.wav");
    Mix_VolumeChunk(musica[0], MIX_MAX_VOLUME / 4);
    Mix_PlayChannel(1, musica[0], -1);
}

int main(void)
{
    int quit = 0;
    timer = 0;
    SDL_Event event;
    SDL_Surface *screen = NULL;
    SDL_EnableKeyRepeat(100, 100);
    init(&screen);
    srand(time(NULL));
    TTF_Init();
    police = TTF_OpenFont("assets/leadcoat.ttf", 50);
    s_asteroid *list_asteroid = NULL;
    s_spaceship *spaceship = malloc(sizeof (s_spaceship));
    s_bg *backg = malloc(sizeof (s_bg));
    init_spaceship(spaceship);
    init_background(backg);
    music();
    const uint8_t *state = SDL_GetKeyState(NULL);
    int option = 1;
    FILE *f = fopen("save", "a+");

    display_menu(screen, &option, event);
    if (option == 4)
        quit = 1;
    else if (option == 3)
        display_hscores(&screen, f);
    else
    {
        while (!quit)
        {
            timer += 1;
            handle_keys(state, spaceship);
            while (SDL_PollEvent(&event))
                if (event.type == SDL_QUIT)
                    quit = 1;
            if (state[SDLK_ESCAPE])
                quit = 1;

            if (state[SDLK_F1] || option == 2)
            {
                if (!spaceship2)
                {
                    spaceship2 = malloc(sizeof (s_spaceship));
                    spaceship2->invincible = 0;
                    init_spaceship(spaceship2);
                    spaceship2->points = spaceship->points;
                    spaceship2->surf = IMG_Load("assets/spaceship_2.png");
                }
            }
            draw(screen, spaceship, &list_asteroid, backg);
            if (collision(spaceship, list_asteroid))
                treat_collision(collision(spaceship, list_asteroid), spaceship,
                        &list_asteroid);
            if (spaceship2)
                if (collision(spaceship2, list_asteroid))
                    treat_collision(collision(spaceship2, list_asteroid), spaceship2,
                            &list_asteroid);
            if (spaceship->life <= 0 || (spaceship2 && spaceship2->life <= 0))
            {
                int quit2 = 0;
                if (spaceship2 && spaceship2->life <= 0)
                {
                    spaceship2->surf = IMG_Load("assets/explosion1.jpg");
                    SDL_SetColorKey(spaceship2->surf, SDL_SRCCOLORKEY, 
                            SDL_MapRGB(spaceship2->surf->format, 0, 0, 0));
                }
                spaceship->points += spaceship->points * spaceship->life * 0.15;
                if (spaceship2)
                    spaceship2->points += spaceship2->points * spaceship2->life * 0.15;
                if (spaceship->life <= 0)
                {
                    spaceship->surf = IMG_Load("assets/explosion1.jpg");
                    SDL_SetColorKey(spaceship->surf, SDL_SRCCOLORKEY, 
                            SDL_MapRGB(spaceship->surf->format, 0, 0, 0));
                }
                draw(screen, spaceship, &list_asteroid, backg);
                SDL_Delay(3000);
                gameover(&screen, spaceship);
                while (!quit2)
                {
                    if (state[SDLK_RETURN])
                    {
                        timer = 0;
                        free(backg);
                        free(spaceship);
                        spaceship2 = NULL;
                        s_bg *backg = malloc(sizeof (s_bg));
                        s_spaceship *spaceship = malloc(sizeof (s_spaceship));
                        init_spaceship(spaceship);
                        init_background(backg);
                        freelist(&list_asteroid);
                        quit2 = 1;
                    }
                    if (state[SDLK_ESCAPE])
                    {
                        quit = 1;
                        quit2 = 1;
                    }
                    while (SDL_PollEvent(&event))
                        if (event.type == SDL_QUIT)
                        {
                            quit2 = 1;
                            quit = 1;
                        }
                }
            }
    }
    free(spaceship);
    free(backg);
    free(list_asteroid);
    SDL_FreeSurface(screen);
    TTF_Quit();
    SDL_Quit();
    return 0;

}
SDL_Quit();
return 0;
}
