

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>


 //make it easy on the folks that want to compile the libs with a
   //different malloc than stdlib
#define _ogg_malloc  malloc
#define _ogg_calloc  calloc
#define _ogg_realloc realloc
#define _ogg_free    free

#include "ogdecode.h"
#define vorbis_ftoi(f) ((int)(f+100000.5)-100000)
#define rint(x) (floor((x)+.5f))



typedef struct {
  unsigned char *header;
  long header_len;
  unsigned char *body;
  long body_len;
} ogg_page;

typedef struct {
  unsigned char *packet;
  long  bytes;
  long  b_o_s;
  long  e_o_s;
  ogg_int64_t  granulepos;
  ogg_int64_t  packetno;
} ogg_packet;



struct alloc_chain {
  void *ptr;
  struct alloc_chain *next;
};

typedef struct static_codebook {
  long   dim;
  long   entries;
  long  *lengthlist;
  int    maptype;
  long   q_min;
  long   q_delta;
  int    q_quant;
  int    q_sequencep;
  long   *quantlist;
  int allocedp;
} static_codebook;

typedef struct decode_aux {
  long   *tab;
  int    *tabl;
  int    tabn;
  long   *ptr0;
  long   *ptr1;
  long   aux;
} decode_aux;

typedef struct codebook {
  long dim;
  long entries;
  const static_codebook *c;
  float  *valuelist;
  long   *codelist;
  struct decode_aux *decode_tree;
} codebook;

typedef void vorbis_look_mapping;
typedef void vorbis_look_floor;
typedef void vorbis_look_residue;
typedef void vorbis_look_transform;

typedef struct backend_lookup_state {
  float                 **window[2][2][2];
  vorbis_look_transform **transform[2];
  codebook               *fullbooks;
  int                     modebits;
  vorbis_look_mapping   **mode;
  float ampmax;
} backend_lookup_state;

typedef struct {
  int blockflag;
  int windowtype;
  int transformtype;
  int mapping;
} vorbis_info_mode;

typedef void vorbis_info_floor;
typedef void vorbis_info_residue;
typedef void vorbis_info_mapping;

typedef struct codec_setup_info {
  long blocksizes[2];
  int        modes;
  int        maps;
  int        times;
  int        floors;
  int        residues;
  int        books;
  vorbis_info_mode    *mode_param[64];
  vorbis_info_mapping *map_param[64];
  int                  floor_type[64];
  vorbis_info_floor   *floor_param[64];
  int                  residue_type[64];
  vorbis_info_residue *residue_param[64];
  static_codebook     *book_param[256];
} codec_setup_info;

typedef struct {
  vorbis_info_floor     *(*unpack)(vorbis_info *, oggpack_buffer *);
  vorbis_look_floor     *(*look)  (vorbis_dsp_state *, vorbis_info_mode *,
                                   vorbis_info_floor *);
  void (*free_info) (vorbis_info_floor *);
  void (*free_look) (vorbis_look_floor *);
  void *(*inverse1)  (struct vorbis_block *, vorbis_look_floor *);
  int   (*inverse2)  (struct vorbis_block *, vorbis_look_floor *,
                     void *buffer, float *);
} vorbis_func_floor;

typedef struct {
  int   order;
  long  rate;
  long  barkmap;
  int   ampbits;
  int   ampdB;
  int   numbooks;
  int   books[16];
} vorbis_info_floor0;

typedef struct {
  int   partitions;
  int   partitionclass[31];
  int   class_dim[16];
  int   class_subs[16];
  int   class_book[16];
  int   class_subbook[16][8];
  int   mult;
  int   postlist[63+2];
} vorbis_info_floor1;

typedef struct {
  vorbis_info_residue *(*unpack)(vorbis_info *, oggpack_buffer *);
  vorbis_look_residue *(*look)  (vorbis_dsp_state *, vorbis_info_mode *,
                                 vorbis_info_residue *);
  void (*free_info)    (vorbis_info_residue *);
  void (*free_look)    (vorbis_look_residue *);
  int  (*inverse)      (struct vorbis_block *, vorbis_look_residue *,
                        float **, int *, int);
} vorbis_func_residue;

typedef struct vorbis_info_residue0{
  long  begin;
  long  end;
  int    grouping;
  int    partitions;
  int    groupbook;
  int    secondstages[64];
  int    booklist[256];
} vorbis_info_residue0;

typedef struct {
  vorbis_info_mapping *(*unpack)(vorbis_info *, oggpack_buffer *);
  vorbis_look_mapping *(*look)  (vorbis_dsp_state *, vorbis_info_mode *,
                                 vorbis_info_mapping *);
  void (*free_info)    (vorbis_info_mapping *);
  void (*free_look)    (vorbis_look_mapping *);
  int  (*inverse)      (struct vorbis_block *vb, vorbis_look_mapping *);
} vorbis_func_mapping;

typedef struct vorbis_info_mapping0 {
  int   submaps;
  int   chmuxlist[256];
  int   floorsubmap[16];
  int   residuesubmap[16];
  int   coupling_steps;
  int   coupling_mag[256];
  int   coupling_ang[256];
} vorbis_info_mapping0;

typedef struct {
  long n;
  int ln;
  int  m;
  int *linearmap;
  vorbis_info_floor0 *vi;
  float *lsp_look;
} vorbis_look_floor0;

typedef struct {
  int sorted_index[63+2];
  int forward_index[63+2];
  int reverse_index[63+2];
  int hineighbor[63];
  int loneighbor[63];
  int posts;
  int n;
  int quant_q;
  vorbis_info_floor1 *vi;
} vorbis_look_floor1;

typedef struct {
  vorbis_info_residue0 *info;
  int         map;
  int         parts;
  int         stages;
  codebook   *fullbooks;
  codebook   *phrasebook;
  codebook ***partbooks;
  int         partvals;
  int       **decodemap;
} vorbis_look_residue0;

typedef struct {
  vorbis_info_mode *mode;
  vorbis_info_mapping0 *map;
  vorbis_look_floor **floor_look;
  vorbis_look_residue **residue_look;
  vorbis_func_floor **floor_func;
  vorbis_func_residue **residue_func;
  int ch;
} vorbis_look_mapping0;

typedef struct {
  int n;
  int log2n;
  float *trig;
  int       *bitrev;
  float scale;
} mdct_lookup;


#define  NOTOPEN   0
#define  PARTOPEN  1
#define  OPENED    2
#define  STREAMSET 3
#define  INITSET   4



static void   oggpack_readinit(oggpack_buffer *b,unsigned char *buf,int bytes);
static long   oggpack_look(oggpack_buffer *b,int bits);
static void   oggpack_adv(oggpack_buffer *b,int bits);
static long   oggpack_read(oggpack_buffer *b,int bits);
static long   oggpack_read1(oggpack_buffer *b);
static int    ogg_sync_init(ogg_sync_state *oy);
static int    ogg_sync_clear(ogg_sync_state *oy);
static char  *ogg_sync_buffer(ogg_sync_state *oy, long size);
static int    ogg_sync_wrote(ogg_sync_state *oy, long bytes);
static long   ogg_sync_pageseek(ogg_sync_state *oy,ogg_page *og);
static int    ogg_sync_pageout(ogg_sync_state *oy, ogg_page *og);
static int    ogg_stream_pagein(ogg_stream_state *os, ogg_page *og);
static int    ogg_stream_packetout(ogg_stream_state *os,ogg_packet *op);
static int    ogg_stream_init(ogg_stream_state *os,int serialno);
static int    ogg_stream_clear(ogg_stream_state *os);
static int    ogg_page_version(ogg_page *og);
static int    ogg_page_continued(ogg_page *og);
static int    ogg_page_bos(ogg_page *og);
static int    ogg_page_eos(ogg_page *og);
static ogg_int64_t ogg_page_granulepos(ogg_page *og);
static int    ogg_page_serialno(ogg_page *og);
static long   ogg_page_pageno(ogg_page *og);
static int    ogg_page_packets(ogg_page *og);
static void   vorbis_info_init(vorbis_info *vi);
static void   vorbis_info_clear(vorbis_info *vi);
static int    vorbis_block_init(vorbis_dsp_state *v, vorbis_block *vb);
static int    vorbis_block_clear(vorbis_block *vb);
static void   vorbis_dsp_clear(vorbis_dsp_state *v);
static int    vorbis_synthesis_headerin(vorbis_info *vi,ogg_packet *op);
static int    vorbis_synthesis_init(vorbis_dsp_state *v,vorbis_info *vi);
static int    vorbis_synthesis(vorbis_block *vb,ogg_packet *op);
static int    vorbis_synthesis_blockin(vorbis_dsp_state *v,vorbis_block *vb);
static int    vorbis_synthesis_pcmout(vorbis_dsp_state *v,float ***pcm);
static int    vorbis_synthesis_read(vorbis_dsp_state *v,int samples);
static void   vorbis_staticbook_clear(static_codebook *b);
static void   vorbis_staticbook_destroy(static_codebook *b);
static int    vorbis_book_init_decode(codebook *dest,const static_codebook *source);
static void   vorbis_book_clear(codebook *b);
static float *_book_unquantize(const static_codebook *b);
static float  _float32_unpack(long val);
static long   _book_maptype1_quantvals(const static_codebook *b);
static int    vorbis_staticbook_unpack(oggpack_buffer *b,static_codebook *c);
static long   vorbis_book_decode(codebook *book, oggpack_buffer *b);
static long   vorbis_book_decodevs_add(codebook *book, float *a, oggpack_buffer *b,int n);
static long   vorbis_book_decodev_set(codebook *book, float *a, oggpack_buffer *b,int n);
static long   vorbis_book_decodev_add(codebook *book, float *a, oggpack_buffer *b,int n);
static long   vorbis_book_decodevv_add(codebook *book, float **a, long off,int ch, oggpack_buffer *b,int n);
static void   vorbis_lsp_to_curve(float *curve, int *map, int n, int ln, float *lsp, int m, float amp, float ampoffset);
static void  *_vorbis_block_alloc(vorbis_block *vb, long bytes);
static void   _vorbis_block_ripcord(vorbis_block *vb);
static void   mdct_init(mdct_lookup *lookup,int n);
static void   mdct_clear(mdct_lookup *l);
static void   mdct_backward(mdct_lookup *init, float *in, float *out);
static float *_vorbis_window(int type,int window,int left,int right);
static int    _ilog(unsigned int v);
static int    ilog(unsigned int v);
static int    ilog2(unsigned int v);



extern int ov_clear(OggVorbis_File *vf);
extern int ov_open(FILE *f,OggVorbis_File *vf,char *initial,long ibytes);
extern int ov_open_callbacks(void *datasource, OggVorbis_File *vf,
                char *initial, long ibytes, ov_callbacks callbacks);

extern int ov_test(FILE *f,OggVorbis_File *vf,char *initial,long ibytes);
extern int ov_test_callbacks(void *datasource, OggVorbis_File *vf,
                char *initial, long ibytes, ov_callbacks callbacks);
extern int ov_test_open(OggVorbis_File *vf);

extern long ov_bitrate(OggVorbis_File *vf,int i);
extern long ov_bitrate_instant(OggVorbis_File *vf);
extern long ov_streams(OggVorbis_File *vf);
extern long ov_seekable(OggVorbis_File *vf);
extern long ov_serialnumber(OggVorbis_File *vf,int i);

extern ogg_int64_t ov_raw_total(OggVorbis_File *vf,int i);
extern ogg_int64_t ov_pcm_total(OggVorbis_File *vf,int i);
extern double ov_time_total(OggVorbis_File *vf,int i);

extern int ov_raw_seek(OggVorbis_File *vf,long pos);
extern int ov_pcm_seek(OggVorbis_File *vf,ogg_int64_t pos);
extern int ov_pcm_seek_page(OggVorbis_File *vf,ogg_int64_t pos);
extern int ov_time_seek(OggVorbis_File *vf,double pos);
extern int ov_time_seek_page(OggVorbis_File *vf,double pos);

extern ogg_int64_t ov_raw_tell(OggVorbis_File *vf);
extern ogg_int64_t ov_pcm_tell(OggVorbis_File *vf);
extern double ov_time_tell(OggVorbis_File *vf);

extern vorbis_info *ov_info(OggVorbis_File *vf,int link);

extern long ov_read(OggVorbis_File *vf,char *buffer,int length,
                    int bigendianp,int word,int sgned,int *bitstream);











static void floor0_free_info(vorbis_info_floor *i) {
  if (i) {
    memset(i, 0, sizeof(vorbis_info_floor0));
    _ogg_free(i);
  }
}

static void floor0_free_look(vorbis_look_floor *i) {
  vorbis_look_floor0 *look=(vorbis_look_floor0 *)i;
  if (i) {
    if (look->linearmap) _ogg_free(look->linearmap);
    if (look->lsp_look) _ogg_free(look->lsp_look);
    memset(look, 0, sizeof(vorbis_look_floor0));
    _ogg_free(look);
  }
}

static vorbis_info_floor *floor0_unpack (vorbis_info *vi, oggpack_buffer *opb) {
  codec_setup_info     *ci=vi->codec_setup;
  int j;

  vorbis_info_floor0 *info=_ogg_malloc(sizeof(vorbis_info_floor0));
  info->order=oggpack_read(opb, 8);
  info->rate=oggpack_read(opb, 16);
  info->barkmap=oggpack_read(opb, 16);
  info->ampbits=oggpack_read(opb, 6);
  info->ampdB=oggpack_read(opb, 8);
  info->numbooks=oggpack_read(opb, 4)+1;

  if (info->order<1) goto err_out;
  if (info->rate<1) goto err_out;
  if (info->barkmap<1) goto err_out;
  if (info->numbooks<1) goto err_out;

  for (j=0; j<info->numbooks; j++) {
    info->books[j]=oggpack_read(opb, 8);
    if (info->books[j]<0 || info->books[j]>=ci->books) goto err_out;
  }
  return info;

 err_out:
  floor0_free_info(info);
  return NULL;
}

static vorbis_look_floor *floor0_look (vorbis_dsp_state *vd, vorbis_info_mode *mi,
                              vorbis_info_floor *i) {
  int j;
  float scale;
  vorbis_info        *vi=vd->vi;
  codec_setup_info   *ci=vi->codec_setup;
  vorbis_info_floor0 *info=(vorbis_info_floor0 *)i;
  vorbis_look_floor0 *look=_ogg_calloc(1, sizeof(vorbis_look_floor0));
  look->m=info->order;
  look->n=ci->blocksizes[mi->blockflag]/2;
  look->ln=info->barkmap;
  look->vi=info;
  scale=look->ln/(13.1f*(float)atan(.00074f*(info->rate*.5f))+2.24f*(float)atan((info->rate*.5f)*(info->rate*.5f)*1.85e-8f)+1e-4f*(info->rate*.5f));
  look->linearmap=_ogg_malloc((look->n+1)*sizeof(int));
  for (j=0; j<look->n; j++) {
    int val=(int)floor((13.1f*(float)atan(.00074f*((info->rate*.5f)/look->n*j))+2.24f*(float)atan(((info->rate*.5f)/look->n*j)*((info->rate*.5f)/look->n*j)*1.85e-8f)+1e-4f*((info->rate*.5f)/look->n*j)) *scale);
    if (val>=look->ln) val=look->ln; /* guard against the approximation */
    look->linearmap[j]=val;
  }
  look->linearmap[j]=-1;

  look->lsp_look=_ogg_malloc(look->ln*sizeof(float));
  for (j=0; j<look->ln; j++)
    look->lsp_look[j]=2*(float)cos((3.1415926536f)/look->ln*j);

  return look;
}

static void *floor0_inverse1(vorbis_block *vb, vorbis_look_floor *i) {
  vorbis_look_floor0 *look=(vorbis_look_floor0 *)i;
  vorbis_info_floor0 *info=look->vi;
  int j, k;

  int ampraw=oggpack_read(&vb->opb, info->ampbits);
  if (ampraw>0) {
    long maxval=(1<<info->ampbits)-1;
    float amp=(float)ampraw/maxval*info->ampdB;
    int booknum=oggpack_read(&vb->opb, _ilog(info->numbooks));

    if (booknum!=-1 && booknum<info->numbooks) {
      backend_lookup_state *be=vb->vd->backend_state;
      codebook *b=be->fullbooks+info->books[booknum];
      float last=0.f;
      float *lsp=_vorbis_block_alloc(vb, sizeof(float)*(look->m+1));

      for (j=0; j<look->m; j+=b->dim)
        if (vorbis_book_decodev_set(b, lsp+j, &vb->opb, b->dim)==-1) goto eop;
      for (j=0; j<look->m; ) {
        for (k=0; k<b->dim; k++, j++) lsp[j]+=last;
        last=lsp[j-1];
      }

      lsp[look->m]=amp;
      return lsp;
    }
  }
 eop:
  return NULL;
}

static int floor0_inverse2(vorbis_block *vb, vorbis_look_floor *i,
                           void *memo, float *out) {
  vorbis_look_floor0 *look=(vorbis_look_floor0 *)i;
  vorbis_info_floor0 *info=look->vi;

  if (memo) {
    float *lsp=(float *)memo;
    float amp=lsp[look->m];
    vorbis_lsp_to_curve(out, look->linearmap, look->n, look->ln,
                        lsp, look->m, amp, (float)info->ampdB);
    return 1;
  }
  memset(out, 0, sizeof(float)*look->n);
  return 0;
}

static void floor1_free_info(vorbis_info_floor *i) {
  if (i) {
    memset(i, 0, sizeof(vorbis_info_floor1));
    _ogg_free(i);
  }
}

static void floor1_free_look(vorbis_look_floor *i) {
  vorbis_look_floor1 *look=(vorbis_look_floor1 *)i;
  if (i) {
    memset(look, 0, sizeof(vorbis_look_floor1));
    free(i);
  }
}

static vorbis_info_floor *floor1_unpack (vorbis_info *vi, oggpack_buffer *opb) {
  codec_setup_info     *ci=vi->codec_setup;
  int j, k, count=0, maxclass=-1, rangebits;

  vorbis_info_floor1 *info=_ogg_calloc(1, sizeof(vorbis_info_floor1));
  info->partitions=oggpack_read(opb, 5);
  for (j=0; j<info->partitions; j++) {
    info->partitionclass[j]=oggpack_read(opb, 4);
    if (maxclass<info->partitionclass[j]) maxclass=info->partitionclass[j];
  }

  for (j=0; j<maxclass+1; j++) {
    info->class_dim[j]=oggpack_read(opb, 3)+1;
    info->class_subs[j]=oggpack_read(opb, 2);
    if (info->class_subs[j]<0)
      goto err_out;
    if (info->class_subs[j]) info->class_book[j]=oggpack_read(opb, 8);
    if (info->class_book[j]<0 || info->class_book[j]>=ci->books)
      goto err_out;
    for (k=0; k<(1<<info->class_subs[j]); k++) {
      info->class_subbook[j][k]=oggpack_read(opb, 8)-1;
      if (info->class_subbook[j][k]<-1 || info->class_subbook[j][k]>=ci->books)
        goto err_out;
    }
  }

  info->mult=oggpack_read(opb, 2)+1;
  rangebits=oggpack_read(opb, 4);

  for (j=0, k=0; j<info->partitions; j++) {
    count+=info->class_dim[info->partitionclass[j]];
    for (; k<count; k++) {
      int t=info->postlist[k+2]=oggpack_read(opb, rangebits);
      if (t<0 || t>=1<<rangebits)
        goto err_out;
    }
  }
  info->postlist[0]=0;
  info->postlist[1]=1<<rangebits;

  return info;

err_out:
  floor1_free_info(info);
  return NULL;
}

static int icomp(const void *a, const void *b) {
  return **(int **)a-**(int **)b;
}

static int render_point(int x0, int x1, int y0, int y1, int x) {
  y0&=0x7fff;
  y1&=0x7fff;

  {
    int dy=y1-y0;
    int adx=x1-x0;
    int ady=abs(dy);
    int err=ady*(x-x0);

    int off=err/adx;
    if (dy<0) return y0-off;
    return y0+off;
  }
}

static const float FLOOR_fromdB_LOOKUP[256]={
  1.0649863e-07F, 1.1341951e-07F, 1.2079015e-07F, 1.2863978e-07F,
  1.3699951e-07F, 1.4590251e-07F, 1.5538408e-07F, 1.6548181e-07F,
  1.7623575e-07F, 1.8768855e-07F, 1.9988561e-07F, 2.128753e-07F,
  2.2670913e-07F, 2.4144197e-07F, 2.5713223e-07F, 2.7384213e-07F,
  2.9163793e-07F, 3.1059021e-07F, 3.3077411e-07F, 3.5226968e-07F,
  3.7516214e-07F, 3.9954229e-07F, 4.2550680e-07F, 4.5315863e-07F,
  4.8260743e-07F, 5.1396998e-07F, 5.4737065e-07F, 5.8294187e-07F,
  6.2082472e-07F, 6.6116941e-07F, 7.0413592e-07F, 7.4989464e-07F,
  7.9862701e-07F, 8.5052630e-07F, 9.0579828e-07F, 9.6466216e-07F,
  1.0273513e-06F, 1.0941144e-06F, 1.1652161e-06F, 1.2409384e-06F,
  1.3215816e-06F, 1.4074654e-06F, 1.4989305e-06F, 1.5963394e-06F,
  1.7000785e-06F, 1.8105592e-06F, 1.9282195e-06F, 2.0535261e-06F,
  2.1869758e-06F, 2.3290978e-06F, 2.4804557e-06F, 2.6416497e-06F,
  2.8133190e-06F, 2.9961443e-06F, 3.1908506e-06F, 3.3982101e-06F,
  3.6190449e-06F, 3.8542308e-06F, 4.1047004e-06F, 4.3714470e-06F,
  4.6555282e-06F, 4.9580707e-06F, 5.2802740e-06F, 5.6234160e-06F,
  5.9888572e-06F, 6.3780469e-06F, 6.7925283e-06F, 7.2339451e-06F,
  7.7040476e-06F, 8.2047000e-06F, 8.7378876e-06F, 9.3057248e-06F,
  9.9104632e-06F, 1.0554501e-05F, 1.1240392e-05F, 1.1970856e-05F,
  1.2748789e-05F, 1.3577278e-05F, 1.4459606e-05F, 1.5399272e-05F,
  1.6400004e-05F, 1.7465768e-05F, 1.8600792e-05F, 1.9809576e-05F,
  2.1096914e-05F, 2.2467911e-05F, 2.3928002e-05F, 2.5482978e-05F,
  2.7139006e-05F, 2.8902651e-05F, 3.0780908e-05F, 3.2781225e-05F,
  3.4911534e-05F, 3.7180282e-05F, 3.9596466e-05F, 4.2169667e-05F,
  4.4910090e-05F, 4.7828601e-05F, 5.0936773e-05F, 5.4246931e-05F,
  5.7772202e-05F, 6.1526565e-05F, 6.5524908e-05F, 6.9783085e-05F,
  7.4317983e-05F, 7.9147585e-05F, 8.4291040e-05F, 8.9768747e-05F,
  9.5602426e-05F, 0.00010181521F, 0.00010843174F, 0.00011547824F,
  0.00012298267F, 0.00013097477F, 0.00013948625F, 0.00014855085F,
  0.00015820453F, 0.00016848555F, 0.00017943469F, 0.00019109536F,
  0.00020351382F, 0.00021673929F, 0.00023082423F, 0.00024582449F,
  0.00026179955F, 0.00027881276F, 0.00029693158F, 0.00031622787F,
  0.00033677814F, 0.00035866388F, 0.00038197188F, 0.00040679456F,
  0.00043323036F, 0.00046138411F, 0.00049136745F, 0.00052329927F,
  0.00055730621F, 0.00059352311F, 0.00063209358F, 0.00067317058F,
  0.00071691700F, 0.00076350630F, 0.00081312324F, 0.00086596457F,
  0.00092223983F, 0.00098217216F, 0.0010459992F, 0.0011139742F,
  0.0011863665F, 0.0012634633F, 0.0013455702F, 0.0014330129F,
  0.0015261382F, 0.0016253153F, 0.0017309374F, 0.0018434235F,
  0.0019632195F, 0.0020908006F, 0.0022266726F, 0.0023713743F,
  0.0025254795F, 0.0026895994F, 0.0028643847F, 0.0030505286F,
  0.0032487691F, 0.0034598925F, 0.0036847358F, 0.0039241906F,
  0.0041792066F, 0.0044507950F, 0.0047400328F, 0.0050480668F,
  0.0053761186F, 0.0057254891F, 0.0060975636F, 0.0064938176F,
  0.0069158225F, 0.0073652516F, 0.0078438871F, 0.0083536271F,
  0.0088964928F, 0.009474637F, 0.010090352F, 0.010746080F,
  0.011444421F, 0.012188144F, 0.012980198F, 0.013823725F,
  0.014722068F, 0.015678791F, 0.016697687F, 0.017782797F,
  0.018938423F, 0.020169149F, 0.021479854F, 0.022875735F,
  0.024362330F, 0.025945531F, 0.027631618F, 0.029427276F,
  0.031339626F, 0.033376252F, 0.035545228F, 0.037855157F,
  0.040315199F, 0.042935108F, 0.045725273F, 0.048696758F,
  0.051861348F, 0.055231591F, 0.058820850F, 0.062643361F,
  0.066714279F, 0.071049749F, 0.075666962F, 0.080584227F,
  0.085821044F, 0.091398179F, 0.097337747F, 0.10366330F,
  0.11039993F, 0.11757434F, 0.12521498F, 0.13335215F,
  0.14201813F, 0.15124727F, 0.16107617F, 0.17154380F,
  0.18269168F, 0.19456402F, 0.20720788F, 0.22067342F,
  0.23501402F, 0.25028656F, 0.26655159F, 0.28387361F,
  0.30232132F, 0.32196786F, 0.34289114F, 0.36517414F,
  0.38890521F, 0.41417847F, 0.44109412F, 0.46975890F,
  0.50028648F, 0.53279791F, 0.56742212F, 0.60429640F,
  0.64356699F, 0.68538959F, 0.72993007F, 0.77736504F,
  0.82788260F, 0.88168307F, 0.9389798F, 1.F,
};

static void render_line(int x0, int x1, int y0, int y1, float *d) {
  int dy=y1-y0;
  int adx=x1-x0;
  int ady=abs(dy);
  int base=dy/adx;
  int sy=dy<0?base-1:base+1;
  int x=x0;
  int y=y0;
  int err=0;

  ady-=abs(base*adx);

  d[x]*=FLOOR_fromdB_LOOKUP[y];
  while (++x<x1) {
    err=err+ady;
    if (err>=adx) {
      err-=adx;
      y+=sy;
    } else {
      y+=base;
    }
    d[x]*=FLOOR_fromdB_LOOKUP[y];
  }
}

static vorbis_look_floor *floor1_look(vorbis_dsp_state *vd, vorbis_info_mode *mi,
                              vorbis_info_floor *in) {

  int *sortpointer[63+2];
  vorbis_info_floor1 *info=(vorbis_info_floor1 *)in;
  vorbis_look_floor1 *look=_ogg_calloc(1, sizeof(vorbis_look_floor1));
  int i, j, n=0;

  look->vi=info;
  look->n=info->postlist[1];

  for (i=0; i<info->partitions; i++) n+=info->class_dim[info->partitionclass[i]];
  n+=2;
  look->posts=n;

  for (i=0; i<n; i++) sortpointer[i]=info->postlist+i;
  qsort(sortpointer, n, sizeof(int), icomp);

  for (i=0; i<n; i++) look->forward_index[i]=sortpointer[i]-info->postlist;
  for (i=0; i<n; i++) look->reverse_index[look->forward_index[i]]=i;
  for (i=0; i<n; i++) look->sorted_index[i]=info->postlist[look->forward_index[i]];

  switch (info->mult) {
    case 1: look->quant_q=256; break;
    case 2: look->quant_q=128; break;
    case 3: look->quant_q=86; break;
    case 4: look->quant_q=64; break;
  }

  for (i=0; i<n-2; i++) {
    int lo=0;
    int hi=1;
    int lx=0;
    int hx=look->n;
    int currentx=info->postlist[i+2];
    for (j=0; j<i+2; j++) {
      int x=info->postlist[j];
      if (x>lx && x<currentx) {
        lo=j;
        lx=x;
      }
      if (x<hx && x>currentx) {
        hi=j;
        hx=x;
      }
    }
    look->loneighbor[i]=lo;
    look->hineighbor[i]=hi;
  }

  return look;
}

static void *floor1_inverse1(vorbis_block *vb, vorbis_look_floor *in) {
  vorbis_look_floor1 *look=(vorbis_look_floor1 *)in;
  vorbis_info_floor1 *info=look->vi;

  codec_setup_info   *ci=vb->vd->vi->codec_setup;
  int i, j, k;
  codebook *books=((backend_lookup_state *)(vb->vd->backend_state))->fullbooks;

  if (oggpack_read(&vb->opb, 1)==1) {
    int *fit_value=_vorbis_block_alloc(vb, (look->posts)*sizeof(int));

    fit_value[0]=oggpack_read(&vb->opb, ilog(look->quant_q-1));
    fit_value[1]=oggpack_read(&vb->opb, ilog(look->quant_q-1));

    for (i=0, j=2; i<info->partitions; i++) {
      int class=info->partitionclass[i];
      int cdim=info->class_dim[class];
      int csubbits=info->class_subs[class];
      int csub=1<<csubbits;
      int cval=0;

      if (csubbits) {
        cval=vorbis_book_decode(books+info->class_book[class], &vb->opb);

        if (cval==-1) goto eop;
      }

      for (k=0; k<cdim; k++) {
        int book=info->class_subbook[class][cval&(csub-1)];
        cval>>=csubbits;
        if (book>=0) {
          if ((fit_value[j+k]=vorbis_book_decode(books+book, &vb->opb))==-1)
            goto eop;
        } else {
          fit_value[j+k]=0;
        }
      }
      j+=cdim;
    }

    for (i=2; i<look->posts; i++) {
      int predicted=render_point(info->postlist[look->loneighbor[i-2]],
                                 info->postlist[look->hineighbor[i-2]],
                                 fit_value[look->loneighbor[i-2]],
                                 fit_value[look->hineighbor[i-2]],
                                 info->postlist[i]);
      int hiroom=look->quant_q-predicted;
      int loroom=predicted;
      int room=(hiroom<loroom?hiroom:loroom)<<1;
      int val=fit_value[i];

      if (val) {
        if (val>=room) {
          if (hiroom>loroom) {
            val = val-loroom;
          } else {
          val = -1-(val-hiroom);
          }
        } else {
          if (val&1) {
            val= -((val+1)>>1);
          } else {
            val>>=1;
          }
        }

        fit_value[i]=val+predicted;
        fit_value[look->loneighbor[i-2]]&=0x7fff;
        fit_value[look->hineighbor[i-2]]&=0x7fff;

      } else {
        fit_value[i]=predicted|0x8000;
      }

    }

    return fit_value;
  }
 eop:
  return NULL;
}

static int floor1_inverse2(vorbis_block *vb, vorbis_look_floor *in, void *memo,
                          float *out) {
  vorbis_look_floor1 *look=(vorbis_look_floor1 *)in;
  vorbis_info_floor1 *info=look->vi;

  codec_setup_info   *ci=vb->vd->vi->codec_setup;
  int                  n=ci->blocksizes[vb->mode]/2;
  int j;

  if (memo) {
    int *fit_value=(int *)memo;
    int hx;
    int lx=0;
    int ly=fit_value[0]*info->mult;
    for (j=1; j<look->posts; j++) {
      int current=look->forward_index[j];
      int hy=fit_value[current]&0x7fff;
      if (hy==fit_value[current]) {

        hy*=info->mult;
        hx=info->postlist[current];

        render_line(lx, hx, ly, hy, out);

        lx=hx;
        ly=hy;
      }
    }
    for (j=hx; j<n; j++) out[j]*=out[j-1];
    return 1;
  }
  memset(out, 0, sizeof(float)*n);
  return 0;
}

static int icount(unsigned int v) {
  int ret=0;
  while (v) {
    ret+=v&1;
    v>>=1;
  }
  return ret;
}

static int _01inverse(vorbis_block *vb, vorbis_look_residue *vl,
                      float **in, int ch,
                      long (*decodepart)(codebook *, float *,
                                         oggpack_buffer *, int)) {

  long i, j, k, l, s;
  vorbis_look_residue0 *look=(vorbis_look_residue0 *)vl;
  vorbis_info_residue0 *info=look->info;

  int samples_per_partition=info->grouping;
  int partitions_per_word=look->phrasebook->dim;
  int n=info->end-info->begin;

  int partvals=n/samples_per_partition;
  int partwords=(partvals+partitions_per_word-1)/partitions_per_word;
  int ***partword=alloca(ch*sizeof(int **));
  partvals=partwords*partitions_per_word;

  for (j=0; j<ch; j++)
    partword[j]=_vorbis_block_alloc(vb, partwords*sizeof(int *));

  for (s=0; s<look->stages; s++) {
    for (i=info->begin, l=0; i<info->end; l++) {

      if (s==0) {
        for (j=0; j<ch; j++) {
          int temp=vorbis_book_decode(look->phrasebook, &vb->opb);
          if (temp==-1) goto eopbreak;
          partword[j][l]=look->decodemap[temp];
          if (partword[j][l]==NULL) goto errout;
        }
      }

      for (k=0; k<partitions_per_word; k++, i+=samples_per_partition)
        for (j=0; j<ch; j++) {
          if (info->secondstages[partword[j][l][k]]&(1<<s)) {
            codebook *stagebook=look->partbooks[partword[j][l][k]][s];
            if (stagebook) {
              if (decodepart(stagebook, in[j]+i, &vb->opb,
                            samples_per_partition)==-1) goto eopbreak;
            }
          }
        }
    }
  }

 errout:
 eopbreak:
  return 0;
}

static void res0_free_info(vorbis_info_residue *i) {
  if (i) {
    memset(i, 0, sizeof(vorbis_info_residue0));
    _ogg_free(i);
  }
}

static vorbis_info_residue *res0_unpack(vorbis_info *vi, oggpack_buffer *opb) {
  int j, acc=0;
  vorbis_info_residue0 *info=_ogg_calloc(1, sizeof(vorbis_info_residue0));
  codec_setup_info     *ci=vi->codec_setup;

  info->begin=oggpack_read(opb, 24);
  info->end=oggpack_read(opb, 24);
  info->grouping=oggpack_read(opb, 24)+1;
  info->partitions=oggpack_read(opb, 6)+1;
  info->groupbook=oggpack_read(opb, 8);

  for (j=0; j<info->partitions; j++) {
    int cascade=oggpack_read(opb, 3);
    if (oggpack_read(opb, 1)) cascade|=oggpack_read(opb, 5)<<3;
    info->secondstages[j]=cascade;

    acc+=icount(cascade);
  }
  for (j=0; j<acc; j++)
    info->booklist[j]=oggpack_read(opb, 8);

  if (info->groupbook>=ci->books) goto errout;
  for (j=0; j<acc; j++)
    if (info->booklist[j]>=ci->books) goto errout;

  return info;
 errout:
  res0_free_info(info);
  return NULL;
}

static vorbis_look_residue *res0_look (vorbis_dsp_state *vd, vorbis_info_mode *vm,
                          vorbis_info_residue *vr) {
  vorbis_info_residue0 *info=(vorbis_info_residue0 *)vr;
  vorbis_look_residue0 *look=_ogg_calloc(1, sizeof(vorbis_look_residue0));
  backend_lookup_state *be=vd->backend_state;

  int j, k, acc=0;
  int dim;
  int maxstage=0;
  look->info=info;
  look->map=vm->mapping;

  look->parts=info->partitions;
  look->fullbooks=be->fullbooks;
  look->phrasebook=be->fullbooks+info->groupbook;
  dim=look->phrasebook->dim;

  look->partbooks=_ogg_calloc(look->parts, sizeof(codebook **));

  for (j=0; j<look->parts; j++) {
    int stages=ilog(info->secondstages[j]);
    if (stages) {
      if (stages>maxstage) maxstage=stages;
      look->partbooks[j]=_ogg_calloc(stages, sizeof(codebook *));
      for (k=0; k<stages; k++)
        if (info->secondstages[j]&(1<<k))
          look->partbooks[j][k]=be->fullbooks+info->booklist[acc++];
    }
  }

  look->partvals=(int)floor(pow(look->parts, dim)+0.5f);
  look->stages=maxstage;
  look->decodemap=_ogg_malloc(look->partvals*sizeof(int *));
  for (j=0; j<look->partvals; j++) {
    long val=j;
    long mult=look->partvals/look->parts;
    look->decodemap[j]=_ogg_malloc(dim*sizeof(int));
    for (k=0; k<dim; k++) {
      long deco=val/mult;
      val-=deco*mult;
      mult/=look->parts;
      look->decodemap[j][k]=deco;
    }
  }

  return look;
}

static void res0_free_look(vorbis_look_residue *i) {
  int j;
  if (i) {

    vorbis_look_residue0 *look=(vorbis_look_residue0 *)i;
    vorbis_info_residue0 *info=look->info;

    for (j=0; j<look->parts; j++)
      if (look->partbooks[j]) _ogg_free(look->partbooks[j]);
    _ogg_free(look->partbooks);
    for (j=0; j<look->partvals; j++)
      _ogg_free(look->decodemap[j]);
    _ogg_free(look->decodemap);
    memset(i, 0, sizeof(vorbis_look_residue0));
    _ogg_free(i);
  }
}

static int res0_inverse(vorbis_block *vb, vorbis_look_residue *vl,
                 float **in, int *nonzero, int ch) {
  int i, used=0;
  for (i=0; i<ch; i++)
    if (nonzero[i])
      in[used++]=in[i];
  if (used)
    return _01inverse(vb, vl, in, used, vorbis_book_decodevs_add);
  else
    return 0;
}

static int res1_inverse(vorbis_block *vb, vorbis_look_residue *vl,
                 float **in, int *nonzero, int ch) {
  int i, used=0;
  for (i=0; i<ch; i++)
    if (nonzero[i])
      in[used++]=in[i];
  if (used)
    return _01inverse(vb, vl, in, used, vorbis_book_decodev_add);
  else
    return 0;
}

static int res2_inverse(vorbis_block *vb, vorbis_look_residue *vl,
                 float **in, int *nonzero, int ch) {
  long i, k, l, s;
  vorbis_look_residue0 *look=(vorbis_look_residue0 *)vl;
  vorbis_info_residue0 *info=look->info;

  int samples_per_partition=info->grouping;
  int partitions_per_word=look->phrasebook->dim;
  int n=info->end-info->begin;

  int partvals=n/samples_per_partition;
  int partwords=(partvals+partitions_per_word-1)/partitions_per_word;
  int **partword=_vorbis_block_alloc(vb, partwords*sizeof(int *));
  partvals=partwords*partitions_per_word;

  for (i=0; i<ch; i++) if (nonzero[i]) break;
  if (i==ch) return 0;

  for (s=0; s<look->stages; s++) {
    for (i=info->begin, l=0; i<info->end; l++) {

      if (s==0) {
        int temp=vorbis_book_decode(look->phrasebook, &vb->opb);
        if (temp==-1) goto eopbreak;
        partword[l]=look->decodemap[temp];
        if (partword[l]==NULL) goto errout;
      }

      for (k=0; k<partitions_per_word; k++, i+=samples_per_partition)
        if (info->secondstages[partword[l][k]]&(1<<s)) {
          codebook *stagebook=look->partbooks[partword[l][k]][s];

          if (stagebook) {
            if (vorbis_book_decodevv_add(stagebook, in, i, ch,
                                        &vb->opb, samples_per_partition)==-1)
              goto eopbreak;
          }
        }
    }
  }

 errout:
 eopbreak:
  return 0;
}

static void mapping0_free_info(vorbis_info_mapping *i) {
  if (i) {
    memset(i, 0, sizeof(vorbis_info_mapping0));
    _ogg_free(i);
  }
}

static vorbis_info_mapping *mapping0_unpack(vorbis_info *vi, oggpack_buffer *opb) {
  int i;
  vorbis_info_mapping0 *info=_ogg_calloc(1, sizeof(vorbis_info_mapping0));
  codec_setup_info     *ci=vi->codec_setup;
  memset(info, 0, sizeof(vorbis_info_mapping0));

  if (oggpack_read(opb, 1))
    info->submaps=oggpack_read(opb, 4)+1;
  else
    info->submaps=1;

  if (oggpack_read(opb, 1)) {
    info->coupling_steps=oggpack_read(opb, 8)+1;

    for (i=0; i<info->coupling_steps; i++) {
      int testM=info->coupling_mag[i]=oggpack_read(opb, ilog2(vi->channels));
      int testA=info->coupling_ang[i]=oggpack_read(opb, ilog2(vi->channels));

      if (testM<0 ||
         testA<0 ||
         testM==testA ||
         testM>=vi->channels ||
         testA>=vi->channels) goto err_out;
    }

  }

  if (oggpack_read(opb, 2)>0) goto err_out;

  if (info->submaps>1) {
    for (i=0; i<vi->channels; i++) {
      info->chmuxlist[i]=oggpack_read(opb, 4);
      if (info->chmuxlist[i]>=info->submaps) goto err_out;
    }
  }
  for (i=0; i<info->submaps; i++) {
    oggpack_read(opb, 8);
    info->floorsubmap[i]=oggpack_read(opb, 8);
    if (info->floorsubmap[i]>=ci->floors) goto err_out;
    info->residuesubmap[i]=oggpack_read(opb, 8);
    if (info->residuesubmap[i]>=ci->residues) goto err_out;
  }

  return info;

 err_out:
  mapping0_free_info(info);
  return NULL;
}

static int mapping0_inverse(vorbis_block *vb, vorbis_look_mapping *l) {
  vorbis_dsp_state     *vd=vb->vd;
  vorbis_info          *vi=vd->vi;
  codec_setup_info     *ci=vi->codec_setup;
  backend_lookup_state *b=vd->backend_state;
  vorbis_look_mapping0 *look=(vorbis_look_mapping0 *)l;
  vorbis_info_mapping0 *info=look->map;
  vorbis_info_mode     *mode=look->mode;
  int                   i, j;
  long                  n=vb->pcmend=ci->blocksizes[vb->W];

  float *window=b->window[vb->W][vb->lW][vb->nW][mode->windowtype];
  float **pcmbundle=alloca(sizeof(float *)*vi->channels);
  int    *zerobundle=alloca(sizeof(int)*vi->channels);

  int   *nonzero  =alloca(sizeof(int)*vi->channels);
  void **floormemo=alloca(sizeof(void *)*vi->channels);

  for (i=0; i<vi->channels; i++) {
    int submap=info->chmuxlist[i];
    floormemo[i]=look->floor_func[submap]->
      inverse1(vb, look->floor_look[submap]);
    if (floormemo[i])
      nonzero[i]=1;
    else
      nonzero[i]=0;
    memset(vb->pcm[i], 0, sizeof(float)*n/2);
  }

  for (i=0; i<info->coupling_steps; i++) {
    if (nonzero[info->coupling_mag[i]] ||
       nonzero[info->coupling_ang[i]]) {
      nonzero[info->coupling_mag[i]]=1;
      nonzero[info->coupling_ang[i]]=1;
    }
  }

  for (i=0; i<info->submaps; i++) {
    int ch_in_bundle=0;
    for (j=0; j<vi->channels; j++) {
      if (info->chmuxlist[j]==i) {
        if (nonzero[j])
          zerobundle[ch_in_bundle]=1;
        else
          zerobundle[ch_in_bundle]=0;
        pcmbundle[ch_in_bundle++]=vb->pcm[j];
      }
    }

    look->residue_func[i]->inverse(vb, look->residue_look[i],
                                   pcmbundle, zerobundle, ch_in_bundle);
  }

  for (i=info->coupling_steps-1; i>=0; i--) {
    float *pcmM=vb->pcm[info->coupling_mag[i]];
    float *pcmA=vb->pcm[info->coupling_ang[i]];

    for (j=0; j<n/2; j++) {
      float mag=pcmM[j];
      float ang=pcmA[j];

      if (mag>0)
        if (ang>0) {
          pcmM[j]=mag;
          pcmA[j]=mag-ang;
        } else {
          pcmA[j]=mag;
          pcmM[j]=mag+ang;
        }
      else
        if (ang>0) {
          pcmM[j]=mag;
          pcmA[j]=mag+ang;
        } else {
          pcmA[j]=mag;
          pcmM[j]=mag-ang;
        }
    }
  }

  for (i=0; i<vi->channels; i++) {
    float *pcm=vb->pcm[i];
    int submap=info->chmuxlist[i];
    look->floor_func[submap]->
      inverse2(vb, look->floor_look[submap], floormemo[i], pcm);
  }

  for (i=0; i<vi->channels; i++) {
    float *pcm=vb->pcm[i];
    mdct_backward(b->transform[vb->W][0], pcm, pcm);
  }

  for (i=0; i<vi->channels; i++) {
    float *pcm=vb->pcm[i];
    if (nonzero[i])
      for (j=0; j<n; j++)
        pcm[j]*=window[j];
    else
      for (j=0; j<n; j++)
        pcm[j]=0.f;
  }

  return 0;
}

static void mapping0_free_look(vorbis_look_mapping *look) {
  int i;
  vorbis_look_mapping0 *l=(vorbis_look_mapping0 *)look;
  if (l) {
    for (i=0; i<l->map->submaps; i++) {
      l->floor_func[i]->free_look(l->floor_look[i]);
      l->residue_func[i]->free_look(l->residue_look[i]);
    }

    _ogg_free(l->floor_func);
    _ogg_free(l->residue_func);
    _ogg_free(l->floor_look);
    _ogg_free(l->residue_look);
    memset(l, 0, sizeof(vorbis_look_mapping0));
    _ogg_free(l);
  }
}

static vorbis_func_floor floor0_exportbundle={
  &floor0_unpack, &floor0_look,
  &floor0_free_info,
  &floor0_free_look,
  &floor0_inverse1, &floor0_inverse2
};
static vorbis_func_floor floor1_exportbundle={
  &floor1_unpack, &floor1_look,
  &floor1_free_info,
  &floor1_free_look,
  &floor1_inverse1, &floor1_inverse2
};
static vorbis_func_floor *_floor_P[]={
  &floor0_exportbundle,
  &floor1_exportbundle,
};

static vorbis_func_residue residue0_exportbundle={
  &res0_unpack,
  &res0_look,
  &res0_free_info,
  &res0_free_look,
  &res0_inverse
};

static vorbis_func_residue residue1_exportbundle={
  &res0_unpack,
  &res0_look,
  &res0_free_info,
  &res0_free_look,
  &res1_inverse
};

static vorbis_func_residue residue2_exportbundle={
  &res0_unpack,
  &res0_look,
  &res0_free_info,
  &res0_free_look,
  &res2_inverse
};
static vorbis_func_residue *_residue_P[]={
  &residue0_exportbundle,
  &residue1_exportbundle,
  &residue2_exportbundle,
};

static vorbis_look_mapping *mapping0_look(vorbis_dsp_state *vd, vorbis_info_mode *vm,
                          vorbis_info_mapping *m) {
  int i;
  vorbis_info          *vi=vd->vi;
  codec_setup_info     *ci=vi->codec_setup;
  vorbis_look_mapping0 *look=_ogg_calloc(1, sizeof(vorbis_look_mapping0));
  vorbis_info_mapping0 *info=look->map=(vorbis_info_mapping0 *)m;
  look->mode=vm;

  look->floor_look=_ogg_calloc(info->submaps, sizeof(vorbis_look_floor *));

  look->residue_look=_ogg_calloc(info->submaps, sizeof(vorbis_look_residue *));

  look->floor_func=_ogg_calloc(info->submaps, sizeof(vorbis_func_floor *));
  look->residue_func=_ogg_calloc(info->submaps, sizeof(vorbis_func_residue *));

  for (i=0; i<info->submaps; i++) {
    int floornum=info->floorsubmap[i];
    int resnum=info->residuesubmap[i];

    look->floor_func[i]=_floor_P[ci->floor_type[floornum]];
    look->floor_look[i]=look->floor_func[i]->
      look(vd, vm, ci->floor_param[floornum]);
    look->residue_func[i]=_residue_P[ci->residue_type[resnum]];
    look->residue_look[i]=look->residue_func[i]->
      look(vd, vm, ci->residue_param[resnum]);

  }

  look->ch=vi->channels;

  return look;
}

/* We're 'LSb' endian; if we write a word but read individual bits,
   then we'll read the lsb first */
static unsigned long mask[]=
{0x00000000, 0x00000001, 0x00000003, 0x00000007, 0x0000000f,
 0x0000001f, 0x0000003f, 0x0000007f, 0x000000ff, 0x000001ff,
 0x000003ff, 0x000007ff, 0x00000fff, 0x00001fff, 0x00003fff,
 0x00007fff, 0x0000ffff, 0x0001ffff, 0x0003ffff, 0x0007ffff,
 0x000fffff, 0x001fffff, 0x003fffff, 0x007fffff, 0x00ffffff,
 0x01ffffff, 0x03ffffff, 0x07ffffff, 0x0fffffff, 0x1fffffff,
 0x3fffffff, 0x7fffffff, 0xffffffff };

static void oggpack_readinit(oggpack_buffer *b, unsigned char *buf, int bytes) {
  memset(b, 0, sizeof(oggpack_buffer));
  b->buffer=b->ptr=buf;
  b->storage=bytes;
}

static long oggpack_look(oggpack_buffer *b, int bits) {
  unsigned long ret;
  unsigned long m=mask[bits];

  bits+=b->endbit;

  if (b->endbyte+4>=b->storage) {
    if (b->endbyte+(bits-1)/8>=b->storage) return -1;
  }

  ret=b->ptr[0]>>b->endbit;
  if (bits>8) {
    ret|=b->ptr[1]<<(8-b->endbit);
    if (bits>16) {
      ret|=b->ptr[2]<<(16-b->endbit);
      if (bits>24) {
        ret|=b->ptr[3]<<(24-b->endbit);
        if (bits>32 && b->endbit)
          ret|=b->ptr[4]<<(32-b->endbit);
      }
    }
  }
  return m&ret;
}

static void oggpack_adv(oggpack_buffer *b, int bits) {
  bits+=b->endbit;
  b->ptr+=bits/8;
  b->endbyte+=bits/8;
  b->endbit=bits&7;
}

static long oggpack_read(oggpack_buffer *b, int bits) {
  unsigned long ret;
  unsigned long m=mask[bits];

  bits+=b->endbit;

  if (b->endbyte+4>=b->storage) {
    ret=(unsigned long)-1;
    if (b->endbyte+(bits-1)/8>=b->storage) goto overflow;
  }

  ret=b->ptr[0]>>b->endbit;
  if (bits>8) {
    ret|=b->ptr[1]<<(8-b->endbit);
    if (bits>16) {
      ret|=b->ptr[2]<<(16-b->endbit);
      if (bits>24) {
        ret|=b->ptr[3]<<(24-b->endbit);
        if (bits>32 && b->endbit) {
          ret|=b->ptr[4]<<(32-b->endbit);
        }
      }
    }
  }
  ret&=m;

 overflow:

  b->ptr+=bits/8;
  b->endbyte+=bits/8;
  b->endbit=bits&7;
  return ret;
}

static long oggpack_read1(oggpack_buffer *b) {
  unsigned long ret;

  if (b->endbyte>=b->storage) {
    ret=(unsigned long)-1;
    goto overflow;
  }

  ret=b->ptr[0]>>b->endbit&1;

 overflow:

  b->endbit++;
  if (b->endbit>7) {
    b->endbit=0;
    b->ptr++;
    b->endbyte++;
  }
  return ret;
}

static int ilog(unsigned int v) {
  int ret=0;
  while (v) {
    ret++;
    v>>=1;
  }
  return ret;
}

static int ilog2(unsigned int v) {
  int ret=0;
  while (v>1) {
    ret++;
    v>>=1;
  }
  return ret;
}

static int vorbis_block_init(vorbis_dsp_state *v, vorbis_block *vb) {
  memset(vb, 0, sizeof(vorbis_block));
  vb->vd=v;
  vb->localalloc=0;
  vb->localstore=NULL;
  return 0;
}

static void *_vorbis_block_alloc(vorbis_block *vb, long bytes) {
  bytes=bytes+(8-1) & ~(8-1);
  if (bytes+vb->localtop>vb->localalloc) {
    if (vb->localstore) {
      struct alloc_chain *link=_ogg_malloc(sizeof(struct alloc_chain));
      vb->totaluse+=vb->localtop;
      link->next=vb->reap;
      link->ptr=vb->localstore;
      vb->reap=link;
    }
    vb->localalloc=bytes;
    vb->localstore=_ogg_malloc(vb->localalloc);
    vb->localtop=0;
  }
  {
    void *ret=(void *)(((char *) vb->localstore)+vb->localtop);
    vb->localtop+=bytes;
    return ret;
  }
}

static void _vorbis_block_ripcord(vorbis_block *vb) {
  struct alloc_chain *reap=vb->reap;
  while (reap) {
    struct alloc_chain *next=reap->next;
    _ogg_free(reap->ptr);
    memset(reap, 0, sizeof(struct alloc_chain));
    _ogg_free(reap);
    reap=next;
  }
  if (vb->totaluse) {
    vb->localstore=_ogg_realloc(vb->localstore, vb->totaluse+vb->localalloc);
    vb->localalloc+=vb->totaluse;
    vb->totaluse=0;
  }

  vb->localtop=0;
  vb->reap=NULL;
}

static int vorbis_block_clear(vorbis_block *vb) {
  if (vb->vd)
  _vorbis_block_ripcord(vb);
  if (vb->localstore) _ogg_free(vb->localstore);
  if (vb->internal) _ogg_free(vb->internal);

  memset(vb, 0, sizeof(vorbis_block));
  return 0;
}

static int _vds_shared_init(vorbis_dsp_state *v, vorbis_info *vi) {
  int i;
  codec_setup_info *ci=vi->codec_setup;
  backend_lookup_state *b=NULL;

  memset(v, 0, sizeof(vorbis_dsp_state));
  b=v->backend_state=_ogg_calloc(1, sizeof(backend_lookup_state));

  v->vi=vi;
  b->modebits=ilog2(ci->modes);
  b->ampmax=-9999;

  b->transform[0]=_ogg_calloc(1, sizeof(vorbis_look_transform *));
  b->transform[1]=_ogg_calloc(1, sizeof(vorbis_look_transform *));

  b->transform[0][0]=_ogg_calloc(1, sizeof(mdct_lookup));
  b->transform[1][0]=_ogg_calloc(1, sizeof(mdct_lookup));
  mdct_init(b->transform[0][0], ci->blocksizes[0]);
  mdct_init(b->transform[1][0], ci->blocksizes[1]);

  b->window[0][0][0]=_ogg_calloc(1, sizeof(float *));
  b->window[0][0][1]=b->window[0][0][0];
  b->window[0][1][0]=b->window[0][0][0];
  b->window[0][1][1]=b->window[0][0][0];
  b->window[1][0][0]=_ogg_calloc(1, sizeof(float *));
  b->window[1][0][1]=_ogg_calloc(1, sizeof(float *));
  b->window[1][1][0]=_ogg_calloc(1, sizeof(float *));
  b->window[1][1][1]=_ogg_calloc(1, sizeof(float *));

  for (i=0; i<1; i++) {
    b->window[0][0][0][i]=
      _vorbis_window(i, ci->blocksizes[0], ci->blocksizes[0]/2, ci->blocksizes[0]/2);
    b->window[1][0][0][i]=
      _vorbis_window(i, ci->blocksizes[1], ci->blocksizes[0]/2, ci->blocksizes[0]/2);
    b->window[1][0][1][i]=
      _vorbis_window(i, ci->blocksizes[1], ci->blocksizes[0]/2, ci->blocksizes[1]/2);
    b->window[1][1][0][i]=
      _vorbis_window(i, ci->blocksizes[1], ci->blocksizes[1]/2, ci->blocksizes[0]/2);
    b->window[1][1][1][i]=
      _vorbis_window(i, ci->blocksizes[1], ci->blocksizes[1]/2, ci->blocksizes[1]/2);
  }

    /* finish the codebooks */
    b->fullbooks=_ogg_calloc(ci->books, sizeof(codebook));
    for (i=0; i<ci->books; i++)
      vorbis_book_init_decode(b->fullbooks+i, ci->book_param[i]);

  /* initialize the storage vectors to a decent size greater than the
     minimum */

  v->pcm_storage=8192; /* we'll assume later that we have
                          a minimum of twice the blocksize of
                          accumulated samples in analysis */
  v->pcm=_ogg_malloc(vi->channels*sizeof(float *));
  v->pcmret=_ogg_malloc(vi->channels*sizeof(float *));
  {
    int i;
    for (i=0; i<vi->channels; i++)
      v->pcm[i]=_ogg_calloc(v->pcm_storage, sizeof(float));
  }

  /* all 1 (large block) or 0 (small block) */
  /* explicitly set for the sake of clarity */
  v->lW=0; /* previous window size */
  v->W=0;  /* current window size */

  /* all vector indexes */
  v->centerW=ci->blocksizes[1]/2;

  v->pcm_current=v->centerW;

  /* initialize all the mapping/backend lookups */
  b->mode=_ogg_calloc(ci->modes, sizeof(vorbis_look_mapping *));
  for (i=0; i<ci->modes; i++) {
    int mapnum=ci->mode_param[i]->mapping;
    b->mode[i]=mapping0_look(v, ci->mode_param[i],
                                         ci->map_param[mapnum]);
  }

  return 0;
}

static void vorbis_dsp_clear(vorbis_dsp_state *v) {
  int i, j, k;
  if (v) {
    vorbis_info *vi=v->vi;
    codec_setup_info *ci=vi?vi->codec_setup:NULL;
    backend_lookup_state *b=v->backend_state;

    if (b) {
      if (b->window[0][0][0]) {
        for (i=0; i<1; i++)
          if (b->window[0][0][0][i]) _ogg_free(b->window[0][0][0][i]);
        _ogg_free(b->window[0][0][0]);

        for (j=0; j<2; j++)
          for (k=0; k<2; k++) {
            for (i=0; i<1; i++)
              if (b->window[1][j][k][i]) _ogg_free(b->window[1][j][k][i]);
            _ogg_free(b->window[1][j][k]);
          }
      }

      if (b->transform[0]) {
        mdct_clear(b->transform[0][0]);
        _ogg_free(b->transform[0][0]);
        _ogg_free(b->transform[0]);
      }
      if (b->transform[1]) {
        mdct_clear(b->transform[1][0]);
        _ogg_free(b->transform[1][0]);
        _ogg_free(b->transform[1]);
      }

    }

    if (v->pcm) {
      for (i=0; i<vi->channels; i++)
        if (v->pcm[i]) _ogg_free(v->pcm[i]);
      _ogg_free(v->pcm);
      if (v->pcmret) _ogg_free(v->pcmret);
    }

    if (ci) {
      for (i=0; i<ci->modes; i++) {
        int mapnum=ci->mode_param[i]->mapping;
        if (b && b->mode) mapping0_free_look(b->mode[i]);
      }
      for (i=0; i<ci->books; i++)
        if (b && b->fullbooks) vorbis_book_clear(b->fullbooks+i);
    }

    if (b) {
      if (b->mode) _ogg_free(b->mode);
      if (b->fullbooks) _ogg_free(b->fullbooks);

      _ogg_free(b);
    }

    memset(v, 0, sizeof(vorbis_dsp_state));
  }
}

static int vorbis_synthesis_init(vorbis_dsp_state *v, vorbis_info *vi) {
  codec_setup_info *ci=vi->codec_setup;
  _vds_shared_init(v, vi);

  v->pcm_returned=-1;
  v->granulepos=-1;
  v->sequence=-1;

  return 0;
}

static int vorbis_synthesis_blockin(vorbis_dsp_state *v, vorbis_block *vb) {
  vorbis_info *vi=v->vi;
  codec_setup_info *ci=vi->codec_setup;

  if (v->centerW>ci->blocksizes[1]/2 && v->pcm_returned>8192) {
    int shiftPCM=v->centerW-ci->blocksizes[1]/2;
    shiftPCM=v->pcm_returned<shiftPCM?v->pcm_returned:shiftPCM;

    v->pcm_current-=shiftPCM;
    v->centerW-=shiftPCM;
    v->pcm_returned-=shiftPCM;

    if (shiftPCM) {
      int i;
      for (i=0; i<vi->channels; i++)
        memmove(v->pcm[i], v->pcm[i]+shiftPCM,
                v->pcm_current*sizeof(float));
    }
  }

  v->lW=v->W;
  v->W=vb->W;
  v->nW=-1;

  v->glue_bits+=vb->glue_bits;
  //v->time_bits+=vb->time_bits;
  v->floor_bits+=vb->floor_bits;
  v->res_bits+=vb->res_bits;

  if (v->sequence+1 != vb->sequence) v->granulepos=-1;
  v->sequence=vb->sequence;

  {
    int sizeW=ci->blocksizes[v->W];
    int centerW=v->centerW+ci->blocksizes[v->lW]/4+sizeW/4;
    int beginW=centerW-sizeW/2;
    int endW=beginW+sizeW;
    int beginSl;
    int endSl;
    int i, j;

    if (endW>v->pcm_storage) {
      v->pcm_storage=endW+ci->blocksizes[1];

      for (i=0; i<vi->channels; i++)
        v->pcm[i]=_ogg_realloc(v->pcm[i], v->pcm_storage*sizeof(float));
    }

    switch (v->W) {
      case 0:
        beginSl=0;
        endSl=ci->blocksizes[0]/2;
        break;
      case 1:
        beginSl=ci->blocksizes[1]/4-ci->blocksizes[v->lW]/4;
        endSl=beginSl+ci->blocksizes[v->lW]/2;
        break;
      default:
        return -1;
    }

    for (j=0; j<vi->channels; j++) {
      float *pcm=v->pcm[j]+beginW;
      float *p=vb->pcm[j];

      for (i=beginSl; i<endSl; i++) pcm[i]+=p[i];
      for (; i<sizeW; i++) pcm[i]=p[i];
    }

    if (v->pcm_returned==-1)
      v->pcm_returned=centerW;

    if (v->granulepos==-1) {
      if (vb->granulepos==-1) {
        v->granulepos=0;
      } else {
        v->granulepos=vb->granulepos;
      }
    } else {
      v->granulepos+=centerW-v->centerW;
      if (vb->granulepos!=-1 && v->granulepos!=vb->granulepos) {

        if (v->granulepos>vb->granulepos) {
          long extra=(long)(v->granulepos-vb->granulepos);

          if (vb->eofflag) {
            centerW-=extra;
          }else if (vb->sequence == 1) {
            v->pcm_returned+=extra;
            if (v->pcm_returned>centerW) v->pcm_returned=centerW;

          }

        }
        v->granulepos=vb->granulepos;
      }
    }

    v->centerW=centerW;
    v->pcm_current=endW;

    if (vb->eofflag) v->eofflag=1;
  }

  return 0;
}

static int vorbis_synthesis_pcmout(vorbis_dsp_state *v, float ***pcm) {
  vorbis_info *vi=v->vi;
  if (v->pcm_returned>-1 && v->pcm_returned<v->centerW) {
    if (pcm) {
      int i;
      for (i=0; i<vi->channels; i++)
        v->pcmret[i]=v->pcm[i]+v->pcm_returned;
      *pcm=v->pcmret;
    }
    return v->centerW-v->pcm_returned;
  }
  return 0;
}

static int vorbis_synthesis_read(vorbis_dsp_state *v, int bytes) {
  if (bytes && v->pcm_returned+bytes>v->centerW) return -131;
  v->pcm_returned+=bytes;
  return 0;
}

static int vorbis_staticbook_unpack(oggpack_buffer *opb, static_codebook *s) {
  long i, j;
  memset(s, 0, sizeof(static_codebook));
  s->allocedp=1;

  if (oggpack_read(opb, 24)!=0x564342) goto _eofout;

  s->dim=oggpack_read(opb, 16);
  s->entries=oggpack_read(opb, 24);
  if (s->entries==-1) goto _eofout;

  switch (oggpack_read(opb, 1)) {
    case 0:
      s->lengthlist=_ogg_malloc(sizeof(long)*s->entries);

      if (oggpack_read(opb, 1)) {

        for (i=0; i<s->entries; i++) {
          if (oggpack_read(opb, 1)) {
            long num=oggpack_read(opb, 5);
            if (num==-1) goto _eofout;
            s->lengthlist[i]=num+1;
          }else
            s->lengthlist[i]=0;
        }
      } else {
        for (i=0; i<s->entries; i++) {
          long num=oggpack_read(opb, 5);
          if (num==-1) goto _eofout;
          s->lengthlist[i]=num+1;
        }
      }

      break;
    case 1:
      {
        long length=oggpack_read(opb, 5)+1;
        s->lengthlist=_ogg_malloc(sizeof(long)*s->entries);

        for (i=0; i<s->entries; ) {
          long num=oggpack_read(opb, _ilog(s->entries-i));
          if (num==-1) goto _eofout;
          for (j=0; j<num; j++, i++)
            s->lengthlist[i]=length;
          length++;
        }
      }
      break;
    default:
      return -1;
  }

  switch (s->maptype=oggpack_read(opb, 4)) {
    case 0:
      break;
    case 1: case 2:

      s->q_min=oggpack_read(opb, 32);
      s->q_delta=oggpack_read(opb, 32);
      s->q_quant=oggpack_read(opb, 4)+1;
      s->q_sequencep=oggpack_read(opb, 1);

      {
        int quantvals;
        switch (s->maptype) {
        case 1:
          quantvals=_book_maptype1_quantvals(s);
          break;
        case 2:
          quantvals=s->entries*s->dim;
          break;
        }

        s->quantlist=_ogg_malloc(sizeof(long)*quantvals);
        for (i=0; i<quantvals; i++)
          s->quantlist[i]=oggpack_read(opb, s->q_quant);

        if (s->quantlist[quantvals-1]==-1) goto _eofout;
      }
      break;
    default:
      goto _errout;
  }

  return 0;

 _errout:
 _eofout:
  vorbis_staticbook_clear(s);
  return -1;
}

static long vorbis_book_decode(codebook *book, oggpack_buffer *b) {
  long ptr=0;
  decode_aux *t=book->decode_tree;
  int lok = oggpack_look(b, t->tabn);

  if (lok >= 0) {
    ptr = t->tab[lok];
    oggpack_adv(b, t->tabl[lok]);
    if (ptr <= 0)
      return -ptr;
  }

  do {
    switch (oggpack_read1(b)) {
      case 0: ptr=t->ptr0[ptr]; break;
      case 1: ptr=t->ptr1[ptr]; break;
      case -1: return -1;
    }
  } while (ptr>0);
  return -ptr;
}

static long vorbis_book_decodevs_add(codebook *book, float *a, oggpack_buffer *b, int n) {
  int step=n/book->dim;
  long *entry = alloca(sizeof(long)*step);
  float **t = alloca(sizeof(float *)*step);
  int i, j, o;

  for (i = 0; i < step; i++) {
    entry[i]=vorbis_book_decode(book, b);
    if (entry[i]==-1) return -1;
    t[i] = book->valuelist+entry[i]*book->dim;
  }
  for (i=0, o=0; i<book->dim; i++, o+=step)
    for (j=0; j<step; j++)
      a[o+j]+=t[j][i];
  return 0;
}

static long vorbis_book_decodev_add(codebook *book, float *a, oggpack_buffer *b, int n) {
  int i, j=0, entry;
  float *t;

  if (book->dim>8) {
    for (i=0; i<n; ) {
      entry = vorbis_book_decode(book, b);
      if (entry==-1) return -1;
      t     = book->valuelist+entry*book->dim;
      for (j=0; j<book->dim; )
        a[i++]+=t[j++];
    }
  } else {
    for (i=0; i<n; ) {
      entry = vorbis_book_decode(book, b);
      if (entry==-1) return -1;
      t     = book->valuelist+entry*book->dim;
      switch (book->dim) {
        case 8: a[i++]+=t[j++];
        case 7: a[i++]+=t[j++];
        case 6: a[i++]+=t[j++];
        case 5: a[i++]+=t[j++];
        case 4: a[i++]+=t[j++];
        case 3: a[i++]+=t[j++];
        case 2: a[i++]+=t[j++];
        case 1: a[i++]+=t[j++];
        case 0: break;
      }
    }
  }
  return 0;
}

static long vorbis_book_decodev_set(codebook *book, float *a, oggpack_buffer *b, int n) {
  int i, j, entry;
  float *t;

  for (i=0; i<n; ) {
    entry = vorbis_book_decode(book, b);
    if (entry==-1) return -1;
    t     = book->valuelist+entry*book->dim;
    for (j=0; j<book->dim; )
      a[i++]=t[j++];
  }
  return 0;
}

static long vorbis_book_decodevv_add(codebook *book, float **a, long offset, int ch,
                              oggpack_buffer *b, int n) {
  long i, j, entry;
  int chptr=0;

  for (i=offset/ch; i<(offset+n)/ch; ) {
    entry = vorbis_book_decode(book, b);
    if (entry==-1) return -1;
    {
      const float *t     = book->valuelist+entry*book->dim;
      for (j=0; j<book->dim; j++) {
        a[chptr++][i]+=t[j];
        if (chptr==ch) {
          chptr=0;
          i++;
        }
      }
    }
  }
  return 0;
}

static int ogg_page_version(ogg_page *og) {
  return (int)(og->header[4]);
}

static int ogg_page_continued(ogg_page *og) {
  return (int)(og->header[5]&0x01);
}

static int ogg_page_bos(ogg_page *og) {
  return (int)(og->header[5]&0x02);
}

static int ogg_page_eos(ogg_page *og) {
  return (int)(og->header[5]&0x04);
}

static ogg_int64_t ogg_page_granulepos(ogg_page *og) {
  unsigned char *page=og->header;
  ogg_int64_t granulepos=page[13]&(0xff);
  granulepos= (granulepos<<8)|(page[12]&0xff);
  granulepos= (granulepos<<8)|(page[11]&0xff);
  granulepos= (granulepos<<8)|(page[10]&0xff);
  granulepos= (granulepos<<8)|(page[9]&0xff);
  granulepos= (granulepos<<8)|(page[8]&0xff);
  granulepos= (granulepos<<8)|(page[7]&0xff);
  granulepos= (granulepos<<8)|(page[6]&0xff);
  return granulepos;
}

static int ogg_page_serialno(ogg_page *og) {
  return og->header[14] |
         og->header[15]<<8 |
         og->header[16]<<16 |
         og->header[17]<<24;
}

static long ogg_page_pageno(ogg_page *og) {
  return og->header[18] |
         og->header[19]<<8 |
         og->header[20]<<16 |
         og->header[21]<<24;
}

static ogg_uint32_t crc_lookup[256];
static int crc_ready=0;

static ogg_uint32_t _ogg_crc_entry(unsigned long index) {
  int           i;
  unsigned long r;

  r = index << 24;
  for (i=0; i<8; i++)
    if (r & 0x80000000UL)
      r = (r << 1) ^ 0x04c11db7;
    else
       r<<=1;
 return (r & 0xffffffffUL);
}

static void _ogg_crc_init(void) {
  if (!crc_ready) {
    int i;
    for (i=0; i<256; i++)
      crc_lookup[i]=_ogg_crc_entry((unsigned long) i);
    crc_ready=0;
  }
}

static int ogg_stream_init(ogg_stream_state *os, int serialno) {
  if (os) {
    memset(os, 0, sizeof(ogg_stream_state));
    os->body_storage=16*1024;
    os->body_data=_ogg_malloc(os->body_storage*sizeof(char));

    os->lacing_storage=1024;
    os->lacing_vals=_ogg_malloc(os->lacing_storage*sizeof(int));
    os->granule_vals=_ogg_malloc(os->lacing_storage*sizeof(ogg_int64_t));

    _ogg_crc_init();

    os->serialno=serialno;

    return 0;
  }
  return -1;
}

static int ogg_stream_clear(ogg_stream_state *os) {
  if (os) {
    if (os->body_data) _ogg_free(os->body_data);
    if (os->lacing_vals) _ogg_free(os->lacing_vals);
    if (os->granule_vals) _ogg_free(os->granule_vals);

    memset(os, 0, sizeof(ogg_stream_state));
  }
  return 0;
}

static void _os_body_expand(ogg_stream_state *os, int needed) {
  if (os->body_storage<=os->body_fill+needed) {
    os->body_storage+=needed+1024;
    os->body_data=_ogg_realloc(os->body_data, os->body_storage);
  }
}

static void _os_lacing_expand(ogg_stream_state *os, int needed) {
  if (os->lacing_storage<=os->lacing_fill+needed) {
    os->lacing_storage+=needed+32;
    os->lacing_vals=_ogg_realloc(os->lacing_vals, os->lacing_storage*sizeof(int));
    os->granule_vals=_ogg_realloc(os->granule_vals, os->lacing_storage*sizeof(ogg_int64_t));
  }
}

static void _os_checksum(ogg_page *og) {
  ogg_uint32_t crc_reg=0;
  int i;

  for (i=0; i<og->header_len; i++)
    crc_reg=crc_reg<<8^crc_lookup[((crc_reg >> 24)&0xff)^og->header[i]];
  for (i=0; i<og->body_len; i++)
    crc_reg=crc_reg<<8^crc_lookup[((crc_reg >> 24)&0xff)^og->body[i]];

  og->header[22]=(unsigned char)(crc_reg&0xff);
  og->header[23]=(unsigned char)(crc_reg>>8&0xff);
  og->header[24]=(unsigned char)(crc_reg>>16&0xff);
  og->header[25]=(unsigned char)(crc_reg>>24&0xff);
}

static int ogg_sync_init(ogg_sync_state *oy) {
  if (oy) {
    memset(oy, 0, sizeof(ogg_sync_state));
    _ogg_crc_init();
  }
  return 0;
}

static int ogg_sync_clear(ogg_sync_state *oy) {
  if (oy) {
    if (oy->data) _ogg_free(oy->data);
    ogg_sync_init(oy);
  }
  return 0;
}

static char *ogg_sync_buffer(ogg_sync_state *oy, long size) {

  if (oy->returned) {
    oy->fill-=oy->returned;
    if (oy->fill>0)
      memmove(oy->data, oy->data+oy->returned,
              (oy->fill)*sizeof(char));
    oy->returned=0;
  }

  if (size>oy->storage-oy->fill) {
    long newsize=size+oy->fill+4096;

    if (oy->data)
      oy->data=_ogg_realloc(oy->data, newsize);
    else
      oy->data=_ogg_malloc(newsize);
    oy->storage=newsize;
  }

  return (char *)oy->data+oy->fill;
}

static int ogg_sync_wrote(ogg_sync_state *oy, long bytes) {
  if (oy->fill+bytes>oy->storage) return -1;
  oy->fill+=bytes;
  return 0;
}

static long ogg_sync_pageseek(ogg_sync_state *oy, ogg_page *og) {
  unsigned char *page=oy->data+oy->returned;
  unsigned char *next;
  long bytes=oy->fill-oy->returned;

  if (oy->headerbytes==0) {
    int headerbytes, i;
    if (bytes<27) return 0;

    if (memcmp(page, "OggS", 4)) goto sync_fail;

    headerbytes=page[26]+27;
    if (bytes<headerbytes) return 0;

    for (i=0; i<page[26]; i++)
      oy->bodybytes+=page[27+i];
    oy->headerbytes=headerbytes;
  }

  if (oy->bodybytes+oy->headerbytes>bytes) return 0;

  {
    char chksum[4];
    ogg_page log;

    memcpy(chksum, page+22, 4);
    memset(page+22, 0, 4);

    log.header=page;
    log.header_len=oy->headerbytes;
    log.body=page+oy->headerbytes;
    log.body_len=oy->bodybytes;
    _os_checksum(&log);

    if (memcmp(chksum, page+22, 4)) {
      memcpy(page+22, chksum, 4);
      goto sync_fail;
    }
  }

  {
    unsigned char *page=oy->data+oy->returned;
    long bytes;

    if (og) {
      og->header=page;
      og->header_len=oy->headerbytes;
      og->body=page+oy->headerbytes;
      og->body_len=oy->bodybytes;
    }

    oy->unsynced=0;
    oy->returned+=(bytes=oy->headerbytes+oy->bodybytes);
    oy->headerbytes=0;
    oy->bodybytes=0;
    return bytes;
  }

 sync_fail:

  oy->headerbytes=0;
  oy->bodybytes=0;

  next=memchr(page+1, 'O', bytes-1);
  if (!next)
    next=oy->data+oy->fill;

  oy->returned=next-oy->data;
  return -(next-page);
}

static int ogg_sync_pageout(ogg_sync_state *oy, ogg_page *og) {

  for (;;) {
    long ret=ogg_sync_pageseek(oy, og);
    if (ret>0) {
      return 1;
    }
    if (ret==0) {
      return 0;
    }

    if (!oy->unsynced) {
      oy->unsynced=1;
      return -1;
    }

  }
}

/* clear things to an initial state.  Good to call, eg, before seeking */
int ogg_sync_reset(ogg_sync_state *oy){
  oy->fill=0;
  oy->returned=0;
  oy->unsynced=0;
  oy->headerbytes=0;
  oy->bodybytes=0;
  return(0);
}

static int ogg_stream_pagein(ogg_stream_state *os, ogg_page *og) {
  unsigned char *header=og->header;
  unsigned char *body=og->body;
  long           bodysize=og->body_len;
  int            segptr=0;

  int version=ogg_page_version(og);
  int continued=ogg_page_continued(og);
  int bos=ogg_page_bos(og);
  int eos=ogg_page_eos(og);
  ogg_int64_t granulepos=ogg_page_granulepos(og);
  int serialno=ogg_page_serialno(og);
  long pageno=ogg_page_pageno(og);
  int segments=header[26];

  {
    long lr=os->lacing_returned;
    long br=os->body_returned;

    if (br) {
      os->body_fill-=br;
      if (os->body_fill)
        memmove(os->body_data, os->body_data+br, os->body_fill);
      os->body_returned=0;
    }

    if (lr) {
      if (os->lacing_fill-lr) {
        memmove(os->lacing_vals, os->lacing_vals+lr,
                (os->lacing_fill-lr)*sizeof(int));
        memmove(os->granule_vals, os->granule_vals+lr,
                (os->lacing_fill-lr)*sizeof(ogg_int64_t));
      }
      os->lacing_fill-=lr;
      os->lacing_packet-=lr;
      os->lacing_returned=0;
    }
  }

  if (serialno!=os->serialno) return -1;
  if (version>0) return -1;

  _os_lacing_expand(os, segments+1);

  if (pageno!=os->pageno) {
    int i;

    for (i=os->lacing_packet; i<os->lacing_fill; i++)
      os->body_fill-=os->lacing_vals[i]&0xff;
    os->lacing_fill=os->lacing_packet;

    if (os->pageno!=-1) {
      os->lacing_vals[os->lacing_fill++]=0x400;
      os->lacing_packet++;
    }

    if (continued) {
      bos=0;
      for (; segptr<segments; segptr++) {
        int val=header[27+segptr];
        body+=val;
        bodysize-=val;
        if (val<255) {
          segptr++;
          break;
        }
      }
    }
  }

  if (bodysize) {
    _os_body_expand(os, bodysize);
    memcpy(os->body_data+os->body_fill, body, bodysize);
    os->body_fill+=bodysize;
  }

  {
    int saved=-1;
    while (segptr<segments) {
      int val=header[27+segptr];
      os->lacing_vals[os->lacing_fill]=val;
      os->granule_vals[os->lacing_fill]=-1;

      if (bos) {
        os->lacing_vals[os->lacing_fill]|=0x100;
        bos=0;
      }

      if (val<255) saved=os->lacing_fill;

      os->lacing_fill++;
      segptr++;

      if (val<255) os->lacing_packet=os->lacing_fill;
    }

    if (saved!=-1) {
      os->granule_vals[saved]=granulepos;
    }

  }

  if (eos) {
    os->e_o_s=1;
    if (os->lacing_fill>0)
      os->lacing_vals[os->lacing_fill-1]|=0x200;
  }

  os->pageno=pageno+1;

  return 0;
}

int ogg_stream_reset(ogg_stream_state *os){
  os->body_fill=0;
  os->body_returned=0;
  os->lacing_fill=0;
  os->lacing_packet=0;
  os->lacing_returned=0;
  os->e_o_s=0;
  os->b_o_s=0;
  os->pageno=-1;
  os->packetno=0;
  os->granulepos=0;
  return(0);
}


static int _packetout(ogg_stream_state *os, ogg_packet *op, int adv) {

  int ptr=os->lacing_returned;

  if (os->lacing_packet<=ptr) return 0;

  if (os->lacing_vals[ptr]&0x400) {
    os->lacing_returned++;
    os->packetno++;
    return -1;
  }

  if (!op && !adv) return 1;
  {
    int size=os->lacing_vals[ptr]&0xff;
    int bytes=size;
    int eos=os->lacing_vals[ptr]&0x200;
    int bos=os->lacing_vals[ptr]&0x100;

    while (size==255) {
      int val=os->lacing_vals[++ptr];
      size=val&0xff;
      if (val&0x200) eos=0x200;
      bytes+=size;
    }

    if (op) {
      op->e_o_s=eos;
      op->b_o_s=bos;
      op->packet=os->body_data+os->body_returned;
      op->packetno=os->packetno;
      op->granulepos=os->granule_vals[ptr];
      op->bytes=bytes;
    }

    if (adv) {
      os->body_returned+=bytes;
      os->lacing_returned=ptr+1;
      os->packetno++;
    }
  }
  return 1;
}

int ogg_stream_packetpeek(ogg_stream_state *os,ogg_packet *op){
  return _packetout(os,op,0);
}

static int ogg_stream_packetout(ogg_stream_state *os, ogg_packet *op) {
  return _packetout(os, op, 1);
}

static void _v_readstring(oggpack_buffer *o, char *buf, int bytes) {
  while (bytes--) *buf++=(char)oggpack_read(o, 8);
}

static void vorbis_info_init(vorbis_info *vi) {
  memset(vi, 0, sizeof(vorbis_info));
  vi->codec_setup=_ogg_calloc(1, sizeof(codec_setup_info));
}

static void vorbis_info_clear(vorbis_info *vi) {
  codec_setup_info     *ci=vi->codec_setup;
  int i;

  if (ci) {
    for (i=0; i<ci->modes; i++)
      if (ci->mode_param[i]) _ogg_free(ci->mode_param[i]);
    for (i=0; i<ci->maps; i++)
      mapping0_free_info(ci->map_param[i]);
    for (i=0; i<ci->floors; i++)
      _floor_P[ci->floor_type[i]]->free_info(ci->floor_param[i]);
    for (i=0; i<ci->residues; i++)
      _residue_P[ci->residue_type[i]]->free_info(ci->residue_param[i]);

    for (i=0; i<ci->books; i++) {
      if (ci->book_param[i]) {
        vorbis_staticbook_destroy(ci->book_param[i]);
      }
    }

    _ogg_free(ci);
  }

  memset(vi, 0, sizeof(vorbis_info));
}

static int _vorbis_unpack_info(vorbis_info *vi, oggpack_buffer *opb) {
  codec_setup_info     *ci=vi->codec_setup;
  if (!ci) return -129;

  vi->version=oggpack_read(opb, 32);
  if (vi->version!=0) return -134;

  vi->channels=oggpack_read(opb, 8);
  vi->rate=oggpack_read(opb, 32);

  vi->bitrate_upper=oggpack_read(opb, 32);
  vi->bitrate_nominal=oggpack_read(opb, 32);
  vi->bitrate_lower=oggpack_read(opb, 32);

  ci->blocksizes[0]=1<<oggpack_read(opb, 4);
  ci->blocksizes[1]=1<<oggpack_read(opb, 4);

  if (vi->rate<1) goto err_out;
  if (vi->channels<1) goto err_out;
  if (ci->blocksizes[0]<8) goto err_out;
  if (ci->blocksizes[1]<ci->blocksizes[0]) goto err_out;

  if (oggpack_read(opb, 1)!=1) goto err_out;

  return 0;
 err_out:
  vorbis_info_clear(vi);
  return -133;
}

static int _vorbis_unpack_books(vorbis_info *vi, oggpack_buffer *opb) {
  codec_setup_info     *ci=vi->codec_setup;
  int i;
  if (!ci) return -129;

  ci->books=oggpack_read(opb, 8)+1;
  for (i=0; i<ci->books; i++) {
    ci->book_param[i]=_ogg_calloc(1, sizeof(static_codebook));
    if (vorbis_staticbook_unpack(opb, ci->book_param[i])) goto err_out;
  }

  ci->times=oggpack_read(opb, 6)+1;
  for (i=0; i<ci->times; i++) oggpack_read(opb, 16);

  ci->floors=oggpack_read(opb, 6)+1;
  for (i=0; i<ci->floors; i++) {
    ci->floor_type[i]=oggpack_read(opb, 16);
    if (ci->floor_type[i]<0 || ci->floor_type[i]>=2) goto err_out;
    ci->floor_param[i]=_floor_P[ci->floor_type[i]]->unpack(vi, opb);
    if (!ci->floor_param[i]) goto err_out;
  }

  ci->residues=oggpack_read(opb, 6)+1;
  for (i=0; i<ci->residues; i++) {
    ci->residue_type[i]=oggpack_read(opb, 16);
    if (ci->residue_type[i]<0 || ci->residue_type[i]>=3) goto err_out;
    ci->residue_param[i]=_residue_P[ci->residue_type[i]]->unpack(vi, opb);
    if (!ci->residue_param[i]) goto err_out;
  }

  ci->maps=oggpack_read(opb, 6)+1;
  for (i=0; i<ci->maps; i++) {
    int typ=oggpack_read(opb, 16);
    if (typ<0 || typ>=1) goto err_out;
    ci->map_param[i]=mapping0_unpack(vi, opb);
    if (!ci->map_param[i]) goto err_out;
  }

  ci->modes=oggpack_read(opb, 6)+1;
  for (i=0; i<ci->modes; i++) {
    ci->mode_param[i]=_ogg_calloc(1, sizeof(vorbis_info_mode));
    ci->mode_param[i]->blockflag=oggpack_read(opb, 1);
    ci->mode_param[i]->windowtype=oggpack_read(opb, 16);
    ci->mode_param[i]->transformtype=oggpack_read(opb, 16);
    ci->mode_param[i]->mapping=oggpack_read(opb, 8);

    if (ci->mode_param[i]->windowtype>=1) goto err_out;
    if (ci->mode_param[i]->transformtype>=1) goto err_out;
    if (ci->mode_param[i]->mapping>=ci->maps) goto err_out;
  }

  if (oggpack_read(opb, 1)!=1) goto err_out;

  return 0;
 err_out:
  vorbis_info_clear(vi);
  return -133;
}

static int vorbis_synthesis_headerin(vorbis_info *vi, ogg_packet *op) {
  oggpack_buffer opb;

  if (op) {
    oggpack_readinit(&opb, op->packet, op->bytes);

    {
      char buffer[6];
      int packtype=oggpack_read(&opb, 8);
      memset(buffer, 0, 6);
      _v_readstring(&opb, buffer, 6);
      if (memcmp(buffer, "vorbis", 6)) return -132;
      switch (packtype) {
        case 0x01:
          if (!op->b_o_s) return -133;
          if (vi->rate!=0) return -133;
          return _vorbis_unpack_info(vi, &opb);

        case 0x03: /* least significant *bit* is read first */
         if (vi->rate==0) return -133;
         return 0;

        case 0x05:
          if (vi->rate==0) return -133;
          return _vorbis_unpack_books(vi, &opb);

        default:
          return -133;
          break;
      }
    }
  }
  return -133;
}

static void vorbis_lsp_to_curve(float *curve, int *map, int n, int ln, float *lsp, int m,
                            float amp, float ampoffset) {
  int i;
  float wdel=3.1415926536f/ln;
  for (i=0; i<m; i++) lsp[i]=2.f*(float)cos(lsp[i]);

  i=0;
  while (i<n) {
    int j, k=map[i];
    float p=.5f;
    float q=.5f;
    float w=2.f*(float)cos(wdel*k);
    for (j=1; j<m; j+=2) {
      q *= w-lsp[j-1];
      p *= w-lsp[j];
    }
    if (j==m) {
      q*=w-lsp[j-1];
      p*=p*(4.f-w*w);
      q*=q;
    } else {
      p*=p*(2.f-w);
      q*=q*(2.f+w);
    }

    q=(float)exp((amp/sqrt(p+q)-ampoffset)*.11512925f);

    curve[i]*=q;
    while (map[++i]==k) curve[i]*=q;
  }
}

static void mdct_init(mdct_lookup *lookup, int n) {
  int   *bitrev=_ogg_malloc(sizeof(int)*(n/4));
  float *T=_ogg_malloc(sizeof(float)*(n+n/4));

  int i;
  int n2=n>>1;
  int log2n=lookup->log2n=(int)floor(log(n)/log(2)+0.5f);
  lookup->n=n;
  lookup->trig=T;
  lookup->bitrev=bitrev;

  for (i=0; i<n/4; i++) {
    T[i*2]=(float)cos(((3.1415926536f)/n)*(4*i));
    T[i*2+1]=-(float)sin(((3.1415926536f)/n)*(4*i));
    T[n2+i*2]=(float)cos(((3.1415926536f)/(2*n))*(2*i+1));
    T[n2+i*2+1]=(float)sin(((3.1415926536f)/(2*n))*(2*i+1));
  }
  for (i=0; i<n/8; i++) {
    T[n+i*2]=(float)cos(((3.1415926536f)/n)*(4*i+2))*.5f;
    T[n+i*2+1]=-(float)sin(((3.1415926536f)/n)*(4*i+2))*.5f;
  }

  {
    int mask=(1<<log2n-1)-1, i, j;
    int msb=1<<log2n-2;
    for (i=0; i<n/8; i++) {
      int acc=0;
      for (j=0; msb>>j; j++)
        if (msb>>j&i) acc|=1<<j;
      bitrev[i*2]=(~acc&mask)-1;
      bitrev[i*2+1]=acc;

    }
  }
  lookup->scale=4.f/n;
}

static __inline void mdct_butterfly_8(float *x) {
  float r0   = x[6] + x[2];
  float r1   = x[6] - x[2];
  float r2   = x[4] + x[0];
  float r3   = x[4] - x[0];

           r0   = x[6] + x[2];
           r1   = x[6] - x[2];
           r2   = x[4] + x[0];
           r3   = x[4] - x[0];

           x[6] = r0   + r2;
           x[4] = r0   - r2;

           r0   = x[5] - x[1];
           r2   = x[7] - x[3];
           x[0] = r1   + r0;
           x[2] = r1   - r0;

           r0   = x[5] + x[1];
           r1   = x[7] + x[3];
           x[3] = r2   + r3;
           x[1] = r2   - r3;
           x[7] = r1   + r0;
           x[5] = r1   - r0;

}

static __inline void mdct_butterfly_16(float *x) {
  float r0     = x[1]  - x[9];
  float r1     = x[0]  - x[8];

           x[8]  += x[0];
           x[9]  += x[1];
           x[0]   = ((r0 + r1) * .70710678118654752441F);
           x[1]   = ((r0 - r1) * .70710678118654752441F);

           r0     = x[3]  - x[11];
           r1     = x[10] - x[2];
           x[10] += x[2];
           x[11] += x[3];
           x[2]   = r0;
           x[3]   = r1;

           r0     = x[12] - x[4];
           r1     = x[13] - x[5];
           x[12] += x[4];
           x[13] += x[5];
           x[4]   = ((r0 - r1) * .70710678118654752441F);
           x[5]   = ((r0 + r1) * .70710678118654752441F);

           r0     = x[14] - x[6];
           r1     = x[15] - x[7];
           x[14] += x[6];
           x[15] += x[7];
           x[6]  = r0;
           x[7]  = r1;

           mdct_butterfly_8(x);
           mdct_butterfly_8(x+8);
}

static __inline void mdct_butterfly_32(float *x) {
  float r0     = x[30] - x[14];
  float r1     = x[31] - x[15];

           x[30] +=         x[14];
           x[31] +=         x[15];
           x[14]  =         r0;
           x[15]  =         r1;

           r0     = x[28] - x[12];
           r1     = x[29] - x[13];
           x[28] +=         x[12];
           x[29] +=         x[13];
           x[12]  = (r0 * .92387953251128675613F - r1 * .38268343236508977175F);
           x[13]  = (r0 * .38268343236508977175F + r1 * .92387953251128675613F);

           r0     = x[26] - x[10];
           r1     = x[27] - x[11];
           x[26] +=         x[10];
           x[27] +=         x[11];
           x[10]  = (( r0 - r1 ) * .70710678118654752441F);
           x[11]  = (( r0 + r1 ) * .70710678118654752441F);

           r0     = x[24] - x[8];
           r1     = x[25] - x[9];
           x[24] += x[8];
           x[25] += x[9];
           x[8]   = (r0 * .38268343236508977175F - r1 * .92387953251128675613F);
           x[9]   = (r1 * .38268343236508977175F + r0 * .92387953251128675613F);

           r0     = x[22] - x[6];
           r1     = x[7]  - x[23];
           x[22] += x[6];
           x[23] += x[7];
           x[6]   = r1;
           x[7]   = r0;

           r0     = x[4]  - x[20];
           r1     = x[5]  - x[21];
           x[20] += x[4];
           x[21] += x[5];
           x[4]   = (r1 * .92387953251128675613F + r0 * .38268343236508977175F);
           x[5]   = (r1 * .38268343236508977175F - r0 * .92387953251128675613F);

           r0     = x[2]  - x[18];
           r1     = x[3]  - x[19];
           x[18] += x[2];
           x[19] += x[3];
           x[2]   = (( r1 + r0 ) * .70710678118654752441F);
           x[3]   = (( r1 - r0 ) * .70710678118654752441F);

           r0     = x[0]  - x[16];
           r1     = x[1]  - x[17];
           x[16] += x[0];
           x[17] += x[1];
           x[0]   = (r1 * .38268343236508977175F + r0 * .92387953251128675613F);
           x[1]   = (r1 * .92387953251128675613F - r0 * .38268343236508977175F);

           mdct_butterfly_16(x);
           mdct_butterfly_16(x+16);

}

static __inline void mdct_butterfly_first(float *T,
                                        float *x,
                                        int points) {

  float *x1        = x          + points      - 8;
  float *x2        = x          + (points>>1) - 8;
  float   r0;
  float   r1;

  do {

               r0      = x1[6]      -  x2[6];
               r1      = x1[7]      -  x2[7];
               x1[6]  += x2[6];
               x1[7]  += x2[7];
               x2[6]   = (r1 * T[1] + r0 * T[0]);
               x2[7]   = (r1 * T[0] - r0 * T[1]);

               r0      = x1[4]      -  x2[4];
               r1      = x1[5]      -  x2[5];
               x1[4]  += x2[4];
               x1[5]  += x2[5];
               x2[4]   = (r1 * T[5] + r0 * T[4]);
               x2[5]   = (r1 * T[4] - r0 * T[5]);

               r0      = x1[2]      -  x2[2];
               r1      = x1[3]      -  x2[3];
               x1[2]  += x2[2];
               x1[3]  += x2[3];
               x2[2]   = (r1 * T[9] + r0 * T[8]);
               x2[3]   = (r1 * T[8] - r0 * T[9]);

               r0      = x1[0]      -  x2[0];
               r1      = x1[1]      -  x2[1];
               x1[0]  += x2[0];
               x1[1]  += x2[1];
               x2[0]   = (r1 * T[13] + r0 * T[12]);
               x2[1]   = (r1 * T[12] - r0 * T[13]);

    x1-=8;
    x2-=8;
    T+=16;

  }while (x2>=x);
}

static __inline void mdct_butterfly_generic(float *T,
                                          float *x,
                                          int points,
                                          int trigint) {

  float *x1        = x          + points      - 8;
  float *x2        = x          + (points>>1) - 8;
  float   r0;
  float   r1;

  do {

               r0      = x1[6]      -  x2[6];
               r1      = x1[7]      -  x2[7];
               x1[6]  += x2[6];
               x1[7]  += x2[7];
               x2[6]   = (r1 * T[1] + r0 * T[0]);
               x2[7]   = (r1 * T[0] - r0 * T[1]);

               T+=trigint;

               r0      = x1[4]      -  x2[4];
               r1      = x1[5]      -  x2[5];
               x1[4]  += x2[4];
               x1[5]  += x2[5];
               x2[4]   = (r1 * T[1] + r0 * T[0]);
               x2[5]   = (r1 * T[0] - r0 * T[1]);

               T+=trigint;

               r0      = x1[2]      -  x2[2];
               r1      = x1[3]      -  x2[3];
               x1[2]  += x2[2];
               x1[3]  += x2[3];
               x2[2]   = (r1 * T[1] + r0 * T[0]);
               x2[3]   = (r1 * T[0] - r0 * T[1]);

               T+=trigint;

               r0      = x1[0]      -  x2[0];
               r1      = x1[1]      -  x2[1];
               x1[0]  += x2[0];
               x1[1]  += x2[1];
               x2[0]   = (r1 * T[1] + r0 * T[0]);
               x2[1]   = (r1 * T[0] - r0 * T[1]);

               T+=trigint;
    x1-=8;
    x2-=8;

  }while (x2>=x);
}

static __inline void mdct_butterflies(mdct_lookup *init,
                             float *x,
                             int points) {

  float *T=init->trig;
  int stages=init->log2n-5;
  int i, j;

  if (--stages>0) {
    mdct_butterfly_first(T, x, points);
  }

  for (i=1; --stages>0; i++) {
    for (j=0; j<(1<<i); j++)
      mdct_butterfly_generic(T, x+(points>>i)*j, points>>i, 4<<i);
  }

  for (j=0; j<points; j+=32)
    mdct_butterfly_32(x+j);

}

static void mdct_clear(mdct_lookup *l) {
  if (l) {
    if (l->trig) _ogg_free(l->trig);
    if (l->bitrev) _ogg_free(l->bitrev);
    memset(l, 0, sizeof(mdct_lookup));
  }
}

static __inline void mdct_bitreverse(mdct_lookup *init,
                            float *x) {
  int        n       = init->n;
  int       *bit     = init->bitrev;
  float *w0      = x;
  float *w1      = x = w0+(n>>1);
  float *T       = init->trig+n;

  do {
    float *x0    = x+bit[0];
    float *x1    = x+bit[1];

    float  r0     = x0[1]  - x1[1];
    float  r1     = x0[0]  + x1[0];
    float  r2     = (r1 * T[0] + r0 * T[1]);
    float  r3     = (r1 * T[1] - r0 * T[0]);

              w1    -= 4;

              r0     = ((x0[1] + x1[1])*.5f);
              r1     = ((x0[0] - x1[0])*.5f);

              w0[0]  = r0     + r2;
              w1[2]  = r0     - r2;
              w0[1]  = r1     + r3;
              w1[3]  = r3     - r1;

              x0     = x+bit[2];
              x1     = x+bit[3];

              r0     = x0[1]  - x1[1];
              r1     = x0[0]  + x1[0];
              r2     = (r1 * T[2] + r0 * T[3]);
              r3     = (r1 * T[3] - r0 * T[2]);

              r0     = ((x0[1] + x1[1])*.5f);
              r1     = ((x0[0] - x1[0])*.5f);

              w0[2]  = r0     + r2;
              w1[0]  = r0     - r2;
              w0[3]  = r1     + r3;
              w1[1]  = r3     - r1;

              T     += 4;
              bit   += 4;
              w0    += 4;

  }while (w0<w1);
}

static void mdct_backward(mdct_lookup *init, float *in, float *out) {
  int n=init->n;
  int n2=n>>1;
  int n4=n>>2;

  float *iX = in+n2-7;
  float *oX = out+n2+n4;
  float *T  = init->trig+n4;

  do {
    oX         -= 4;
    oX[0]       = (-iX[2] * T[3] - iX[0] * T[2]);
    oX[1]       = (iX[0] * T[3] - iX[2] * T[2]);
    oX[2]       = (-iX[6] * T[1] - iX[4] * T[0]);
    oX[3]       = (iX[4] * T[1] - iX[6] * T[0]);
    iX         -= 8;
    T          += 4;
  }while (iX>=in);

  iX            = in+n2-8;
  oX            = out+n2+n4;
  T             = init->trig+n4;

  do {
    T          -= 4;
    oX[0]       =  (iX[4] * T[3] + iX[6] * T[2]);
    oX[1]       =  (iX[4] * T[2] - iX[6] * T[3]);
    oX[2]       =  (iX[0] * T[1] + iX[2] * T[0]);
    oX[3]       =  (iX[0] * T[0] - iX[2] * T[1]);
    iX         -= 8;
    oX         += 4;
  }while (iX>=in);

  mdct_butterflies(init, out+n2, n2);
  mdct_bitreverse(init, out);

  {
    float *oX1=out+n2+n4;
    float *oX2=out+n2+n4;
    float *iX =out;
    T             =init->trig+n2;

    do {
      oX1-=4;

      oX1[3]  =  (iX[0] * T[1] - iX[1] * T[0]);
      oX2[0]  = -(iX[0] * T[0] + iX[1] * T[1]);

      oX1[2]  =  (iX[2] * T[3] - iX[3] * T[2]);
      oX2[1]  = -(iX[2] * T[2] + iX[3] * T[3]);

      oX1[1]  =  (iX[4] * T[5] - iX[5] * T[4]);
      oX2[2]  = -(iX[4] * T[4] + iX[5] * T[5]);

      oX1[0]  =  (iX[6] * T[7] - iX[7] * T[6]);
      oX2[3]  = -(iX[6] * T[6] + iX[7] * T[7]);

      oX2+=4;
      iX    +=   8;
      T     +=   8;
    }while (iX<oX1);

    iX=out+n2+n4;
    oX1=out+n4;
    oX2=oX1;

    do {
      oX1-=4;
      iX-=4;

      oX2[0] = -(oX1[3] = iX[3]);
      oX2[1] = -(oX1[2] = iX[2]);
      oX2[2] = -(oX1[1] = iX[1]);
      oX2[3] = -(oX1[0] = iX[0]);

      oX2+=4;
    }while (oX2<iX);

    iX=out+n2+n4;
    oX1=out+n2+n4;
    oX2=out+n2;
    do {
      oX1-=4;
      oX1[0]= iX[3];
      oX1[1]= iX[2];
      oX1[2]= iX[1];
      oX1[3]= iX[0];
      iX+=4;
    }while (oX1>oX2);
  }
}

static int _ilog(unsigned int v) {
  int ret=0;
  while (v) {
    ret++;
    v>>=1;
  }
  return ret;
}

static float _float32_unpack(long val) {
  double mant=val&0x1fffff;
  int    sign=val&0x80000000;
  long   exp =(val&0x7fe00000L)>>21;
  if (sign) mant= -mant;
  return (float)ldexp(mant, exp-(21-1)-768);
}

static long *_make_words(long *l, long n) {
  long i, j;
  long marker[33];
  long *r=_ogg_malloc(n*sizeof(long));
  memset(marker, 0, sizeof(marker));

  for (i=0; i<n; i++) {
    long length=l[i];
    if (length>0) {
      long entry=marker[length];

      if (length<32 && (entry>>length)) {
        _ogg_free(r);
        return NULL;
      }
      r[i]=entry;

      {
        for (j=length; j>0; j--) {

          if (marker[j]&1) {
            if (j==1)
              marker[1]++;
            else
              marker[j]=marker[j-1]<<1;
            break;
          }
          marker[j]++;
        }
      }

      for (j=length+1; j<33; j++)
        if ((marker[j]>>1) == entry) {
          entry=marker[j];
          marker[j]=marker[j-1]<<1;
        }else
          break;
    }
  }

  for (i=0; i<n; i++) {
    long temp=0;
    for (j=0; j<l[i]; j++) {
      temp<<=1;
      temp|=r[i]>>j&1;
    }
    r[i]=temp;
  }

  return r;
}

static decode_aux *_make_decode_tree(codebook *c) {
  const static_codebook *s=c->c;
  long top=0, i, j, n;
  decode_aux *t=_ogg_malloc(sizeof(decode_aux));
  long *ptr0=t->ptr0=_ogg_calloc(c->entries*2, sizeof(long));
  long *ptr1=t->ptr1=_ogg_calloc(c->entries*2, sizeof(long));
  long *codelist=_make_words(s->lengthlist, s->entries);

  if (codelist==NULL) return NULL;
  t->aux=c->entries*2;

  for (i=0; i<c->entries; i++) {
    if (s->lengthlist[i]>0) {
      long ptr=0;
      for (j=0; j<s->lengthlist[i]-1; j++) {
        int bit=codelist[i]>>j&1;
        if (!bit) {
          if (!ptr0[ptr]) ptr0[ptr]= ++top;
          ptr=ptr0[ptr];
        } else {
          if (!ptr1[ptr]) ptr1[ptr]= ++top;
          ptr=ptr1[ptr];
        }
      }
      if (!((codelist[i]>>j)&1))
        ptr0[ptr]=-i;
      else
        ptr1[ptr]=-i;
    }
  }
  _ogg_free(codelist);

  t->tabn = _ilog(c->entries)-4;
  if (t->tabn<5) t->tabn=5;
  n = 1<<t->tabn;
  t->tab = _ogg_malloc(n*sizeof(long));
  t->tabl = _ogg_malloc(n*sizeof(int));
  for (i = 0; i < n; i++) {
    long p = 0;
    for (j = 0; j < t->tabn && (p > 0 || j == 0); j++) {
      if (i & (1 << j))
        p = ptr1[p];
      else
        p = ptr0[p];
    }
    t->tab[i] = p;
    t->tabl[i] = j;
  }

  return t;
}

static long _book_maptype1_quantvals(const static_codebook *b) {
  long vals=(long)floor(pow(b->entries, 1.f/b->dim));

  for (;;) {
    long acc=1;
    long acc1=1;
    int i;
    for (i=0; i<b->dim; i++) {
      acc*=vals;
      acc1*=vals+1;
    }
    if (acc<=b->entries && acc1>b->entries) {
      return vals;
    } else {
      if (acc>b->entries) {
        vals--;
      } else {
        vals++;
      }
    }
  }
}

static float *_book_unquantize(const static_codebook *b) {
  long j, k;
  if (b->maptype==1 || b->maptype==2) {
    int quantvals;
    float mindel=_float32_unpack(b->q_min);
    float delta=_float32_unpack(b->q_delta);
    float *r=_ogg_calloc(b->entries*b->dim, sizeof(float));

    switch (b->maptype) {
      case 1:
        quantvals=_book_maptype1_quantvals(b);
        for (j=0; j<b->entries; j++) {
          float last=0.f;
          int indexdiv=1;
          for (k=0; k<b->dim; k++) {
            int index= (j/indexdiv)%quantvals;
            float val=(float)b->quantlist[index];
            val=(float)fabs(val)*delta+mindel+last;
            if (b->q_sequencep) last=val;
            r[j*b->dim+k]=val;
            indexdiv*=quantvals;
          }
        }
        break;
      case 2:
        for (j=0; j<b->entries; j++) {
          float last=0.f;
          for (k=0; k<b->dim; k++) {
            float val=(float)b->quantlist[j*b->dim+k];
            val=(float)fabs(val)*delta+mindel+last;
            if (b->q_sequencep) last=val;
            r[j*b->dim+k]=val;
          }
        }
        break;
    }

    return r;
  }
  return NULL;
}

static void vorbis_staticbook_clear(static_codebook *b) {
  if (b->allocedp) {
    if (b->quantlist) _ogg_free(b->quantlist);
    if (b->lengthlist) _ogg_free(b->lengthlist);
    memset(b, 0, sizeof(static_codebook));
  }
}

static void vorbis_staticbook_destroy(static_codebook *b) {
  if (b->allocedp) {
    vorbis_staticbook_clear(b);
    _ogg_free(b);
  }
}

static void vorbis_book_clear(codebook *b) {
  if (b->decode_tree) {
    _ogg_free(b->decode_tree->tab);
    _ogg_free(b->decode_tree->tabl);

    _ogg_free(b->decode_tree->ptr0);
    _ogg_free(b->decode_tree->ptr1);
    memset(b->decode_tree, 0, sizeof(decode_aux));
    _ogg_free(b->decode_tree);
  }
  if (b->valuelist) _ogg_free(b->valuelist);
  if (b->codelist) _ogg_free(b->codelist);
  memset(b, 0, sizeof(codebook));
}

static int vorbis_book_init_decode(codebook *c, const static_codebook *s) {
  memset(c, 0, sizeof(codebook));
  c->c=s;
  c->entries=s->entries;
  c->dim=s->dim;
  c->valuelist=_book_unquantize(s);
  c->decode_tree=_make_decode_tree(c);
  if (c->decode_tree==NULL) goto err_out;
  return 0;
 err_out:
  vorbis_book_clear(c);
  return -1;
}

static int vorbis_synthesis(vorbis_block *vb, ogg_packet *op) {
  vorbis_dsp_state     *vd=vb->vd;
  backend_lookup_state *b=vd->backend_state;
  vorbis_info          *vi=vd->vi;
  codec_setup_info     *ci=vi->codec_setup;
  oggpack_buffer       *opb=&vb->opb;
  int                   mode, i;

  _vorbis_block_ripcord(vb);
  oggpack_readinit(opb, op->packet, op->bytes);

  if (oggpack_read(opb, 1)!=0) {
    return -135;
  }

  mode=oggpack_read(opb, b->modebits);
  if (mode==-1) return -136;

  vb->mode=mode;
  vb->W=ci->mode_param[mode]->blockflag;
  if (vb->W) {
    vb->lW=oggpack_read(opb, 1);
    vb->nW=oggpack_read(opb, 1);
    if (vb->nW==-1) return -136;
  } else {
    vb->lW=0;
    vb->nW=0;
  }

  vb->granulepos=op->granulepos;
  vb->sequence=op->packetno-3;
  vb->eofflag=op->e_o_s;

  vb->pcmend=ci->blocksizes[vb->W];
  vb->pcm=_vorbis_block_alloc(vb, sizeof(float *)*vi->channels);
  for (i=0; i<vi->channels; i++)
    vb->pcm[i]=_vorbis_block_alloc(vb, vb->pcmend*sizeof(float));

  return mapping0_inverse(vb, b->mode[mode]);
}

static float *_vorbis_window(int type, int window, int left, int right) {
  float *ret=_ogg_calloc(window, sizeof(float));

  switch (type) {
    case 0:
      {
        int leftbegin=window/4-left/2;
        int rightbegin=window-window/4-right/2;
        int i;

        for (i=0; i<left; i++) {
          float x=(i+.5f)/left*3.1415926536f*.5f;
          x=(float)sin(x);
          x*=x;
          x*=(3.1415926536f)*.5f;
          x=(float)sin(x);
          ret[i+leftbegin]=x;
        }

        for (i=leftbegin+left; i<rightbegin; i++) ret[i]=1.f;

        for (i=0; i<right; i++) {
          float x=(right-i-.5f)/right*3.1415926536f*.5f;
          x=(float)sin(x);
          x*=x;
          x*=3.1415926536f*.5f;
          x=(float)sin(x);
          ret[i+rightbegin]=x;
        }
      }
      break;
    default:
      _ogg_free(ret);
      return NULL;
  }
  return ret;
}




/* A 'chained bitstream' is a Vorbis bitstream that contains more than
   one logical bitstream arranged end to end (the only form of Ogg
   multiplexing allowed in a Vorbis bitstream; grouping [parallel
   multiplexing] is not allowed in Vorbis) */

/* A Vorbis file can be played beginning to end (streamed) without
   worrying ahead of time about chaining (see decoder_example.c).  If
   we have the whole file, however, and want random access
   (seeking/scrubbing) or desire to know the total length/time of a
   file, we need to account for the possibility of chaining. */

/* We can handle things a number of ways; we can determine the entire
   bitstream structure right off the bat, or find pieces on demand.
   This example determines and caches structure for the entire
   bitstream, but builds a virtual decoder on the fly when moving
   between links in the chain. */

/* There are also different ways to implement seeking.  Enough
   information exists in an Ogg bitstream to seek to
   sample-granularity positions in the output.  Or, one can seek by
   picking some portion of the stream roughly in the desired area if
   we only want coarse navigation through the stream. */

/*************************************************************************
 * Many, many internal helpers.  The intention is not to be confusing;
 * rampant duplication and monolithic function implementation would be
 * harder to understand anyway.  The high level functions are last.  Begin
 * grokking near the end of the file */

/* read a little more data from the file/pipe into the ogg_sync framer
*/
#define CHUNKSIZE 8500 /* a shade over 8k; anyone using pages well
                          over 8k gets what they deserve */
static long _get_data(OggVorbis_File *vf){
  errno=0;
  if(vf->datasource){
    char *buffer=ogg_sync_buffer(&vf->oy,CHUNKSIZE);
    long bytes=(vf->callbacks.read_func)(buffer,1,CHUNKSIZE,vf->datasource);
    if(bytes>0)ogg_sync_wrote(&vf->oy,bytes);
    if(bytes==0 && errno)return(-1);
    return(bytes);
  }else
    return(0);
}

/* save a tiny smidge of verbosity to make the code more readable */
static void _seek_helper(OggVorbis_File *vf,long offset){
  if(vf->datasource){
    (vf->callbacks.seek_func)(vf->datasource, offset, SEEK_SET);
    vf->offset=offset;
    ogg_sync_reset(&vf->oy);
  }else{
    /* shouldn't happen unless someone writes a broken callback */
    return;
  }
}

/* The read/seek functions track absolute position within the stream */

/* from the head of the stream, get the next page.  boundary specifies
   if the function is allowed to fetch more data from the stream (and
   how much) or only use internally buffered data.

   boundary: -1) unbounded search
              0) read no additional data; use cached only
              n) search for a new page beginning for n bytes

   return:   <0) did not find a page (OV_FALSE, -2, -128)
              n) found a page at absolute offset n */

static long _get_next_page(OggVorbis_File *vf,ogg_page *og,int boundary){
  if(boundary>0)boundary+=vf->offset;
  while(1){
    long more;

    if(boundary>0 && vf->offset>=boundary)return -1;
    more=ogg_sync_pageseek(&vf->oy,og);

    if(more<0){
      /* skipped n bytes */
      vf->offset-=more;
    }else{
      if(more==0){
        /* send more paramedics */
        if(!boundary)return -1;
        {
          long ret=_get_data(vf);
          if(ret==0)return(-2);
          if(ret<0)return(-128);
        }
      }else{
        /* got a page.  Return the offset at the page beginning,
           advance the internal offset past the page end */
        long ret=vf->offset;
        vf->offset+=more;
        return(ret);

      }
    }
  }
}

/* find the latest page beginning before the current stream cursor
   position. Much dirtier than the above as Ogg doesn't have any
   backward search linkage.  no 'readp' as it will certainly have to
   read. */
/* returns offset or -128, OV_FAULT */
static long _get_prev_page(OggVorbis_File *vf,ogg_page *og){
  long begin=vf->offset;
  long ret;
  int offset=-1;

  while(offset==-1){
    begin-=CHUNKSIZE;
    _seek_helper(vf,begin);
    while(vf->offset<begin+CHUNKSIZE){
      ret=_get_next_page(vf,og,begin+CHUNKSIZE-vf->offset);
      if(ret==-128)return(-128);
      if(ret<0){
        break;
      }else{
        offset=ret;
      }
    }
  }

  /* we have the offset.  Actually snork and hold the page now */
  _seek_helper(vf,offset);
  ret=_get_next_page(vf,og,CHUNKSIZE);
  if(ret<0)
    /* this shouldn't be possible */
    return(-129);

  return(offset);
}

/* finds each bitstream link one at a time using a bisection search
   (has to begin by knowing the offset of the lb's initial page).
   Recurses for each link so it can alloc the link storage after
   finding them all, then unroll and fill the cache at the same time */
static int _bisect_forward_serialno(OggVorbis_File *vf,
                                    long begin,
                                    long searched,
                                    long end,
                                    long currentno,
                                    long m){
  long endsearched=end;
  long next=end;
  ogg_page og;
  long ret;

  /* the below guards against garbage seperating the last and
     first pages of two links. */
  while(searched<endsearched){
    long bisect;

    if(endsearched-searched<CHUNKSIZE){
      bisect=searched;
    }else{
      bisect=(searched+endsearched)/2;
    }

    _seek_helper(vf,bisect);
    ret=_get_next_page(vf,&og,-1);
    if(ret==-128)return(-128);
    if(ret<0 || ogg_page_serialno(&og)!=currentno){
      endsearched=bisect;
      if(ret>=0)next=ret;
    }else{
      searched=ret+og.header_len+og.body_len;
    }
  }

  _seek_helper(vf,next);
  ret=_get_next_page(vf,&og,-1);
  if(ret==-128)return(-128);

  if(searched>=end || ret<0){
    vf->links=m+1;
    vf->offsets=_ogg_malloc((m+2)*sizeof(ogg_int64_t));
    vf->offsets[m+1]=searched;
  }else{
    ret=_bisect_forward_serialno(vf,next,vf->offset,
                                 end,ogg_page_serialno(&og),m+1);
    if(ret==-128)return(-128);
  }

  vf->offsets[m]=begin;
  return(0);
}

/* uses the local ogg_stream storage in vf; this is important for
   non-streaming input sources */
static int _fetch_headers(OggVorbis_File *vf,vorbis_info *vi,
                          long *serialno,ogg_page *og_ptr){
  ogg_page og;
  ogg_packet op;
  int i,ret=0;

  if(!og_ptr){
    ret=_get_next_page(vf,&og,CHUNKSIZE);
    if(ret==-128)return(-128);
    if(ret<0)return -132;
    og_ptr=&og;
  }

  if(serialno)*serialno=ogg_page_serialno(og_ptr);
  ogg_stream_init(&vf->os,ogg_page_serialno(og_ptr));
  vf->ready_state=STREAMSET;

  /* extract the initial header from the first page and verify that the
     Ogg bitstream is in fact Vorbis data */

  vorbis_info_init(vi);

  i=0;
  while(i<3){
    ogg_stream_pagein(&vf->os,og_ptr);
    while(i<3){
      int result=ogg_stream_packetout(&vf->os,&op);
      if(result==0)break;
      if(result==-1){
        ret=-133;
        goto bail_header;
      }
      if((ret=vorbis_synthesis_headerin(vi,&op))){
        goto bail_header;
      }
      i++;
    }
    if(i<3)
      if(_get_next_page(vf,og_ptr,1)<0){
        ret=-133;
        goto bail_header;
      }
  }
  return 0;

 bail_header:
  vorbis_info_clear(vi);
  ogg_stream_clear(&vf->os);
  return ret;
}

/* last step of the OggVorbis_File initialization; get all the
   vorbis_info structs and PCM positions.  Only called by the seekable
   initialization (local stream storage is hacked slightly; pay
   attention to how that's done) */

/* this is void and does not propogate errors up because we want to be
   able to open and use damaged bitstreams as well as we can.  Just
   watch out for missing information for links in the OggVorbis_File
   struct */
static void _prefetch_all_headers(OggVorbis_File *vf, long dataoffset){
  ogg_page og;
  int i,ret;

  vf->vi=_ogg_realloc(vf->vi,vf->links*sizeof(vorbis_info));
  vf->dataoffsets=_ogg_malloc(vf->links*sizeof(ogg_int64_t));
  vf->pcmlengths=_ogg_malloc(vf->links*sizeof(ogg_int64_t));
  vf->serialnos=_ogg_malloc(vf->links*sizeof(long));

  for(i=0;i<vf->links;i++){
    if(i==0){
      /* we already grabbed the initial header earlier.  Just set the offset */
      vf->dataoffsets[i]=dataoffset;
    }else{

      /* seek to the location of the initial header */

      _seek_helper(vf,vf->offsets[i]);
      if(_fetch_headers(vf,vf->vi+i,NULL,NULL)<0){
            vf->dataoffsets[i]=-1;
      }else{
        vf->dataoffsets[i]=vf->offset;
        ogg_stream_clear(&vf->os);
      }
    }

    /* get the serial number and PCM length of this link. To do this,
       get the last page of the stream */
    {
      long end=vf->offsets[i+1];
      _seek_helper(vf,end);

      while(1){
        ret=_get_prev_page(vf,&og);
        if(ret<0){
          /* this should not be possible */
          vorbis_info_clear(vf->vi+i);
          break;
        }
        if(ogg_page_granulepos(&og)!=-1){
          vf->serialnos[i]=ogg_page_serialno(&og);
          vf->pcmlengths[i]=ogg_page_granulepos(&og);
          break;
        }
        vf->offset=ret;
      }
    }
  }
}

static void _make_decode_ready(OggVorbis_File *vf){
  if(vf->ready_state!=STREAMSET)return;
  if(vf->seekable){
    vorbis_synthesis_init(&vf->vd,vf->vi+vf->current_link);
  }else{
    vorbis_synthesis_init(&vf->vd,vf->vi);
  }
  vorbis_block_init(&vf->vd,&vf->vb);
  vf->ready_state=INITSET;
  return;
}

static int _open_seekable2(OggVorbis_File *vf){
  long serialno=vf->current_serialno,end;
  long dataoffset=vf->offset;
  ogg_page og;

  /* we're partially open and have a first link header state in
     storage in vf */
  /* we can seek, so set out learning all about this file */
  (vf->callbacks.seek_func)(vf->datasource,0,SEEK_END);
  vf->offset=vf->end=(vf->callbacks.tell_func)(vf->datasource);

  /* We get the offset for the last page of the physical bitstream.
     Most OggVorbis files will contain a single logical bitstream */
  end=_get_prev_page(vf,&og);
  if(end<0){
    ov_clear(vf);
    return(end);
  }

  /* more than one logical bitstream? */
  if(ogg_page_serialno(&og)!=serialno){

    /* Chained bitstream. Bisect-search each logical bitstream
       section.  Do so based on serial number only */
    if(_bisect_forward_serialno(vf,0,0,end+1,serialno,0)<0){
      ov_clear(vf);
      return(-128);
    }

  }else{

    /* Only one logical bitstream */
    if(_bisect_forward_serialno(vf,0,end,end+1,serialno,0)){
      ov_clear(vf);
      return(-128);
    }

  }

  /* the initial header memory is referenced by vf after; don't free it */
  _prefetch_all_headers(vf,dataoffset);
  return(ov_raw_seek(vf,0));
}

/* clear out the current logical bitstream decoder */
static void _decode_clear(OggVorbis_File *vf){
  ogg_stream_clear(&vf->os);
  vorbis_dsp_clear(&vf->vd);
  vorbis_block_clear(&vf->vb);
  vf->ready_state=OPENED;

  vf->bittrack=0.f;
  vf->samptrack=0.f;
}

/* fetch and process a packet.  Handles the case where we're at a
   bitstream boundary and dumps the decoding machine.  If the decoding
   machine is unloaded, it loads it.  It also keeps pcm_offset up to
   date (seek and read both use this.  seek uses a special hack with
   readp).

   return: <0) error, -3 (lost packet) or -2
            0) need more data (only if readp==0)
            1) got a packet
*/

static int _process_packet(OggVorbis_File *vf,int readp){
  ogg_page og;

  /* handle one packet.  Try to fetch it from current stream state */
  /* extract packets from page */
  while(1){

    /* process a packet if we can.  If the machine isn't loaded,
       neither is a page */
    if(vf->ready_state==INITSET){
      ogg_packet op;
      int result=ogg_stream_packetout(&vf->os,&op);
      ogg_int64_t granulepos;

      if(result==-1)return(-3); /* hole in the data. */
      if(result>0){
        /* got a packet.  process it */
        granulepos=op.granulepos;
        if(!vorbis_synthesis(&vf->vb,&op)){ /* lazy check for lazy
                                               header handling.  The
                                               header packets aren't
                                               audio, so if/when we
                                               submit them,
                                               vorbis_synthesis will
                                               reject them */

          /* suck in the synthesis data and track bitrate */
          {
            int oldsamples=vorbis_synthesis_pcmout(&vf->vd,NULL);
            vorbis_synthesis_blockin(&vf->vd,&vf->vb);
            vf->samptrack+=vorbis_synthesis_pcmout(&vf->vd,NULL)-oldsamples;
            vf->bittrack+=op.bytes*8;
          }

          /* update the pcm offset. */
          if(granulepos!=-1 && !op.e_o_s){
            int link=(vf->seekable?vf->current_link:0);
            int i,samples;

            /* this packet has a pcm_offset on it (the last packet
               completed on a page carries the offset) After processing
               (above), we know the pcm position of the *last* sample
               ready to be returned. Find the offset of the *first*

               As an aside, this trick is inaccurate if we begin
               reading anew right at the last page; the end-of-stream
               granulepos declares the last frame in the stream, and the
               last packet of the last page may be a partial frame.
               So, we need a previous granulepos from an in-sequence page
               to have a reference point.  Thus the !op.e_o_s clause
               above */

            samples=vorbis_synthesis_pcmout(&vf->vd,NULL);

            granulepos-=samples;
            for(i=0;i<link;i++)
              granulepos+=vf->pcmlengths[i];
            vf->pcm_offset=granulepos;
          }
          return(1);
        }
      }
    }

    if(vf->ready_state>=STREAMSET){
      if(!readp)return(0);
      if(_get_next_page(vf,&og,-1)<0)return(-2); /* eof.
                                                        leave unitialized */

      /* bitrate tracking; add the header's bytes here, the body bytes
         are done by packet above */
      vf->bittrack+=og.header_len*8;

      /* has our decoding just traversed a bitstream boundary? */
      if(vf->ready_state==INITSET){
        if(vf->current_serialno!=ogg_page_serialno(&og)){
          _decode_clear(vf);

          if(!vf->seekable){
            vorbis_info_clear(vf->vi);
          }
        }
      }
    }

    /* Do we need to load a new machine before submitting the page? */
    /* This is different in the seekable and non-seekable cases.

       In the seekable case, we already have all the header
       information loaded and cached; we just initialize the machine
       with it and continue on our merry way.

       In the non-seekable (streaming) case, we'll only be at a
       boundary if we just left the previous logical bitstream and
       we're now nominally at the header of the next bitstream
    */

    if(vf->ready_state!=INITSET){
      int link;

      if(vf->ready_state<STREAMSET){
        if(vf->seekable){
          vf->current_serialno=ogg_page_serialno(&og);

          /* match the serialno to bitstream section.  We use this rather than
             offset positions to avoid problems near logical bitstream
             boundaries */
          for(link=0;link<vf->links;link++)
            if(vf->serialnos[link]==vf->current_serialno)break;
          if(link==vf->links)return(-137); /* sign of a bogus
                                                     stream.  error out,
                                                     leave machine
                                                     uninitialized */

          vf->current_link=link;

          ogg_stream_init(&vf->os,vf->current_serialno);
          ogg_stream_reset(&vf->os);
          vf->ready_state=STREAMSET;

        }else{
          /* we're streaming */
          /* fetch the three header packets, build the info struct */

          _fetch_headers(vf,vf->vi,&vf->current_serialno,&og);
          vf->current_link++;
          link=0;
        }
      }

      _make_decode_ready(vf);
    }
    ogg_stream_pagein(&vf->os,&og);
  }
}

static int _fseek64_wrap(FILE *f,ogg_int64_t off,int whence){
  if(f==NULL)return(-1);
  return fseek(f,(int)off,whence);
}

static int _ov_open1(void *f,OggVorbis_File *vf,char *initial,
                     long ibytes, ov_callbacks callbacks){
  long offset=(f?callbacks.seek_func(f,0,SEEK_CUR):-1);
  int ret;

  memset(vf,0,sizeof(OggVorbis_File));
  vf->datasource=f;
  vf->callbacks = callbacks;

  /* init the framing state */
  ogg_sync_init(&vf->oy);

  /* perhaps some data was previously read into a buffer for testing
     against other stream types.  Allow initialization from this
     previously read data (as we may be reading from a non-seekable
     stream) */
  if(initial){
    char *buffer=ogg_sync_buffer(&vf->oy,ibytes);
    memcpy(buffer,initial,ibytes);
    ogg_sync_wrote(&vf->oy,ibytes);
  }

  /* can we seek? Stevens suggests the seek test was portable */
  if(offset!=-1)vf->seekable=1;

  /* No seeking yet; Set up a 'single' (current) logical bitstream
     entry for partial open */
  vf->links=1;
  vf->vi=_ogg_calloc(vf->links,sizeof(vorbis_info));

  /* Try to fetch the headers, maintaining all the storage */
  if((ret=_fetch_headers(vf,vf->vi,&vf->current_serialno,NULL))<0){
    vf->datasource=NULL;
    ov_clear(vf);
  }else if(vf->ready_state < PARTOPEN)
    vf->ready_state=PARTOPEN;
  return(ret);
}

static int _ov_open2(OggVorbis_File *vf){
  if(vf->ready_state < OPENED)
    vf->ready_state=OPENED;
  if(vf->seekable){
    int ret=_open_seekable2(vf);
    if(ret){
      vf->datasource=NULL;
      ov_clear(vf);
    }
    return(ret);
  }
  return 0;
}


/* clear out the OggVorbis_File struct */
int ov_clear(OggVorbis_File *vf){
  if(vf){
    vorbis_block_clear(&vf->vb);
    vorbis_dsp_clear(&vf->vd);
    ogg_stream_clear(&vf->os);

    if(vf->vi && vf->links){
      int i;
      for(i=0;i<vf->links;i++){
        vorbis_info_clear(vf->vi+i);
      }
      _ogg_free(vf->vi);
    }
    if(vf->dataoffsets)_ogg_free(vf->dataoffsets);
    if(vf->pcmlengths)_ogg_free(vf->pcmlengths);
    if(vf->serialnos)_ogg_free(vf->serialnos);
    if(vf->offsets)_ogg_free(vf->offsets);
    ogg_sync_clear(&vf->oy);
    if(vf->datasource)(vf->callbacks.close_func)(vf->datasource);
    memset(vf,0,sizeof(OggVorbis_File));
  }
#ifdef DEBUG_LEAKS
  _VDBG_dump();
#endif
  return(0);
}

/* inspects the OggVorbis file and finds/documents all the logical
   bitstreams contained in it.  Tries to be tolerant of logical
   bitstream sections that are truncated/woogie.

   return: -1) error
            0) OK
*/

int ov_open_callbacks(void *f,OggVorbis_File *vf,char *initial,long ibytes,
    ov_callbacks callbacks){
  int ret=_ov_open1(f,vf,initial,ibytes,callbacks);
  if(ret)return ret;
  return _ov_open2(vf);
}

int ov_open(FILE *f,OggVorbis_File *vf,char *initial,long ibytes){
  ov_callbacks callbacks = {
    (size_t (*)(void *, size_t, size_t, void *))  fread,
    (int (*)(void *, ogg_int64_t, int))              _fseek64_wrap,
    (int (*)(void *))                             fclose,
    (long (*)(void *))                            ftell
  };

  return ov_open_callbacks((void *)f, vf, initial, ibytes, callbacks);
}

/* Only partially open the vorbis file; test for Vorbisness, and load
   the headers for the first chain.  Do not seek (although test for
   seekability).  Use ov_test_open to finish opening the file, else
   ov_clear to close/free it. Same return codes as open. */

int ov_test_callbacks(void *f,OggVorbis_File *vf,char *initial,long ibytes,
    ov_callbacks callbacks)
{
  return _ov_open1(f,vf,initial,ibytes,callbacks);
}

int ov_test(FILE *f,OggVorbis_File *vf,char *initial,long ibytes){
  ov_callbacks callbacks = {
    (size_t (*)(void *, size_t, size_t, void *))  fread,
    (int (*)(void *, ogg_int64_t, int))              _fseek64_wrap,
    (int (*)(void *))                             fclose,
    (long (*)(void *))                            ftell
  };

  return ov_test_callbacks((void *)f, vf, initial, ibytes, callbacks);
}

int ov_test_open(OggVorbis_File *vf){
  if(vf->ready_state!=PARTOPEN)return(-131);
  return _ov_open2(vf);
}

/* How many logical bitstreams in this physical bitstream? */
long ov_streams(OggVorbis_File *vf){
  return vf->links;
}

/* Is the FILE * associated with vf seekable? */
long ov_seekable(OggVorbis_File *vf){
  return vf->seekable;
}

/* returns the bitrate for a given logical bitstream or the entire
   physical bitstream.  If the file is open for random access, it will
   find the *actual* average bitrate.  If the file is streaming, it
   returns the nominal bitrate (if set) else the average of the
   upper/lower bounds (if set) else -1 (unset).

   If you want the actual bitrate field settings, get them from the
   vorbis_info structs */

long ov_bitrate(OggVorbis_File *vf,int i){
  if(vf->ready_state<OPENED)return(-131);
  if(i>=vf->links)return(-131);
  if(!vf->seekable && i!=0)return(ov_bitrate(vf,0));
  if(i<0){
    ogg_int64_t bits=0;
    int i;
    for(i=0;i<vf->links;i++)
      bits+=(vf->offsets[i+1]-vf->dataoffsets[i])*8;
    return(rint(bits/ov_time_total(vf,-1)));
  }else{
    if(vf->seekable){
      /* return the actual bitrate */
      return(rint((vf->offsets[i+1]-vf->dataoffsets[i])*8/ov_time_total(vf,i)));
    }else{
      /* return nominal if set */
      if(vf->vi[i].bitrate_nominal>0){
        return vf->vi[i].bitrate_nominal;
      }else{
        if(vf->vi[i].bitrate_upper>0){
          if(vf->vi[i].bitrate_lower>0){
            return (vf->vi[i].bitrate_upper+vf->vi[i].bitrate_lower)/2;
          }else{
            return vf->vi[i].bitrate_upper;
          }
        }
        return -1;
      }
    }
  }
}

/* returns the actual bitrate since last call.  returns -1 if no
   additional data to offer since last call (or at beginning of stream),
   EINVAL if stream is only partially open
*/
long ov_bitrate_instant(OggVorbis_File *vf){
  int link=(vf->seekable?vf->current_link:0);
  long ret;
  if(vf->ready_state<OPENED)return(-131);
  if(vf->samptrack==0)return -1;
  ret=vf->bittrack/vf->samptrack*vf->vi[link].rate+.5;
  vf->bittrack=0.f;
  vf->samptrack=0.f;
  return(ret);
}

/* Guess */
long ov_serialnumber(OggVorbis_File *vf,int i){
  if(i>=vf->links)return(ov_serialnumber(vf,vf->links-1));
  if(!vf->seekable && i>=0)return(ov_serialnumber(vf,-1));
  if(i<0){
    return(vf->current_serialno);
  }else{
    return(vf->serialnos[i]);
  }
}

/* returns: total raw (compressed) length of content if i==-1
            raw (compressed) length of that logical bitstream for i==0 to n
            -131 if the stream is not seekable (we can't know the length)
            or if stream is only partially open
*/
ogg_int64_t ov_raw_total(OggVorbis_File *vf,int i){
  if(vf->ready_state<OPENED)return(-131);
  if(!vf->seekable || i>=vf->links)return(-131);
  if(i<0){
    long acc=0;
    int i;
    for(i=0;i<vf->links;i++)
      acc+=ov_raw_total(vf,i);
    return(acc);
  }else{
    return(vf->offsets[i+1]-vf->offsets[i]);
  }
}

/* returns: total PCM length (samples) of content if i==-1 PCM length
            (samples) of that logical bitstream for i==0 to n
            -131 if the stream is not seekable (we can't know the
            length) or only partially open
*/
ogg_int64_t ov_pcm_total(OggVorbis_File *vf,int i){
  if(vf->ready_state<OPENED)return(-131);
  if(!vf->seekable || i>=vf->links)return(-131);
  if(i<0){
    ogg_int64_t acc=0;
    int i;
    for(i=0;i<vf->links;i++)
      acc+=ov_pcm_total(vf,i);
    return(acc);
  }else{
    return(vf->pcmlengths[i]);
  }
}

/* returns: total seconds of content if i==-1
            seconds in that logical bitstream for i==0 to n
            -131 if the stream is not seekable (we can't know the
            length) or only partially open
*/
double ov_time_total(OggVorbis_File *vf,int i){
  if(vf->ready_state<OPENED)return(-131);
  if(!vf->seekable || i>=vf->links)return(-131);
  if(i<0){
    double acc=0;
    int i;
    for(i=0;i<vf->links;i++)
      acc+=ov_time_total(vf,i);
    return(acc);
  }else{
    return((float)(vf->pcmlengths[i])/vf->vi[i].rate);
  }
}

long vorbis_packet_blocksize(vorbis_info *vi,ogg_packet *op){
  codec_setup_info     *ci=vi->codec_setup;
  oggpack_buffer       opb;
  int                  type,mode,i;

  oggpack_readinit(&opb,op->packet,op->bytes);

  /* Check the packet type */
  if(oggpack_read(&opb,1)!=0){
    /* Oops.  This is not an audio data packet */
    return(-135);
  }

  {
    int modebits=0;
    int v=ci->modes;
    while(v>1){
      modebits++;
      v>>=1;
    }

    /* read our mode and pre/post windowsize */
    mode=oggpack_read(&opb,modebits);
  }
  if(mode==-1)return(-136);
  return(ci->blocksizes[ci->mode_param[mode]->blockflag]);
}

/* seek to an offset relative to the *compressed* data. This also
   scans packets to update the PCM cursor. It will cross a logical
   bitstream boundary, but only if it can't get any packets out of the
   tail of the bitstream we seek to (so no surprises).

   returns zero on success, nonzero on failure */

int ov_raw_seek(OggVorbis_File *vf,long pos){
  ogg_stream_state work_os;

  if(vf->ready_state<OPENED)return(-131);
  if(!vf->seekable)
    return(-138); /* don't dump machine if we can't seek */

  if(pos<0 || pos>vf->offsets[vf->links])return(-131);

  /* clear out decoding machine state */
  vf->pcm_offset=-1;
  _decode_clear(vf);

  _seek_helper(vf,pos);

  /* we need to make sure the pcm_offset is set, but we don't want to
     advance the raw cursor past good packets just to get to the first
     with a granulepos.  That's not equivalent behavior to beginning
     decoding as immediately after the seek position as possible.

     So, a hack.  We use two stream states; a local scratch state and
     a the shared vf->os stream state.  We use the local state to
     scan, and the shared state as a buffer for later decode.

     Unfortuantely, on the last page we still advance to last packet
     because the granulepos on the last page is not necessarily on a
     packet boundary, and we need to make sure the granpos is
     correct.
  */

  {
    ogg_page og;
    ogg_packet op;
    int lastblock=0;
    int accblock=0;
    int thisblock;
    int eosflag;

    memset(&work_os,0,sizeof(work_os));/* so that it's safe to clear
                                          it later even if we don't
                                          init it */

    while(1){
      if(vf->ready_state==STREAMSET){
        /* snarf/scan a packet if we can */
        int result=ogg_stream_packetout(&work_os,&op);

        if(result>0){

          if(vf->vi[vf->current_link].codec_setup)
            thisblock=vorbis_packet_blocksize(vf->vi+vf->current_link,&op);
          if(eosflag)
            ogg_stream_packetout(&vf->os,NULL);
          else
            if(lastblock)accblock+=(lastblock+thisblock)>>2;

          if(op.granulepos!=-1){
            int i,link=vf->current_link;
            ogg_int64_t granulepos=op.granulepos;

            for(i=0;i<link;i++)
              granulepos+=vf->pcmlengths[i];
            vf->pcm_offset=granulepos-accblock;
            break;
          }
          lastblock=thisblock;
          continue;
        }
      }

      if(!lastblock){
        if(_get_next_page(vf,&og,-1)<0){
          vf->pcm_offset=ov_pcm_total(vf,-1);
          break;
        }
      }else{
        /* huh?  Bogus stream with packets but no granulepos */
        vf->pcm_offset=-1;
        break;
      }

      /* has our decoding just traversed a bitstream boundary? */
      if(vf->ready_state==STREAMSET)
        if(vf->current_serialno!=ogg_page_serialno(&og)){
        _decode_clear(vf); /* clear out stream state */
        ogg_stream_clear(&work_os);
      }

      if(vf->ready_state<STREAMSET){
        int link;

        vf->current_serialno=ogg_page_serialno(&og);
        for(link=0;link<vf->links;link++)
          if(vf->serialnos[link]==vf->current_serialno)break;
        if(link==vf->links)goto seek_error; /* sign of a bogus stream.
                                               error out, leave
                                               machine uninitialized */
        vf->current_link=link;

        ogg_stream_init(&vf->os,vf->current_serialno);
        ogg_stream_reset(&vf->os);
        ogg_stream_init(&work_os,vf->current_serialno);
        ogg_stream_reset(&work_os);
        vf->ready_state=STREAMSET;

      }

      ogg_stream_pagein(&vf->os,&og);
      ogg_stream_pagein(&work_os,&og);
      eosflag=ogg_page_eos(&og);
    }
  }

  ogg_stream_clear(&work_os);
  return(0);

 seek_error:
  /* dump the machine so we're in a known state */
  vf->pcm_offset=-1;
  ogg_stream_clear(&work_os);
  _decode_clear(vf);
  return -137;
}

/* Page granularity seek (faster than sample granularity because we
   don't do the last bit of decode to find a specific sample).

   Seek to the last [granule marked] page preceeding the specified pos
   location, such that decoding past the returned point will quickly
   arrive at the requested position. */
int ov_pcm_seek_page(OggVorbis_File *vf,ogg_int64_t pos){
  int link=-1;
  long ret;
  ogg_int64_t total=ov_pcm_total(vf,-1);

  if(vf->ready_state<OPENED)return(-131);
  if(!vf->seekable)return(-138);
  if(pos<0 || pos>total)return(-131);

  /* which bitstream section does this pcm offset occur in? */
  for(link=vf->links-1;link>=0;link--){
    total-=vf->pcmlengths[link];
    if(pos>=total)break;
  }

  /* search within the logical bitstream for the page with the highest
     pcm_pos preceeding (or equal to) pos.  There is a danger here;
     missing pages or incorrect frame number information in the
     bitstream could make our task impossible.  Account for that (it
     would be an error condition) */

  /* new search algorithm by HB (Nicholas Vinen) */
  {
    ogg_int64_t target=pos-total;
    long end=vf->offsets[link+1];
    long begin=vf->offsets[link];
    ogg_int64_t endtime = vf->pcmlengths[link];
    ogg_int64_t begintime = 0;
    long best=begin;

    ogg_page og;
    while(begin<end){
      long bisect;

      if(end-begin<CHUNKSIZE){
        bisect=begin;
      }else{
        /* take a (pretty decent) guess. */
        bisect=begin +
          (target-begintime)*(end-begin)/(endtime-begintime) - CHUNKSIZE;
        if(bisect<=begin)
          bisect=begin+1;
      }
      _seek_helper(vf,bisect);

      while(begin<end){
        ret=_get_next_page(vf,&og,end-bisect);
        if(ret==-128) goto seek_error;
        if(ret<0){
          if(bisect<=begin+1)
            end=begin; /* found it */
          else{
            if(bisect==0)goto seek_error;
            bisect-=CHUNKSIZE;
            if(bisect<=begin)bisect=begin+1;
            _seek_helper(vf,bisect);
          }
        }else{
          ogg_int64_t granulepos=ogg_page_granulepos(&og);
          if(granulepos<target){
            best=ret;  /* raw offset of packet with granulepos */
            begin=vf->offset; /* raw offset of next page */
            begintime=granulepos;

            if(target-begin>44100)break;
            bisect=begin; /* *not* begin + 1 */
          }else{
            if(bisect<=begin+1)
              end=begin;  /* found it */
            else{
              if(end==vf->offset){ /* we're pretty close - we'd be stuck in */
                end=ret;
                bisect-=CHUNKSIZE; /* an endless loop otherwise. */
                if(bisect<=begin)bisect=begin+1;
                _seek_helper(vf,bisect);
              }else{
                end=ret;
                endtime=granulepos;
                break;
              }
            }
          }
        }
      }
    }

    /* found our page. seek to it, update pcm offset. Easier case than
       raw_seek, don't keep packets preceeding granulepos. */
    {
      ogg_page og;
      ogg_packet op;
      /* clear out decoding machine state */
      _decode_clear(vf);
      /* seek */
      _seek_helper(vf,best);

      if(_get_next_page(vf,&og,-1)<0)return(-2); /* shouldn't happen */
      vf->current_serialno=ogg_page_serialno(&og);
      vf->current_link=link;

      ogg_stream_init(&vf->os,vf->current_serialno);
      ogg_stream_reset(&vf->os);
      vf->ready_state=STREAMSET;
      ogg_stream_pagein(&vf->os,&og);

      /* pull out all but last packet; the one with granulepos */
      while(1){
        ret=ogg_stream_packetpeek(&vf->os,&op);
        if(ret==0){
          /* !!! the packet finishing this page originated on a
             preceeding page. Keep fetching previous pages until we
             get one with a granulepos or without the 'continued' flag
             set.  Then just use raw_seek for simplicity. */
          while(1){
            ret=_get_prev_page(vf,&og);
            if(ret<0)goto seek_error;
            if(ogg_page_granulepos(&og)>-1 ||
               !ogg_page_continued(&og)){
              return ov_raw_seek(vf,ret);
            }
            vf->offset=ret;
          }
        }
        if(ret<0)goto seek_error;
        if(op.granulepos!=-1){
          vf->pcm_offset=op.granulepos+total;
          break;
        }else
          ret=ogg_stream_packetout(&vf->os,NULL);
      }
    }
  }

  /* verify result */
  if(vf->pcm_offset>pos || pos>ov_pcm_total(vf,-1)){
    ret=-129;
    goto seek_error;
  }
  return(0);

 seek_error:
  /* dump machine so we're in a known state */
  vf->pcm_offset=-1;
  _decode_clear(vf);
  return ret;
}

/* seek to a sample offset relative to the decompressed pcm stream
   returns zero on success, nonzero on failure */

int ov_pcm_seek(OggVorbis_File *vf,ogg_int64_t pos){
  int thisblock,lastblock=0;
  int ret=ov_pcm_seek_page(vf,pos);
  codec_setup_info *ci=vf->vi->codec_setup;
  if(ret<0)return(ret);

  /* discard leading packets we don't need for the lapping of the
     position we want; don't decode them */

  while(1){
    ogg_packet op;
    ogg_page og;

    int ret=ogg_stream_packetpeek(&vf->os,&op);
    if(ret>0){
      thisblock=vorbis_packet_blocksize(vf->vi+vf->current_link,&op);
      if(lastblock)vf->pcm_offset+=(lastblock+thisblock)>>2;

      if(vf->pcm_offset+((thisblock+ci->blocksizes[1])>>2)>=pos)break;

      ogg_stream_packetout(&vf->os,NULL);


      /* end of logical stream case is hard, especially with exact
         length positioning. */

      if(op.granulepos>-1){
        int i;
        /* always believe the stream markers */
        vf->pcm_offset=op.granulepos;
        for(i=0;i<vf->current_link;i++)
          vf->pcm_offset+=vf->pcmlengths[i];
      }

      lastblock=thisblock;

    }else{
      if(ret<0 && ret!=-3)break;

      /* suck in a new page */
      if(_get_next_page(vf,&og,-1)<0)break;
      if(vf->current_serialno!=ogg_page_serialno(&og))_decode_clear(vf);

      if(vf->ready_state<STREAMSET){
        int link;

        vf->current_serialno=ogg_page_serialno(&og);
        for(link=0;link<vf->links;link++)
          if(vf->serialnos[link]==vf->current_serialno)break;
        if(link==vf->links)return(-137);
        vf->current_link=link;

        ogg_stream_init(&vf->os,vf->current_serialno);
        ogg_stream_reset(&vf->os);
        vf->ready_state=STREAMSET;
        lastblock=0;
      }
      ogg_stream_pagein(&vf->os,&og);
    }
  }

  /* discard samples until we reach the desired position. Crossing a
     logical bitstream boundary with abandon is OK. */
  _make_decode_ready(vf);
  while(vf->pcm_offset<pos){
    float **pcm;
    long target=pos-vf->pcm_offset;
    long samples=vorbis_synthesis_pcmout(&vf->vd,&pcm);

    if(samples>target)samples=target;
    vorbis_synthesis_read(&vf->vd,samples);
    vf->pcm_offset+=samples;

    if(samples<target)
      if(_process_packet(vf,1)<=0)
        vf->pcm_offset=ov_pcm_total(vf,-1); /* eof */
  }
  return 0;
}

/* seek to a playback time relative to the decompressed pcm stream
   returns zero on success, nonzero on failure */
int ov_time_seek(OggVorbis_File *vf,double seconds){
  /* translate time to PCM position and call ov_pcm_seek */

  int link=-1;
  ogg_int64_t pcm_total=ov_pcm_total(vf,-1);
  double time_total=ov_time_total(vf,-1);

  if(vf->ready_state<OPENED)return(-131);
  if(!vf->seekable)return(-138);
  if(seconds<0 || seconds>time_total)return(-131);

  /* which bitstream section does this time offset occur in? */
  for(link=vf->links-1;link>=0;link--){
    pcm_total-=vf->pcmlengths[link];
    time_total-=ov_time_total(vf,link);
    if(seconds>=time_total)break;
  }

  /* enough information to convert time offset to pcm offset */
  {
    ogg_int64_t target=pcm_total+(seconds-time_total)*vf->vi[link].rate;
    return(ov_pcm_seek(vf,target));
  }
}

/* page-granularity version of ov_time_seek
   returns zero on success, nonzero on failure */
int ov_time_seek_page(OggVorbis_File *vf,double seconds){
  /* translate time to PCM position and call ov_pcm_seek */

  int link=-1;
  ogg_int64_t pcm_total=ov_pcm_total(vf,-1);
  double time_total=ov_time_total(vf,-1);

  if(vf->ready_state<OPENED)return(-131);
  if(!vf->seekable)return(-138);
  if(seconds<0 || seconds>time_total)return(-131);

  /* which bitstream section does this time offset occur in? */
  for(link=vf->links-1;link>=0;link--){
    pcm_total-=vf->pcmlengths[link];
    time_total-=ov_time_total(vf,link);
    if(seconds>=time_total)break;
  }

  /* enough information to convert time offset to pcm offset */
  {
    ogg_int64_t target=pcm_total+(seconds-time_total)*vf->vi[link].rate;
    return(ov_pcm_seek_page(vf,target));
  }
}

/* tell the current stream offset cursor.  Note that seek followed by
   tell will likely not give the set offset due to caching */
ogg_int64_t ov_raw_tell(OggVorbis_File *vf){
  if(vf->ready_state<OPENED)return(-131);
  return(vf->offset);
}

/* return PCM offset (sample) of next PCM sample to be read */
ogg_int64_t ov_pcm_tell(OggVorbis_File *vf){
  if(vf->ready_state<OPENED)return(-131);
  return(vf->pcm_offset);
}

/* return time offset (seconds) of next PCM sample to be read */
double ov_time_tell(OggVorbis_File *vf){
  /* translate time to PCM position and call ov_pcm_seek */

  int link=-1;
  ogg_int64_t pcm_total=0;
  double time_total=0.f;

  if(vf->ready_state<OPENED)return(-131);
  if(vf->seekable){
    pcm_total=ov_pcm_total(vf,-1);
    time_total=ov_time_total(vf,-1);

    /* which bitstream section does this time offset occur in? */
    for(link=vf->links-1;link>=0;link--){
      pcm_total-=vf->pcmlengths[link];
      time_total-=ov_time_total(vf,link);
      if(vf->pcm_offset>=pcm_total)break;
    }
  }

  return((double)time_total+(double)(vf->pcm_offset-pcm_total)/vf->vi[link].rate);
}

/*  link:   -1) return the vorbis_info struct for the bitstream section
                currently being decoded
           0-n) to request information for a specific bitstream section

    In the case of a non-seekable bitstream, any call returns the
    current bitstream.  NULL in the case that the machine is not
    initialized */

vorbis_info *ov_info(OggVorbis_File *vf,int link){
  if(vf->seekable){
    if(link<0)
      if(vf->ready_state>=STREAMSET)
        return vf->vi+vf->current_link;
      else
      return vf->vi;
    else
      if(link>=vf->links)
        return NULL;
      else
        return vf->vi+link;
  }else{
    return vf->vi;
  }
}


int host_is_big_endian() {
  unsigned short pattern = 0xbeef;
  unsigned char *bytewise = (unsigned char *)&pattern;
  if (bytewise[0] == 0xbe) return 1;
  return 0;
}

/* up to this point, everything could more or less hide the multiple
   logical bitstream nature of chaining from the toplevel application
   if the toplevel application didn't particularly care.  However, at
   the point that we actually read audio back, the multiple-section
   nature must surface: Multiple bitstream sections do not necessarily
   have to have the same number of channels or sampling rate.

   ov_read returns the sequential logical bitstream number currently
   being decoded along with the PCM data in order that the toplevel
   application can take action on channel/sample rate changes.  This
   number will be incremented even for streamed (non-seekable) streams
   (for seekable streams, it represents the actual logical bitstream
   index within the physical bitstream.  Note that the accessor
   functions above are aware of this dichotomy).

   input values: buffer) a buffer to hold packed PCM data for return
                 length) the byte length requested to be placed into buffer
                 bigendianp) should the data be packed LSB first (0) or
                             MSB first (1)
                 word) word size for output.  currently 1 (byte) or
                       2 (16 bit short)

   return values: <0) error/hole in data (-3), partial open (-131)
                   0) EOF
                   n) number of bytes of PCM actually returned.  The
                   below works on a packet-by-packet basis, so the
                   return length is not related to the 'length' passed
                   in, just guaranteed to fit.

            *section) set to the logical bitstream number */


long ov_read(OggVorbis_File *vf,char *buffer,int length,
                    int bigendianp,int word,int sgned,int *bitstream){
  int i,j;
  int host_endian = host_is_big_endian();

  if(vf->ready_state<OPENED)return(-131);
  if(vf->ready_state==OPENED)return(-2); /* stream is always
                                                initialized after
                                                other calls (after
                                                open)... unless there
                                                was no page at the end
                                                to initialize state
                                                with. */

  while(1){
    if(vf->ready_state>=STREAMSET){
      float **pcm;
      long samples=vorbis_synthesis_pcmout(&vf->vd,&pcm);
      if(samples){
        /* yay! proceed to pack data into the byte buffer */

        long channels=ov_info(vf,-1)->channels;
        long bytespersample=word * channels;
        if(samples>length/bytespersample)samples=length/bytespersample;

        /* a tight loop to pack each size */
        {
          int val;
          if(word==1){
            int off=(sgned?0:128);
            for(j=0;j<samples;j++)
              for(i=0;i<channels;i++){
                val=vorbis_ftoi(pcm[i][j]*128.f);
                if(val>127)val=127;
                else if(val<-128)val=-128;
                *buffer++=val+off;
              }
          }else{
            int off=(sgned?0:32768);

            if(host_endian==bigendianp){
              if(sgned){

                for(i=0;i<channels;i++) { /* It's faster in this order */
                  float *src=pcm[i];
                  short *dest=((short *)buffer)+i;
                  for(j=0;j<samples;j++) {
                    val=vorbis_ftoi(src[j]*32768.f);
                    if(val>32767)val=32767;
                    else if(val<-32768)val=-32768;
                    *dest=val;
                    dest+=channels;
                  }
                }

              }else{

                for(i=0;i<channels;i++) {
                  float *src=pcm[i];
                  short *dest=((short *)buffer)+i;
                  for(j=0;j<samples;j++) {
                    val=vorbis_ftoi(src[j]*32768.f);
                    if(val>32767)val=32767;
                    else if(val<-32768)val=-32768;
                    *dest=val+off;
                    dest+=channels;
                  }
                }

              }
            }else if(bigendianp){

              for(j=0;j<samples;j++)
                for(i=0;i<channels;i++){
                  val=vorbis_ftoi(pcm[i][j]*32768.f);
                  if(val>32767)val=32767;
                  else if(val<-32768)val=-32768;
                  val+=off;
                  *buffer++=(val>>8);
                  *buffer++=(val&0xff);
                }

            }else{
              int val;
              for(j=0;j<samples;j++)
                 for(i=0;i<channels;i++){
                  val=vorbis_ftoi(pcm[i][j]*32768.f);
                  if(val>32767)val=32767;
                  else if(val<-32768)val=-32768;
                  val+=off;
                  *buffer++=(val&0xff);
                  *buffer++=(val>>8);
                  }

            }
          }
        }

        vorbis_synthesis_read(&vf->vd,samples);
        vf->pcm_offset+=samples;
        if(bitstream)*bitstream=vf->current_link;
        return(samples*bytespersample);
      }
    }

    /* suck in another packet */
    switch(_process_packet(vf,1)){
    case 0:case -2:
      return(0);
    case -3:
      return(-3);
    case -137:
      return(-137);
    }
  }
}





/*
char pcmout[4096];
int main(int argc, char **argv){
  OggVorbis_File vf;
  int eof=0;
  int current_section;
  int tmp;
  #ifdef _WIN32
    _setmode( _fileno( stdin ), _O_BINARY );
    _setmode( _fileno( stdout ), _O_BINARY );
  #endif
  if ((tmp=ov_open(stdin, &vf, NULL, 0)) < 0) {
      fprintf(stderr,"Input does not appear to be an Ogg bitstream. %i\n", tmp);
      exit(1);
  }
  {
    vorbis_info *vi=ov_info(&vf,-1);
    fprintf(stderr,"\nBitstream is %d channel, %ldHz\n",vi->channels,vi->rate);
  }

  while (!eof) {
    long ret=ov_read(&vf,pcmout,sizeof(pcmout),0,2,1,&current_section);
    if (ret == 0) {
       //EOF
      eof=1;
    } else if (ret < 0) {
       //error in the stream.  Not a problem, just reporting it in
         //case we (the app) cares.  In this case, we don't.
    } else {
       //we don't bother dealing with sample rate changes, etc, but
         //you'll have to
      fwrite(pcmout,1,ret,stdout);
    }
  }
  ov_clear(&vf);

  fprintf(stderr,"Done.\n");
  return(0);
}
*/
