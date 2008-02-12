#include "demo.h"

#include "schaibe.h"
//#include <lib2\mem.h>
//#include <lib2\asm.h>
//#include <lib2\fasm.h>
#include "line.h"
#include <sys/types.h>



Piste *new_piste(int x, int y) {
  Piste *p=getmem(sizeof(Piste));
  p->x=x; p->y=y; p->code=0; return p;
}

void line(char *d, Piste *p1, Piste *p2, int c) {
  int x, y, kk;
  Piste *tmp;
  if (p2->x<p1->x) tmp=p1, p1=p2, p2=tmp;
/*  if (p2->x>WIDTH*4096) return;
  if (p2->y>HEIGHT*4096) return;
  if (p1->x>WIDTH*4096) return;
  if (p1->y>HEIGHT*4096) return;*/
  kk=(p2->y-p1->y)*4096./(p2->x-p1->x);
  if (p1->x<0) p1->x=0;
  if (p2->x>WIDTH*4096) p2->x=WIDTH*4096;
  y=p1->y+((4096-(p1->x&4095))*kk>>12);
  for (x=p1->x>>12; x<p2->x>>12; x++) {
    if (y>=4096 && y<HEIGHT*4096) d[(y>>12)*WIDTH+x]^=c;
    y+=kk;
  }  
}

void clip1(Piste *p1, Piste *p2, float a, float b, float c) {
  float t=-(p1->x*a+p1->y*b+c)/((p2->y-p1->y)*b+(p2->x-p1->x)*a);
  t=t<0?0:t>1?1:t;
  if (p1->x*a+p1->y*b+c<0)
    p1->x=p1->x+t*(p2->x-p1->x), p1->y=p1->y+t*(p2->y-p1->y);
  if (p2->x*a+p2->y*b+c<0)
    p2->x=p1->x+t*(p2->x-p1->x), p2->y=p1->y+t*(p2->y-p1->y);
}
void lineclip(char *d, Piste *q1, Piste *q2, int c) {
  Piste *p1=getmem(sizeof(Piste)), *p2=getmem(sizeof(Piste));
  p1->x=q1->x; p1->y=q1->y; p2->x=q2->x; p2->y=q2->y;
  clip1(p1, p2, 1,  0,         -1024);
  clip1(p1, p2, -1, 0, WIDTH*4096-1024);
  clip1(p1, p2, 0,  1,  -1*4096-1024);
  clip1(p1, p2, 0, -1, HEIGHT*4096-1024);
  if (q1->code&2&l_mask(p1->x, WIDTH*4096)) d[g_mask(p1->x, 0)&p1->x>>12]^=c;
  if (q2->code&2&l_mask(p2->x, WIDTH*4096)) d[g_mask(p2->x, 0)&p2->x>>12]^=c;
  if ( l_mask(p1->x,        0)| l_mask(p1->y,   1*4096)
     |nl_mask(p1->x, WIDTH*4096)|nl_mask(p1->y, HEIGHT*4096)
     | l_mask(p2->x,        0)| l_mask(p2->y,   1*4096)
     |nl_mask(p2->x, WIDTH*4096)|nl_mask(p2->y, HEIGHT*4096)) return;
  line(d, p1, p2, c);
}


