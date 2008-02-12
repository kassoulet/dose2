#include "schaibe.h"
//#include <lib2\asm.h>
//#include <lib2\fasm.h>
//#include <lib2\mem.h>
//#include <lib2\vectorz.h>
#include "layer.h"
#include "line.h"


typedef struct _Blob Blob;

Blob *new_blob(float v);
void blob_addcomp(Blob *b, float x, float y, float p, float t);
void blob_draw(Blob *b, Layer *l);
void blob_lisaatuneli(Blob *dest, float p, float a, Vector *pos);
void blob_lisaa2xtuneli(Blob *dest, float p, float a, Vector *pos, Vector *pos2);



