#ifndef argb_h_included
#define argb_h_included


typedef struct _Argb Argb;
struct _Argb { float a, r, g, b; };

Argb *new_col(float a, float r, float g, float b);
Argb *col_mix(Argb *d, Argb *a, Argb *b);


#endif
