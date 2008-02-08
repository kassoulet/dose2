#ifndef line_h_included
#define line_h_included


typedef struct _Piste Piste;
struct _Piste { int x, y, code; };

Piste *new_piste(int x, int y);
void line(char *d, Piste *p1, Piste *p2, int c);
void clip1(Piste *p1, Piste *p2, float a, float b, float c);
void lineclip(char *d, Piste *q1, Piste *q2, int c);
#define lxmax 639
#define lymax 479


#endif
