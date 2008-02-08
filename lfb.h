/*
 *  linear flat frame buffer definition and some accessories
 *
 */
#ifndef libya_lfb_h_included
#define libya_lfb_h_included

typedef struct _Lfb Lfb;
struct _Lfb {
  char *buf;
  int width, height, pitch, depth;
};

#define lfbrow4(l, y) ((unsigned*)((l)->buf+(y)*(l)->pitch))
#define lfbrow2(l, y) ((unsigned short*)((l)->buf+(y)*(l)->pitch))
#define lfbrow1(l, y) ((unsigned char*)((l)->buf+(y)*(l)->pitch))
#define lfbpix4(l, x, y) (lfbrow4((l), (y))[(x)])
#define lfbpix2(l, x, y) (lfbrow2((l), (y))[(x)])
#define lfbpix1(l, x, y) (lfbrow1((l), (y))[(x)])

#endif
