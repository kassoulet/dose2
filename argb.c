#include "schaibe.h"
//#include <lib2\asm.h>
//#include <lib2\fasm.h>
//#include <lib2\mem.h>
#include "argb.h"


Argb *new_col(float a, float r, float g, float b) {
  Argb *d=getmem(sizeof(Argb));
  d->a=a; d->r=r; d->g=g; d->b=b;
  return d;
}
Argb *col_mix(Argb *d, Argb *a, Argb *b) {
  d->r=b->r+a->r*(1-b->a);
  d->g=b->g+a->g*(1-b->a);
  d->b=b->b+a->b*(1-b->a);
  d->a=b->a+a->a*(1-b->a);
  return d;
}

