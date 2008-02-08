#include "alue.h"
struct _Taso {
  int ac;
  Alue **al;
};
typedef struct _Taso Taso;

Taso *taso_lue(FILE *f);
Taso *taso_draw(Taso *k, char *videomem, M2d *m, int kolor);
Taso *new_taso_morph(Taso *a, Taso *b, float v);
Taso *new_taso_melt(Taso *a, float v);

