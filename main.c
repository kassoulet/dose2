//3703


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "argb.h"
#include "line.h"
#include "m2d.h"
#include "layer.h"
#include "demo.h"
#include "ogdecode.h"
#include <math.h>
#include <SDL/SDL.h>

#ifdef OSX
#include <SDL/SDL_main.h>
#endif

#define HEAPSIZE 10000000

int WIDTH=640;
int HEIGHT=480;

char *moduleName="data/italo160.ogg";
volatile unsigned frame;
float fsin2(float);
int stopnow=0;
extern float con, bri;

/* Change this to standard fread() and behold: it's x86 byte order! */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
void fix_fread(void *dest,int dummy,int count,FILE *f)
{
    int n;
    unsigned char *d=dest;
    for(n=0;n<count;n++)
        d[3+n/4*4-n%4]=fgetc(f);
}
#else
void fix_fread(void *dest,int dummy,int count,FILE *f)
{
    fread(dest,dummy,count,f);
}
#endif

static unsigned hiippi[HEAPSIZE];
static unsigned *hptrs[4096], hpind=1;
void *getmem(int bytes)
{
    /* Let's keep it aligned */
    int wds=(bytes+4)/4;
    return (hptrs[hpind]+=wds)-wds;
}
void mark() { hptrs[hpind+1]=hptrs[hpind]; hpind++; }
void release() { hpind--; }



M2d *new_m2_roto(float x0, float y0, float r, float i) {
  M2d *m=getmem(sizeof(M2d));
  m->x1=x0; m->y1=y0;
  r=exp(r);
  m->xx=m->yy=cos(i)*r;
  m->xy=-(m->yx=sin(i)*r);
  return m;
}
M2d *new_m2_mul(M2d *a, M2d *b) {
  M2d *m=getmem(sizeof(M2d));
  m->xx=a->xx*b->xx+a->yx*b->xy; m->xy=a->xy*b->xx+a->yy*b->xy; m->x1=a->x1*b->xx+a->y1*b->xy+b->x1;
  m->yx=a->xx*b->yx+a->yx*b->yy; m->yy=a->xy*b->yx+a->yy*b->yy; m->y1=a->x1*b->yx+a->y1*b->yy+b->y1;
  return m;
}


int aikah=0;
void audioback(void *oggi, unsigned char *stream, int len) {
  int i, lkm, foo=0;
 
  if (stopnow) return;
  aikah+=len;
  while (len>0) {
    lkm=ov_read(oggi, stream, len, 0, 2, 1, &foo);
    if (lkm<=0) { stopnow++; return; }
    len-=lkm; stream+=lkm;
  }
 

}

void fillcopy(SDL_Surface *screen, Uint32* graffa, int fill)
{
  int x, y;
 
  int *p32=screen->pixels, *q32=graffa;
  char *q8=(char*)graffa;
  int nextline=0;
  if (fill) {
    nextline = WIDTH/4;
  }
  for (x=0; x<screen->w; x++) q8[x]^=q8[x-1];
  for (y=0; y<screen->h-1; y++) {
    for (x=0; x<screen->w>>2; x++) {
      p32[x]=q32[x];
      q32[x+nextline]^=q32[x];
    }
    p32+=screen->pitch>>2;
    q32+=WIDTH/4;
  }
}



void fla(int a) {}
int main(int argc, char *argv[]) {
  int i, j, bytesleft;
  int current_section,fullscreen=0;
  int fill=1;
  int skip=0;
  float t;
  static char buffer[200000];
  FILE *fp;
  OggVorbis_File oggi;
  vorbis_info *vi;
  int bufsiz;
  unsigned time0, timex;
  SDL_Surface *screen;
  SDL_Color colors[256];
  static Uint32* graffa;
  static SDL_AudioSpec aanispex;
  int frames = 0;

  hptrs[0]=hptrs[1]=hiippi;
  mark();

  printf("dose2 by mfx [-WIDTHxHEIGHT -fullscreen -stretch -nofill] \n");

// This one moved here because of OSX oddities...
  if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)<0) {
    fprintf(stderr, "sdlerror %s\n", SDL_GetError());
    return 1;
  }

  fp=fopen(moduleName, "rb");
  if (!fp || ov_open(fp, &oggi, 0, 0)<0) {
    fprintf(stderr, "error in %s", moduleName);
    return 1;
  }
  
  vi=ov_info(&oggi, -1);
  fprintf(stderr,"\n%dchn/%ldHz\n",vi->channels,vi->rate);

  atexit(SDL_Quit);

  aanispex.freq=44100;
  aanispex.format=AUDIO_S16;
  aanispex.channels=2;
  aanispex.samples=4096;
  aanispex.callback=audioback;
  aanispex.userdata=&oggi;

  if (SDL_OpenAudio(&aanispex, 0)<0) {
    fprintf(stderr, "SDL Audio Error: %s\n", SDL_GetError());
  }

  const SDL_VideoInfo *info;
  info = SDL_GetVideoInfo();
  printf("screen: %dx%d\n", info->current_w, info->current_h);

  {
    int i,n,w,h;
    char *p;
    for(i=1; i<argc; i++) {
      p = argv[i];
      while(*p == '-') p++;

      if(strcmp(p, "fullscreen") == 0) {
        fullscreen=1;
      }
      if(strcmp(p, "stretch") == 0) {
        fullscreen=1;
        WIDTH = info->current_w;
        HEIGHT = info->current_h;
      }
      if(strcmp(p, "nofill") == 0) {
        fill=0;
      }
      
      n = sscanf(p, "%dx%d", &w, &h);
      if(n==2) {
        WIDTH = w;
        HEIGHT = h;
      }
    }  
  }

#ifdef OSX
  /* On OSX we have two different applets with separate names. The one
     ending with 'w' is windowed */
  if(argv[0][strlen(argv[0])-1]!='w')
    fullscreen=1;
#endif

  printf("will display in: %dx%d %s\n", WIDTH, HEIGHT, fullscreen ? "fullscreen" : "windowed");
  graffa = malloc(WIDTH*HEIGHT);

  if(fullscreen) {
    screen=SDL_SetVideoMode(WIDTH, HEIGHT, 8, SDL_SWSURFACE|SDL_FULLSCREEN);//|SDL_DOUBLEBUF);
    SDL_ShowCursor(0);
  } else {
    screen=SDL_SetVideoMode(WIDTH, HEIGHT, 8, SDL_SWSURFACE);//|SDL_DOUBLEBUF);
  }
  SDL_WM_SetCaption("dose 2 by mfx",NULL);

  t=0;
  bytesleft=0;

  initdemo();
  time0=SDL_GetTicks();
  SDL_PauseAudio(0);
  SDL_EventState(SDL_KEYDOWN, SDL_ENABLE);
  SDL_EventState(SDL_QUIT, SDL_ENABLE);

  while (!stopnow) {
    float aikaero=.02;
    SDL_Event eve;
    timex=SDL_GetTicks()-time0;

    //if (timex> 20*1000)
    //  stopnow++; 

    while (SDL_PollEvent(&eve))
    {
      if (eve.type==SDL_KEYDOWN||eve.type==SDL_QUIT) stopnow++;
#ifdef OSX
      /* On Mac it's normal to exit with mouse press */
      if(eve.type==SDL_MOUSEBUTTONDOWN) stopnow++;
#endif
    }
    if (SDL_LockSurface(screen)<0) continue;


    mark();
    con=1, bri=0;

    init_layers((char*)graffa, new_col(0, 0, 0, 0));
    memset(graffa, 0, WIDTH*HEIGHT);


    {
    float f=(timex/1000.0/60.0)*132.3412*4;
    rundemo(f);
    }

    fillcopy(screen, graffa, fill);
    
    {
      char *p=teepal1();
      static SDL_Color pp[256];
      for (i=0; i<256; i++) pp[i].r=p[i*3], pp[i].g=p[i*3+1], pp[i].b=p[i*3+2];
      SDL_SetColors(screen, pp, 0, 256);
    }
 
    SDL_UnlockSurface(screen);
    SDL_Flip(screen);
    
    frames++;
    release();
  }

  printf("fps:%d\n", 1000*frames/timex);

  //fclose(fp);
  ov_clear(&oggi);
  SDL_CloseAudio();
  return 0;
}
