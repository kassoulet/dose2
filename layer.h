#ifndef layer_h_included
#define layer_h_included



#include "palette.h"

typedef struct _Layer Layer;
struct _Layer {
  int shift, tyyp, size;
  int lkm;
  Argb pal[256];
  char *vmem;
};

void init_layers(char *vm, Argb *bg);
char *teepal1();
void teepal2();
int getp();
Layer *new_layer(Palette *p);



#endif

