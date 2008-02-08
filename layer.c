#include "schaibe.h"
//#include <lib2\mem.h>
//#include <lib2\asm.h>
//#include <lib2\fasm.h>
#include <math.h>
#include "argb.h"
//#include "palette.h"
#include "layer.h"

static struct {
  Palette *pal;
  int pp;
  char *vm;
  Layer *llst[32];
  Layer **lp;
} kb_data;
void init_layers(char *vm, Argb *bg) {
  kb_data.pal=new_pal1(bg);
  kb_data.pp=0;
  kb_data.vm=vm;
  kb_data.lp=kb_data.llst;
}
float con=1, bri=0;
static char paljetti[768];
char *teepal1() {
  Layer **l, *ll;
  int p, i, j;
  static char *tp;
  static char gonv[1024];
  if (!gonv[1023]) {
    for (i=1; i<1024; i++) gonv[i]=pow(i*(1/1023.), .5)*255.4;
  }
  for (tp=paljetti, i=0; i<256; i++) {
    Argb *c=pal_getcol(kb_data.pal, i);
    float r=c->r*con+bri, g=c->g*con+bri, b=c->b*con+bri;
    *tp++=gonv[r<0?0:r>1?1023:(int)(r*1023.4)];
    *tp++=gonv[g<0?0:g>1?1023:(int)(g*1023.4)];
    *tp++=gonv[b<0?0:b>1?1023:(int)(b*1023.4)];
  }
  return paljetti;
}
void teepal2() {
//  outb(0x3c8, 0); outsb(0x3c9, paljetti, 768);
}
int getp() { return kb_data.pp; }
Layer *new_layer(Palette *p) {
  int i;
  Layer *l=*kb_data.lp++=getmem(sizeof(Layer));
  l->shift=kb_data.pp;
  l->lkm=pal_getsize(p);
  l->vmem=kb_data.vm;
  pal_product(kb_data.pal, p);
  for (i=1, l->size=0; i<pal_getsize(p); i+=i) l->size++;
  kb_data.pp+=l->size;
  return l;
}



