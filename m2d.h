typedef struct _M2d M2d;
struct _M2d {
  float xx, xy, x1, yx, yy, y1;
};

M2d *new_m2_roto(float x0, float y0, float r, float i);
M2d *new_m2_mul(M2d *a, M2d *b);

