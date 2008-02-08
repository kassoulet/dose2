#include <errno.h>
#ifdef _WIN32
  #ifndef __GNUC__
    //MSVC/Borland
    typedef __int64 ogg_int64_t;
    typedef unsigned __int32 ogg_uint32_t;
    typedef __int16 ogg_int16_t;
    #include <malloc.h>
    #include <io.h>
    #include <fcntl.h>
  #else
    //Cygwin
    #include <_G_config.h>
    typedef _G_int64_t ogg_int64_t;
    typedef _G_uint32_t ogg_uint32_t;
    typedef _G_int16_t ogg_int16_t;
  #endif
#else
  #ifdef __WATCOMC__
    typedef __int64 ogg_int64_t;
    typedef unsigned int ogg_uint32_t;
    typedef short ogg_int16_t;
    #include <malloc.h>
    #include <io.h>
    #include <fcntl.h>
  #else
    #include <sys/types.h>
    typedef int64_t ogg_int64_t;
    typedef int ogg_uint32_t;
    typedef short ogg_int16_t;
  #endif
#endif

typedef struct {
  unsigned char *data;
  int storage;
  int fill;
  int returned;
  int unsynced;
  int headerbytes;
  int bodybytes;
} ogg_sync_state;

typedef struct {
  unsigned char *body_data;
  long    body_storage;
  long    body_fill;
  long    body_returned;
  int     *lacing_vals;
  ogg_int64_t *granule_vals;
  long    lacing_storage;
  long    lacing_fill;
  long    lacing_packet;
  long    lacing_returned;
  int     e_o_s;
  int     b_o_s;
  long    serialno;
  long    pageno;
  ogg_int64_t  packetno;
  ogg_int64_t   granulepos;
} ogg_stream_state;

typedef struct vorbis_info {
  int version;
  int channels;
  long rate;
  long bitrate_upper;
  long bitrate_nominal;
  long bitrate_lower;
  void *codec_setup;
} vorbis_info;

typedef struct {
  long endbyte;
  int  endbit;
  unsigned char *buffer;
  unsigned char *ptr;
  long storage;
} oggpack_buffer;

typedef struct {
  size_t (*read_func)  (void *ptr, size_t size, size_t nmemb, void *datasource);
  int    (*seek_func)  (void *datasource, ogg_int64_t offset, int whence);
  int    (*close_func) (void *datasource);
  long   (*tell_func)  (void *datasource);
} ov_callbacks;

typedef struct vorbis_dsp_state {
  vorbis_info *vi;
  float **pcm;
  float **pcmret;
  int  pcm_storage;
  int  pcm_current;
  int  pcm_returned;
  int  eofflag;
  long lW;
  long W;
  long nW;
  long centerW;
  ogg_int64_t granulepos;
  ogg_int64_t sequence;
  ogg_int64_t glue_bits;
  ogg_int64_t floor_bits;
  ogg_int64_t res_bits;
  void       *backend_state;
} vorbis_dsp_state;

typedef struct vorbis_block {
  float  **pcm;
  oggpack_buffer opb;
  long  lW;
  long  W;
  long  nW;
  int   pcmend;
  int   mode;
  int   eofflag;
  ogg_int64_t granulepos;
  ogg_int64_t sequence;
  vorbis_dsp_state *vd;
  void  *localstore;
  long  localtop;
  long  localalloc;
  long  totaluse;
  struct alloc_chain *reap;
  long glue_bits;
  long floor_bits;
  long res_bits;
  void *internal;
} vorbis_block;

typedef struct OggVorbis_File {
  void            *datasource; /* Pointer to a FILE *, etc. */
  int              seekable;
  ogg_int64_t      offset;
  ogg_int64_t      end;
  ogg_sync_state   oy;

  /* If the FILE handle isn't seekable (eg, a pipe), only the current
     stream appears */
  int              links;
  ogg_int64_t     *offsets;
  ogg_int64_t     *dataoffsets;
  long            *serialnos;
  ogg_int64_t     *pcmlengths;
  vorbis_info     *vi;

  /* Decoding working state local storage */
  ogg_int64_t      pcm_offset;
  int              ready_state;
  long             current_serialno;
  int              current_link;

  double           bittrack;
  double           samptrack;

  ogg_stream_state os; /* take physical pages, weld into a logical
                          stream of packets */
  vorbis_dsp_state vd; /* central working state for the packet->PCM decoder */
  vorbis_block     vb; /* local working space for packet->PCM decode */

  ov_callbacks callbacks;

} OggVorbis_File;



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



