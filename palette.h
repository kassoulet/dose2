
#include "argb.h"

typedef struct _Palette Palette;

Palette *new_pal();
Palette *new_pal1(Argb *c1);
Palette *new_pal2(Argb *c1, Argb *c2);
Palette *new_pal3(Argb *c1, Argb *c2, Argb *c3);
Palette *new_pal4(Argb *c1, Argb *c2, Argb *c3, Argb *c4);
Palette *new_pal8(Argb *c1, Argb *c2, Argb *c3, Argb *c4, Argb *c5, Argb *c6, Argb *c7, Argb *c8);
Palette *new_pals2(int n, Argb *c1, Argb *c2);
Palette *new_pals3(Argb *c0, int n, Argb *c1, Argb *c2);
void pal_addcol(Palette *p, Argb *c);
void pal_set(Palette *p, int i, Argb *c);
int pal_getsize(Palette *p);
Argb *pal_getcol(Palette *p, int ind);
void pal_rotate(Palette *p, float a, float r);
Palette *pal_reverse(Palette *p);
Palette *pal_alpha(Palette *p, float a);
Palette *pal_alpha2(Palette *p, float a);
void pal_product(Palette *p, Palette *q);





