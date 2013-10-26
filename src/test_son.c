#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int continuer = 1;
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  SDL_Surface *ecran = NULL;
  ecran = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
  SDL_Event event;
  SDL_WM_SetCaption("SDL_Mixer", NULL);

  int audio_rate = 22050;
  Uint16 audio_format = AUDIO_S16; /* 16-bit stereo */
  int audio_channels = 2;
  int audio_buffers = 4096;

  Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
  if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers)) {
    printf("Unable to open audio!\n");
    exit(1);
  }

  Mix_Music *music;
  music = Mix_LoadMUS("beep-1.wav");
  Mix_PlayMusic(music, -1);

  while(continuer)
  {
    while (SDL_PollEvent(&event))
    {
      switch(event.type)
      {
        case SDL_QUIT:
          continuer = 0;
          break;
      }
    }
    SDL_Delay(50);
  SDL_Flip(ecran);
  }


  SDL_Quit();
  return EXIT_SUCCESS;
}
