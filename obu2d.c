#include "demo.h"

#include "schaibe.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
//#include <lib2\mem.h>
//#include <lib2\fasm.h>
#include "argb.h"
#include "m2d.h"
#include "alue.h"
#include "taso.h"
#include "layer.h"
#include "obu2d.h"


struct _Obu2d {
  int tc;
  Taso **tl;
};
Obu2d *obu2d_lue(FILE *f) {
  int i;
  Obu2d *o=getmem(sizeof(Obu2d));
  fix_fread(&o->tc, 1, 4, f);
  o->tl=getmem(sizeof(Alue*)*o->tc);
  for (i=0; i<o->tc; i++) o->tl[i]=taso_lue(f);
  return o;
}
Obu2d *obu2d_lue2(char *fname) {
  FILE *fp=fopen(fname, "rb");
  Obu2d *o;
  if (!fp) { fprintf(stderr, "!file %s\n", fname); exit(1); }
  o=obu2d_lue(fp);
  fclose(fp);
  return o;
}
Obu2d *obu2d_draw(Obu2d *o, Layer *k, int tmask, M2d *m, M2d *mk) {
  int i, j;
  static M2d m2d_sifti={WIDTH/640.0, 0, WIDTH/2*4096, 0, HEIGHT/480.0, HEIGHT/2*4096};
  M2d *mm=new_m2_mul(m, &m2d_sifti);
  for (i=0, j=0; i<o->tc; i++) if (!(tmask>>i&1) && j+1<k->lkm) {
    taso_draw(o->tl[i], k->vmem, mm, (j+1^j)<<k->shift);
    mm=new_m2_mul(mk, mm); j++;
  }
  return o;
}


Obu2d *new_obu2d_morph(Obu2d *a, Obu2d *b, float v) {
  Obu2d *d=getmem(sizeof(Obu2d));
  int i=0;
  if (a->tc<b->tc) { Obu2d *t=b; b=a; a=t; v=1-v; }
  d->tc=v<0.5?a->tc:b->tc; d->tl=getmem(sizeof(Taso*)*d->tc);
  for (; i<b->tc; i++) d->tl[i]=new_taso_morph(a->tl[i], b->tl[i], v);
  for (; i<d->tc; i++) d->tl[i]=new_taso_melt(a->tl[i], 0);
  return d;
}
Obu2d *new_obu2d_melt(Obu2d *a, float v) {
  Obu2d *d=getmem(sizeof(Obu2d));
  int i;
  d->tc=a->tc; d->tl=getmem(sizeof(Taso*)*d->tc);
  for (i=0; i<d->tc; i++) d->tl[i]=new_taso_melt(a->tl[i], v);
  return d;
}
Obu2d *new_obu2d_morph2(Obu2d *a, Obu2d *b, float v) {
  return new_obu2d_melt(
    new_obu2d_morph(a, b, v),
    pow(0.3+(1-v*2)*(1-v*2)*0.7, 2)
  );
}



