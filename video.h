/*
 *  vesa video routines
 *  copyleft jarno all rights reversed
 */

#ifndef __VID_H__
#define __VID_H__

#include "lfb.h"

typedef struct _Video Video;
typedef void (*Copyfnc)(void *);


int vid_getwidth(Video *);
int vid_getheight(Video *);
int vid_getbpp(Video *);

Video *new_vid_screen(int, int, int);
void vid_close(Video *);
Lfb *vid_openlfb(Video *);
void vid_closelfb(Video *);
int vid_set_gamma(Video *v, float gma);
int vid_cut(Video *v, int x0, int y0, int wid, int hgt);
int vid_ydiv(Video *v, int divisor);

#endif
