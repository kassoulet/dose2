#include "demo.h"

#include "schaibe.h"
#include <stdio.h>
//#include <lib2\mem.h>
//#include <lib2\asm.h>
//#include <lib2\fasm.h>
#include "m2d.h"
#include "line.h"
#include "alue.h"
#include "taso.h"

Taso *taso_lue(FILE *f) {
  int i;
  unsigned char c;
  Taso *k=getmem(sizeof(Taso));

  fix_fread(&k->ac, 1, 4, f);
  if(k->ac<0)
  {
    k->ac=0;
    return(k);
  }

  k->al=getmem(sizeof(Alue*)*k->ac);
  for (i=0; i<k->ac; i++) k->al[i]=alue_lue(f);
  {
    int phlag;
    do {
      phlag=0;
      for (i=0; i<k->ac-1; i++) if (k->al[i]->size<k->al[i+1]->size) {
        Alue *x=k->al[i]; k->al[i]=k->al[i+1]; k->al[i+1]=x; phlag=1;
      }
    } while (phlag);
  }
  return k;
}
Taso *taso_draw(Taso *k, char *videomem, M2d *m, int kolor) {
  Alue **a, *aa;
  Piste v1, v2;
  Point2d *w;
  for (a=k->al;k->ac && a<k->al+k->ac; a++) {
    aa=*a;
    w=aa->vl+aa->vc-1;
    v1.x=fist2(w->x*m->xx+w->y*m->xy+m->x1);
    v1.y=fist2(w->x*m->yx+w->y*m->yy+m->y1);
    v1.code=l_mask(v1.x, 0)&1|l_mask(v1.y, 4096)&2
           |nl_mask(v1.x, WIDTH*4096)&4|nl_mask(v1.y, HEIGHT*4096)&8;
    for (w=aa->vl; w<aa->vl+aa->vc; w++) {
      v2=v1;
      v1.x=fist2(w->x*m->xx+w->y*m->xy+m->x1);
      v1.y=fist2(w->x*m->yx+w->y*m->yy+m->y1);
      v1.code=l_mask(v1.x, 0)&1|l_mask(v1.y, 4096)&2
             |nl_mask(v1.x, WIDTH*4096)&4|nl_mask(v1.y, HEIGHT*4096)&8;
      if ((v1.code&v2.code)*2&(v1.code^v2.code)&2) videomem[0]^=kolor;
      if (v1.code&v2.code) continue;
      if (v1.code|v2.code) lineclip(videomem, &v1, &v2, kolor); else
        line(videomem, &v1, &v2, kolor);
    }
  }
  return k;
}

Taso *new_taso_morph(Taso *a, Taso *b, float v) {
  Taso *d=getmem(sizeof(Taso));
  int i;
  if (a->ac<b->ac) { Taso *t=b; b=a; a=t; v=1-v; }
  //if (v>0.5) d->ac=b->ac; else d->ac=a->ac;
  d->ac=a->ac;
  d->al=getmem(sizeof(Alue*)*d->ac);
  for (i=0; i<b->ac; i++) d->al[i]=new_alue_morph(a->al[i], b->al[i], v);
  for (; i<d->ac; i++) d->al[i]=new_alue_fade(a->al[i], b->al[b->ac-1], v);
  return d;
}
Taso *new_taso_melt(Taso *a, float v) {
  Taso *d=getmem(sizeof(Taso));
  int i;
  d->ac=a->ac; d->al=getmem(sizeof(Alue*)*d->ac);
  for (i=0; i<d->ac; i++) d->al[i]=new_alue_melt(a->al[i], v);
  return d;
}



