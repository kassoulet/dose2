typedef struct _Obu2d Obu2d;
Obu2d *obu2d_lue(FILE *f);
Obu2d *obu2d_lue2(char *fn);
Obu2d *obu2d_draw(Obu2d *o, Layer *k, int tmask, M2d *m, M2d *mk);
Obu2d *new_obu2d_morph(Obu2d *a, Obu2d *b, float v);
Obu2d *new_obu2d_morph2(Obu2d *a, Obu2d *b, float v);
Obu2d *new_obu2d_melt(Obu2d *a, float v);

