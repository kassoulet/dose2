#include <math.h>
#include "schaibe.h"
//#include <lib2\asm.h>
//#include <lib2\fasm.h>
//#include <lib2\mem.h>
#include "argb.h"
#include "palette.h"


struct _Palette {
  int size;
  Argb cols[256];
};
Palette *new_pal() { Palette *p=getmem(sizeof(Palette)); p->size=0; return p; }
Palette *new_pal1(Argb *c1) { Palette *p=new_pal(); pal_addcol(p, c1); return p; }
Palette *new_pal2(Argb *c1, Argb *c2) { Palette *p=new_pal(); pal_addcol(p, c1); pal_addcol(p, c2); return p; }
Palette *new_pal3(Argb *c1, Argb *c2, Argb *c3) { Palette *p=new_pal(); pal_addcol(p, c1); pal_addcol(p, c2); pal_addcol(p, c3); return p; }
Palette *new_pal4(Argb *c1, Argb *c2, Argb *c3, Argb *c4) { Palette *p=new_pal(); pal_addcol(p, c1); pal_addcol(p, c2); pal_addcol(p, c3); pal_addcol(p, c4); return p; }
Palette *new_pal8(Argb *c1, Argb *c2, Argb *c3, Argb *c4, Argb *c5, Argb *c6, Argb *c7, Argb *c8) { Palette *p=new_pal(); pal_addcol(p, c1); pal_addcol(p, c2); pal_addcol(p, c3); pal_addcol(p, c4); pal_addcol(p, c5); pal_addcol(p, c6); pal_addcol(p, c7); pal_addcol(p, c8); return p; }
Palette *new_pals2(int n, Argb *c1, Argb *c2) {
  float f, fi=1.0/(n-1); int i;
  Palette *p=new_pal(); p->size=n;
  for (i=0, f=0; i<n; i++, f+=fi) {
    p->cols[i].a=c1->a+f*(c2->a-c1->a);
    p->cols[i].r=c1->r+f*(c2->r-c1->r);
    p->cols[i].g=c1->g+f*(c2->g-c1->g);
    p->cols[i].b=c1->b+f*(c2->b-c1->b);
  }
  return p;
}
Palette *new_pals3(Argb *c0, int n, Argb *c1, Argb *c2) {
  float f, fi=1.0/(n-1); int i;
  Palette *p=new_pal(); p->size=n+1;
  p->cols[0]=*c0;
  for (i=0, f=0; i<n; i++, f+=fi) {
    p->cols[i+1].a=c1->a+f*(c2->a-c1->a);
    p->cols[i+1].r=c1->r+f*(c2->r-c1->r);
    p->cols[i+1].g=c1->g+f*(c2->g-c1->g);
    p->cols[i+1].b=c1->b+f*(c2->b-c1->b);
  }
  return p;
}
void pal_addcol(Palette *p, Argb *c) { if (p->size<256) p->cols[p->size++]=*c; }
void pal_set(Palette *p, int i, Argb *c) { if (i<256 && i>=0) { p->cols[i]=*c; if (i>=p->size) p->size=i+1; } }
int pal_getsize(Palette *p) { return p->size; }
Argb *pal_getcol(Palette *p, int ind) { return p->cols+ind; }
void pal_rotate(Palette *p, float a, float r) {
  float s=sin(a)*r, c=cos(a)*r, x, y, z, x2, y2, k;
  int i;
  for (i=0; i<p->size; i++) {
    x = -sqrt(1.0/2)*p->cols[i].r +                    0    +sqrt(1.0/2)*p->cols[i].b;
    y = -sqrt(1.0/6)*p->cols[i].r +sqrt(2.0/3)*p->cols[i].g -sqrt(1.0/6)*p->cols[i].b;
    z = +sqrt(1.0/3)*p->cols[i].r +sqrt(1.0/3)*p->cols[i].g +sqrt(1.0/3)*p->cols[i].b;
    x2=x*c-y*s; y2=y*c+x*s;
    p->cols[i].r = -sqrt(1.0/2)*x2 -sqrt(1.0/6)*y2 +sqrt(1.0/3)*z;
    p->cols[i].g = +         0     +sqrt(2.0/3)*y2 +sqrt(1.0/3)*z;
    p->cols[i].b = +sqrt(1.0/2)*x2 -sqrt(1.0/6)*y2 +sqrt(1.0/3)*z;
  }
}
Palette *pal_reverse(Palette *p) {
  Argb tmp;
  int i;
  for (i=0; i<p->size-1-i; i++)
    tmp=p->cols[p->size-1-i], p->cols[p->size-1-i]=p->cols[i], p->cols[i]=tmp;
  return p;
}
Palette *pal_alpha(Palette *p, float a) {
  int i;
  for (i=0; i<p->size; i++) p->cols[i].a=a;
  return p;
}
Palette *pal_alpha2(Palette *p, float a) {
  int i;
  for (i=0; i<p->size; i++) p->cols[i].r*=a, p->cols[i].g*=a, p->cols[i].b*=a, p->cols[i].a*=a;
  return p;
}
void pal_product(Palette *p, Palette *q) {
  int i, j, c;
  Palette *t;
  mark();
  t=new_pal(); for (i=0; i<p->size; i++) pal_addcol(t, p->cols+i);
  for (j=0, c=0; j<q->size; j++) for (i=0; i<p->size && c<256; i++, c++)
    col_mix(p->cols+c, t->cols+i, q->cols+j);
  p->size=c;
  release();
}



