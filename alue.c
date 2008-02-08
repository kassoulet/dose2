#include "demo.h"

#include "schaibe.h"
#include <stdio.h>
//#include <lib2\mem.h>
//#include <lib2\fasm.h>
#include "m2d.h"
#include "taso.h"
#include "alue.h"

Alue *alue_lue(FILE *f) {
  Alue *a=getmem(sizeof(Alue));
  int i, j, xx, yy;
  fix_fread(&a->vc, 1, 4, f);
  a->vl=getmem(8*a->vc);
  fix_fread(a->vl, 1, 8*a->vc, f);

  {
    Alue *b=getmem(sizeof(Alue));
    b->vc=a->vc; b->vl=getmem(8*b->vc);
    for (i=0; i<b->vc; i++) b->vl[i]=a->vl[i];

    for (j=0, i=1; i<a->vc; i++) if (a->vl[i].y<a->vl[j].y) j=i;
    a->size=0.0;
    for (j=a->vc-1, i=0; i<a->vc; j=i++)
      a->size+=(a->vl[j].x-a->vl[i].x)*(a->vl[j].y+a->vl[i].y);
    a->size*=0.5;
    if (a->size<0)
      for (i=0; i<a->vc; i++) a->vl[i]=b->vl[(j-i)%b->vc], a->size=-a->size;
    else
      for (i=0; i<a->vc; i++) a->vl[i]=b->vl[(j+i)%b->vc];
    a->size*=((a->vl[0].x<0)^(a->vl[0].y<0)*3)+1;
    //xx=(int)((a->vl[0].x+1)*4+100)-100; yy=((a->vl[0].y+1)*4+100)-100;
    //a->size=((xx&1)^(yy&1)*3^(xx>>1&1)*7^(yy>>1&1)*15^(xx>>2&1)*31^(yy>>2&1)*63);
  }


  return a;
}
Alue *new_alue_morph(Alue *a, Alue *b, float v) {
  Alue *d=getmem(sizeof(Alue));
  int i, j, k;
  if (a->vc<b->vc) { Alue *t=b; b=a; a=t; v=1-v; }
  d->vc=a->vc; d->size=a->size; d->vl=getmem(sizeof(*d->vl)*d->vc);
  k=4096*b->vc/d->vc;
  for (i=0; i<d->vc; i++) {
    j=i*k>>12;
    d->vl[i].x=a->vl[i].x*(1-v)+b->vl[j].x*v;
    d->vl[i].y=a->vl[i].y*(1-v)+b->vl[j].y*v;
  }
  return d;
}
Alue *new_alue_fade(Alue *a, Alue *b, float v) {
  Alue *d=getmem(sizeof(Alue));
  int i;
  d->vc=a->vc; d->size=a->size; d->vl=getmem(sizeof(*d->vl)*d->vc);
  for (i=0; i<d->vc; i++) {
    d->vl[i].x=a->vl[i].x*(1-v)+b->vl[0].x*v;
    d->vl[i].y=a->vl[i].y*(1-v)+b->vl[0].y*v;
  }
  return d;
}
Alue *new_alue_melt(Alue *a, float v) {
  Alue *d=getmem(sizeof(Alue));
  int i;
  float xx, yy;
  d->vc=a->vc; d->size=a->size; d->vl=getmem(sizeof(*d->vl)*d->vc);
  xx=0; yy=0;
  for (i=0; i<a->vc; i++) xx+=a->vl[i].x, yy+=a->vl[i].y;
  xx/=a->vc; yy/=a->vc;
  for (i=0; i<d->vc; i++) xx+=(a->vl[i].x-xx)*v, yy+=(a->vl[i].y-yy)*v;
  for (i=0; i<d->vc; i++) {
    d->vl[i].x=xx+=(a->vl[i].x-xx)*v;
    d->vl[i].y=yy+=(a->vl[i].y-yy)*v;
  }
  return d;
}

Alue *new_alue_clone(Alue *a) {
  Alue *d=getmem(sizeof(Alue));
  int i;
  d->vc=a->vc; d->size=a->size; d->vl=getmem(sizeof(*d->vl)*d->vc);
  for (i=0; i<d->vc; i++) {
    d->vl[i].x=a->vl[i].x;
    d->vl[i].y=a->vl[i].y;
  }
  return d;
}


