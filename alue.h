#ifndef aluehoo
#define aluehoo
typedef struct _Alue Alue;
typedef struct { float x, y; } Point2d;
struct _Alue {
  int vc;
  Point2d *vl;
  float size;
};

Alue *alue_lue(FILE *f);
Alue *new_alue_morph(Alue *a, Alue *b, float v);
Alue *new_alue_melt(Alue *a, float v);
Alue *new_alue_fade(Alue *a, Alue *b, float v);

#endif