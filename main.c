//3703

/* Profile info, in 1024x768
 * 25% vorbis decoding
 * 35% blitting
 * 15% rundemo
 * 20% fillcopy
 * */



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

int WIDTH=1440*2;
int HEIGHT=1080*2;

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
  for (x=1; x<screen->w; x++) q8[x]^=q8[x-1];
  for (y=0; y<screen->h-1; y++) {
    for (x=0; x<screen->w>>2; x++) {
      p32[x]=q32[x];
      q32[x+nextline]^=q32[x];
    }
    p32+=screen->pitch>>2;
    q32+=WIDTH/4;
  }
}

#define OVERFRAMES 20
unsigned char * framesave;
unsigned char palettesave[768*OVERFRAMES];
int pos=0;

int FRAME = 9000;
int ENDFRAME = 12000*OVERFRAMES;

void saveframe()
{
  int i, j, x, y;

  FILE *dst;

  char fname[1024];
  sprintf(fname, "dosedump20x/dose2-dump-%08d.ppm.png", FRAME);
  dst = fopen(fname, "r");
  if (dst) {
    FRAME++;
    return;
  }  
  dst = fopen(fname, "w");
  fclose(dst);

  sprintf(fname, "dosedump20x/dose2-dump-%08d.ppm", FRAME++);
  dst = fopen(fname, "w");
  fprintf(dst, "P3 %d %d 255 \n", WIDTH, HEIGHT);

  int linear[256];
    for (j=0; j<256; j++) {
      linear[j] = 1023.0f * powf(j/255.0f, 2.2f);
    }
  int gamma[1024];
    for (j=0; j<1024; j++) {
      gamma[j] = 255.0f * powf(j/1023.0f, 0.4545f);
    }

  
  int p=0;
  for (y=0; y<HEIGHT; y++) {
    for (x=0; x<WIDTH; x++) {
      //float c[3]= {0};
      int c[3]= {0};
      for (i=0; i<OVERFRAMES; i++) {
        unsigned char color = framesave[WIDTH*HEIGHT* i + p];
        for (j=0; j<3; j++) {
          //c[j] += powf(palettesave[768 * i + 3*color + j]/255.0, 2.2);
          //c[j] += palettesave[768 * i + 3*color + j];
          c[j] += linear[palettesave[768 * i + 3*color + j]];
        }
      }
      
      for (j=0; j<3; j++) {
        //fprintf(dst, "%d ", (int) ( 255.0 * powf(c[j] / OVERFRAMES, 0.45)));
        //fprintf(dst, "%d ", c[j] / OVERFRAMES);
        fprintf(dst, "%d ", gamma[c[j] / OVERFRAMES]);
      }
      p++;
    }
    fprintf(dst, "\n");
  }
  fclose(dst);

    char cmd[10000];
    sprintf(cmd, "{ convert %s -resize 50%% %s.png ; rm %s; } &", fname, fname, fname);
    system(cmd);
}


void storeframe(char * graffa)
{
  char *palette=teepal1();
  memcpy(framesave + WIDTH*HEIGHT*pos, graffa, WIDTH*HEIGHT);
  memcpy(palettesave + 768*pos, palette, 768);
  
  pos++;
  pos %= OVERFRAMES;
  
  if (pos == 0) {
    saveframe();
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

  framesave = malloc(OVERFRAMES*WIDTH*HEIGHT); //DUMP

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
  //SDL_PauseAudio(0); //DUMP
  SDL_EventState(SDL_KEYDOWN, SDL_ENABLE);
  SDL_EventState(SDL_QUIT, SDL_ENABLE);

  timex=FRAME*OVERFRAMES;

  while (!stopnow) {
    float aikaero=.02;
    SDL_Event eve;
    //timex=SDL_GetTicks()-time0; //DUMP

    timex += 20/OVERFRAMES; //DUMP

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
      SDL_SetColors(screen, pp, 0, 256); // this force a blit !!
      //SDL_SetPalette(screen, SDL_PHYSPAL, pp, 0, 256);
    }

    SDL_UnlockSurface(screen);
    //SDL_Flip(screen);

    storeframe((char *)graffa);

    frames++;
    
    if (frames > ENDFRAME) {
      stopnow = 1;
    }
    
    release();
  }

  printf("fps:%d\n", 1000*frames/timex);

  //fclose(fp);
  ov_clear(&oggi);
  SDL_CloseAudio();
  return 0;
}
