#include "blob.h"
#include <math.h>


typedef struct { float v; Piste *px, *py; int joo; } Comp;
struct _Blob {
  Comp buf[41*31];
};

Matrix *new_m() { return getmem(sizeof(Matrix)); }
Matrix *new_m_lookat(Vector *p, Vector *t, Vector *up) {
  Matrix *m=new_m();
  m->l=*p;
  v_norm(v_sub(&m->k, t, p));
  v_norm(v_cross(&m->i, up, &m->k));
  v_norm(v_cross(&m->j, &m->k, &m->i));
  return m;
}
Blob *new_blob(float v) {
  Blob *z=getmem(sizeof(Blob));
  int i;
  for (i=0; i<41*31; i++) z->buf[i].v=v;
  return z;
}
void blob_addcomp(Blob *b, float x, float y, float p, float t) {
  int xi, yi, x0, y0, x1, y1;
  float r, k=(1.0/0.05-1)/t;
  x0=x-15; y0=y-15; x1=x+15; y1=y+15;
  if (x1<0 || y1<0 || x0>40 || y0>30) return;
  if (x0<0) x0=0;
  if (y0<0) y0=0;
  if (x1>41) x1=41;
  if (y1>31) y1=31;
  for (yi=y0; yi<y1; yi++)
    for (xi=x0; xi<x1; xi++) {
      r=(xi-x)*(xi-x)+(yi-y)*(yi-y);
      if (r<k) b->buf[yi*41+xi].v+=p*(1.0/(1+r*t)-0.05);
    }
}
void blob_draw(Blob *b, Layer *l) {
  int x, y, x0, y0, x1, y1, c, col;
  Piste p1, p2;
  Comp *q;

  col=1<<l->shift;
  mark();
  if (b->buf[0].v>0) l->vmem[0]^=col;
  for (y=0; y<30; y++) for (x=0; x<41; x++) {
    q=b->buf+y*41+x;
    if (q[0].v*q[41].v<0) q->py=new_piste(x*16*4096, (y+q[0].v/(q[0].v-q[41].v))*16*4096.0+4096); else q->py=0;
  }
  for (y=0; y<31; y++) for (x=0; x<40; x++) {
    q=b->buf+y*41+x;
    if (q[0].v*q[1].v<0) {
      q->px=new_piste((x+q[0].v/(q[0].v-q[1].v))*16*4096.0, y*16*4096+4096);
      if (y==0) l->vmem[q->px->x>>12]^=col;
    } else q->px=0;
  }
  for (y=0; y<30; y++) {
    q=b->buf+y*41;
    for (x=0; x<40; x++) {
      c=(ne_mask((int)q[ 0].px, 0)&1)
       +(ne_mask((int)q[ 0].py, 0)&2)
       +(ne_mask((int)q[41].px, 0)&4)
       +(ne_mask((int)q[ 1].py, 0)&8);
      switch (c) {
        case  3: line(l->vmem, q[ 0].px, q[ 0].py, col); break;
        case  6: line(l->vmem, q[ 0].py, q[41].px, col); break;
        case 12: line(l->vmem, q[41].px, q[ 1].py, col); break;
        case  9: line(l->vmem, q[ 1].py, q[ 0].px, col); break;
        case  5: line(l->vmem, q[ 0].px, q[41].px, col); break;
        case 10: line(l->vmem, q[ 0].py, q[ 1].py, col); break;
        //case 15: line(l->vmem, q[ 0].px, q[41].px, col);
                 //line(l->vmem, q[ 0].py, q[ 1].py, col); break;
        case 15: line(l->vmem, q[ 0].px, q[ 0].py, col);
                 line(l->vmem, q[41].px, q[ 1].py, col); break;
      }
      q++;
    }
  }
  release();
}


/*Matrix *teematriisi2(float dx, float dy, float dz) {
  static vektori dir, yvec, up, right;

  dir.x=dx; dir.y=dy; dir.z=dz; normalisoi(&dir);
  yvec.x=1; yvec.y=0.5; yvec.z=0.0; normalisoi(&yvec);
  risti(&right, &dir, &yvec); normalisoi(&right);
  risti(&up, &right, &dir); normalisoi(&up);

  m[0][0]=right.x;
  m[0][1]=right.y;
  m[0][2]=right.z;
  m[1][0]=up.x;
  m[1][1]=up.y;
  m[1][2]=up.z;
  m[2][0]=dir.x;
  m[2][1]=dir.y;
  m[2][2]=dir.z;
}*/

void blob_lisaatuneli(Blob *dest, float p, float a, Vector *pos) {
  Matrix camera;
  Vector suunta;
  int x, y;
  float perx, pery, f;
  float ax, bx, cx;
  float t, px, py, pz, bxi;
  Comp *bp=dest->buf;

  camera=*new_m_lookat(pos, &v_c0, &v_cj);
  pery=1/12.5;
  perx=1/15.0;
  v_mul(&camera.i, &camera.i, perx);
  v_mul(&camera.j, &camera.j, pery);

  cx=pos->x*pos->x+pos->y*pos->y+pos->z*pos->z-1;
  for (y=0; y<31; y++) {
    suunta.x=camera.k.x + camera.j.x*(y-15.5) + camera.i.x*(-20.5);
    suunta.y=camera.k.y + camera.j.y*(y-15.5) + camera.i.y*(-20.5);
    suunta.z=camera.k.z + camera.j.z*(y-15.5) + camera.i.z*(-20.5);
    bx=suunta.x*pos->x+suunta.y*pos->y+suunta.z*pos->z;
    bxi=camera.i.x*pos->x+camera.i.y*pos->y+camera.i.z*pos->z;
    for (x=0; x<41; x++) {
      ax=suunta.x*suunta.x+suunta.y*suunta.y+suunta.z*suunta.z;
      //bx=suunta.x*pos->x+suunta.y*pos->y+suunta.z*pos->z;

      t=(sqrt(bx*bx-ax*cx)-bx)/ax;
      px=pos->x+t*suunta.x;
      py=pos->y+t*suunta.y;
      pz=pos->z+t*suunta.z;

      f=(atan2(px, py)+a)+pz/**sin(t)*/*20;
      f*=0.5/pi; bp++->v+=(fabs(f-fist2(f))-0.25)*p*4;
      //bp++->v+=sin(f)*p;
      suunta.x+=camera.i.x;
      suunta.y+=camera.i.y;
      suunta.z+=camera.i.z;
      bx+=bxi;
    }
  }
}





void blob_lisaa2xtuneli(Blob *dest, float p, float a, Vector *pos, Vector *pos2) {
  Matrix camera;
  Vector suunta;
  int x, y;
  float perx, pery, f;
  float ax, bx, cx;
  float t, px, py, pz, bxi;
  Matrix camera2;
  Vector suunta2;
  float ax2, bx2, cx2;
  float t2, px2, py2, pz2, bxi2;
  Comp *bp=dest->buf;

  camera=*new_m_lookat(pos, &v_c0, &v_cj);
  pery=1/12.5;
  perx=1/15.0;
  v_mul(&camera.i, &camera.i, perx);
  v_mul(&camera.j, &camera.j, pery);

  camera2=*new_m_lookat(pos2, &v_c0, &v_cj);
  v_mul(&camera2.i, &camera2.i, perx);
  v_mul(&camera2.j, &camera2.j, pery);

  cx=pos->x*pos->x+pos->y*pos->y+pos->z*pos->z-1;
  cx2=pos2->x*pos2->x+pos2->y*pos2->y+pos2->z*pos2->z-1;
  for (y=0; y<31; y++) {
    suunta.x=camera.k.x + camera.j.x*(y-15.5) + camera.i.x*(-20.5);
    suunta.y=camera.k.y + camera.j.y*(y-15.5) + camera.i.y*(-20.5);
    suunta.z=camera.k.z + camera.j.z*(y-15.5) + camera.i.z*(-20.5);
    suunta2.x=camera2.k.x + camera2.j.x*(y-15.5) + camera2.i.x*(-20.5);
    suunta2.y=camera2.k.y + camera2.j.y*(y-15.5) + camera2.i.y*(-20.5);
    suunta2.z=camera2.k.z + camera2.j.z*(y-15.5) + camera2.i.z*(-20.5);
    bx=suunta.x*pos->x+suunta.y*pos->y+suunta.z*pos->z;
    bxi=camera.i.x*pos->x+camera.i.y*pos->y+camera.i.z*pos->z;
    bx2=suunta2.x*pos2->x+suunta2.y*pos2->y+suunta2.z*pos2->z;
    bxi2=camera2.i.x*pos2->x+camera2.i.y*pos2->y+camera2.i.z*pos2->z;
    for (x=0; x<41; x++) {
      ax=suunta.x*suunta.x+suunta.y*suunta.y+suunta.z*suunta.z;
      ax2=suunta2.x*suunta2.x+suunta2.y*suunta2.y+suunta2.z*suunta2.z;
      //bx=suunta.x*pos->x+suunta.y*pos->y+suunta.z*pos->z;

      t=(sqrt(bx*bx-ax*cx)-bx)/ax;
      px=pos->x+t*suunta.x;
      py=pos->y+t*suunta.y;
      pz=pos->z+t*suunta.z;
      t2=(sqrt(bx2*bx2-ax2*cx2)-bx2)/ax2;
      px2=pos2->x+t2*suunta2.x;
      py2=pos2->y+t2*suunta2.y;
      pz2=pos2->z+t2*suunta2.z;
      //pz=1-pz; pz2=1-pz2;
      pz+=1; pz2+=1;

      f=(atan2(px, py)+a)-atan2(px2, py2)+atan2(1, .1/pz-.1/pz2)*9;
      f*=0.5/pi; bp++->v+=(fabs(f-fist2(f))-0.25)*p*4;
      //bp++->v+=fsin(f)*p;
      suunta.x+=camera.i.x;
      suunta.y+=camera.i.y;
      suunta.z+=camera.i.z;
      bx+=bxi;
      suunta2.x+=camera2.i.x;
      suunta2.y+=camera2.i.y;
      suunta2.z+=camera2.i.z;
      bx2+=bxi2;
    }
  }

}


