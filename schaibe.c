#include "schaibe.h"
#include <math.h>

Vector v_ci={1, 0, 0};
Vector v_cj={0, 1, 0};
Vector v_ck={0, 0, 1};
Vector v_cmi={-1, 0, 0};
Vector v_cmj={0, -1, 0};
Vector v_cmk={0, 0, -1};
Vector v_c0={0, 0, 0};
Matrix m_c_ident={{1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {0, 0, 0}};

Vector *v_sub(Vector *d, Vector *a, Vector *b) { d->x=a->x-b->x; d->y=a->y-b->y; d->z=a->z-b->z; return d; }
float v_dot(Vector *a, Vector *b) { return a->x*b->x+a->y*b->y+a->z*b->z; }
Vector *v_cross(Vector *d, Vector *a, Vector *b) { d->x=a->y*b->z-a->z*b->y; d->y=a->z*b->x-a->x*b->z; d->z=a->x*b->y-a->y*b->x; return d; }
Vector *v_norm(Vector *d) { return v_mul(d, d, 1.0/sqrt(v_dot(d, d))); }
Vector *v_mul(Vector *d, Vector *a, float k) { d->x=a->x*k; d->y=a->y*k; d->z=a->z*k; return d; }
Vector *new_v() { return getmem(sizeof(Vector)); }
Vector *v_create(Vector *d, float x, float y, float z) { d->x=x; d->y=y; d->z=z; return d; }
