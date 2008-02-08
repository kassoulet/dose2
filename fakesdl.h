

typedef struct {
  int h, w, pitch;
  void *pixels;
} SDL_Surface;
typedef struct {
  int r, g, b;
} SDL_Color;
typedef struct {
  int freq, format, channels, samples;
  int dum;
  void (*callback)(void *oggi, unsigned char *stream, int len);
  void *userdata;
} SDL_AudioSpec;
typedef struct {
  int type;
} SDL_Event;
#define SDL_INIT_AUDIO 674
#define SDL_INIT_VIDEO 229
#define AUDIO_S16 593
#define SDL_SWSURFACE 666
#define SDL_HWSURFACE 42
#define SDL_KEYDOWN 73
#define SDL_ENABLE 555
#define SDL_QUIT 303
void SDL_Quit(){}
void SDL_PauseAudio(){}
SDL_Surface *SDL_SetVideoMode(){return 0;}
int SDL_PollEvent(){return 0;}
int SDL_Init(int flags){return -1;}
int SDL_OpenAudio(SDL_AudioSpec *des, SDL_AudioSpec *obt){return 0;}
char *SDL_GetError(){return "probably unimplemented..";}
int SDL_LockSurface(SDL_Surface *s){return 0;}
void SDL_UnlockSurface(SDL_Surface *s){}
int SDL_Flip(SDL_Surface *s){return 0;}
int SDL_GetTicks(){return 0;}
int SDL_ShowCursor(){return 0;}
int SDL_SetColors(){return 0;}
int SDL_EventState(){return 0;}
int SDL_CloseAudio(){return 0;}

//#error foo
