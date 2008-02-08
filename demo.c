#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "argb.h"
#include "m2d.h"
#include "layer.h"
#include "obu2d.h"
#include "demo.h"
#include "line.h"
#include "blob.h"

M2d m2d_ident={1, 0, 0, 0, 1, 0};
M2d m2d_xflip={-1, 0, 0, 0, 1, 0};
Obu2d *son, *luigi, *waitress, *bitch, *tupiss, *mfx, *hotel, *onepiss, *fok, *knife, *spoon, *sheets, *malta;
Obu2d *u2, *u3, *u4, *u5,  *u6, *u7, *u8, *u9, *u10, *u11;
Obu2d *phone, *manager, *resto, *bed, *dstairs, *plate, *toilet, *table;
Obu2d *d2, *dose, *italy, *man1, *man2;

Argb *nwhite, *nblack, *oil, *black, *hbblack, *blackish, *red, *green, *blue, *yellow, *cyan, *magenta, *grey, *white, *orange, *glass, * odula, *invert;

extern int stopnow;
float fpow2(float x, float p) { return x*pow(fabs(x), p-1); }
float fsin2(float x) { return sin(fmod(x, 1)*2*pi); }
float pii=3.14159265358979;

Obu2d *anim1[512];

void initdemo() {
  int i;
  //black   =new_col(1, 0, 0, 0);
  black   =new_col(1.3, .0, .0, .0);
  nblack   =new_col(1.0, .0, .0, .0);
  oil     =new_col(1.3, .0, .0, .0);

  invert   =new_col(1.8, 1, 1, 1);

  hbblack =new_col(0.75, 0.8, 0.4, 1);
  blackish=new_col(1, 0, 0, 0.1);
  red     =new_col(1, 1, 0, 0);
  green   =new_col(1, 0, 1, 0);
  blue    =new_col(1, 0, 0, 1);
  yellow  =new_col(1, 1, 1, 0);
  cyan    =new_col(1, 0, 1, 1);
  magenta =new_col(1, 1, 0, 1);
  grey    =new_col(1, 0.5, 0.5, 0.5);

  white   =new_col(0.25, 0.25, 0.25, 0.25);
  nwhite   =new_col(0.5, 0.5, 0.5, 0.5);

  orange  =new_col(1, 1, 1.0, 0);
  glass   =new_col(0, 0, 0, 0);

  son     =obu2d_lue2("data/son.vec");
  luigi   =obu2d_lue2("data/luigi.vec");
  waitress=obu2d_lue2("data/weitross.vec");
  bitch   =obu2d_lue2("data/biaaccth.vec");
  onepiss =obu2d_lue2("data/onepiss.vec");
  tupiss  =obu2d_lue2("data/tupiss.vec");
  mfx     =obu2d_lue2("data/mfxlogo.vec");
//  mfx     =obu2d_lue2("t13.vec");
  hotel   =obu2d_lue2("data/hotel.vec");
  fok    =obu2d_lue2("data/fok.vec");
  knife   =obu2d_lue2("data/knaiv.vec");
  spoon   =obu2d_lue2("data/spuun.vec");
//  table   =obu2d_lue2("teibl.vec");
  sheets  =obu2d_lue2("data/shits.vec");
  malta   =obu2d_lue2("data/malta.vec");
  u2      =obu2d_lue2("data/u2.vec");
  u3      =obu2d_lue2("data/u3.vec");
  u4      =obu2d_lue2("data/u4.vec");
  u5      =obu2d_lue2("data/u5.vec");
  u6      =obu2d_lue2("data/u6.vec");
  u7      =obu2d_lue2("data/u7.vec");
  u8      =obu2d_lue2("data/u8.vec");
  u9      =obu2d_lue2("data/u9.vec");
  u10     =obu2d_lue2("data/u10.vec");
  u11     =obu2d_lue2("data/u11.vec");
  phone   =obu2d_lue2("data/phoyn.vec");
  manager =obu2d_lue2("data/manitsor.vec");
  resto   =obu2d_lue2("data/resto.vec");
  bed     =obu2d_lue2("data/bed.vec");
  dstairs =obu2d_lue2("data/dstairs.vec");
  plate   =obu2d_lue2("data/pleit.vec");
  toilet  =obu2d_lue2("data/toilet.vec");
  table   =obu2d_lue2("data/teibl2.vec");
  d2      =obu2d_lue2("data/2.vec");
  dose    =obu2d_lue2("data/dose.vec");
  italy   =obu2d_lue2("data/italy.vec");
  man1    =obu2d_lue2("data/man1.vec");
  man2    =obu2d_lue2("data/man2.vec");
  for (i=0; i<277; i++) {
    char s[399];
    sprintf(s, "data/j%04i.vec", (i&~1)+2);
    anim1[i]=obu2d_lue2(s);
  }


}



float scale(float x, float a, float b, float na, float nb) {
  return (x-a)*(nb-na)/(b-a)+na;
}

void juttu(Obu2d *o, float t) {
  Palette *cols2=new_pal2(glass, new_col(0, -0.2, 0.4, 0.2));
  if (t<0 || t>1) return;
  pal_alpha2(cols2, 1-t);
  obu2d_draw(o, new_layer(cols2), 0, new_m2_roto(0, 400000, 14.0, 0), &m2d_ident);
}

extern float con, bri;

void strobo(float t, float start, float br, float cr) {
  if (t>=start*32&&t<(start+0.25)*32) {
    float x=0.1+(t-start*32)/8*0.9;
    bri=0+(1/x-1)*br;
    con=1+(1/x-1)*cr;
  }
}

void strobocont(float t, float start, float stop, float br, float cr, float length) {
  if (t>=start*32&&t<stop*32) {
    float strobe=fmod(t, length);
    float x=0.1+strobe/length*0.9;
    bri=0+(1/x-1)*br;
    con=1+(1/x-1)*cr;
  }
}
void strobocont2(float t, float start, float stop, float br, float cr, float length) {
  if (t>=start*32&&t<stop*32) {
    float strobe=fmod(t, length);
    float x=strobe/length*2;
    if (x<.1) x=.1; if (x>1) x=1;
//    bri=0+(1/x-1)*br;
    con=0+(1/x-1)*cr;
  }
}


int btw(float t, float a, float b) { return t>=a && t<b; }
void rundemo(float t) {
  int i;
  static float ot;
  float tt=(t+fsin2(t*.25)*.0)*0.15;
  float open=0;
  Palette *cols1=new_pals3(glass, 4,
    black, white
//    new_col(0, 0.4+0.4*sin(t*pii), 0.2+0.2*sin(t*pii), -0.2-0.2*sin(t*pii))
  );
//  Palette *cols_ani =new_pals2(5, nwhite, black);

  Palette *cols_ani =new_pals3(white,4,black,glass);
//  Palette *cols_ani =new_pal8(white,glass,nblack,black,nblack,black,black,black);

  Palette *cols2=new_pal2(
    black,
    new_col(0, 1.0+1.0*sin(t*pii)*0.5, 0.5+0.5*sin(t*pii)*0.5, 0.0)
  );
  Palette *cols3=new_pal2(
    black,
    new_col(0, 1.0+1.0*sin(t*pii)*0.5, 0.8+0.8*sin(t*pii)*0.5, 0.0)
  );
  Palette *cols4b=new_pal2(black, glass);
  Palette *cols4=new_pal2(glass, black);

  Palette *cols4bn=new_pal2(invert, glass);
  Palette *cols4n=new_pal2(glass, invert);
  Palette *acols4bn=new_pals2(5,invert, glass);
  Palette *acols4n=new_pals2(5,glass, invert);

  Palette *cols10=new_pals2(5,glass, red);

/*
  Palette *cols4b=new_pal2(
    new_col(-1+3*sin(t*pii*0.5), 0, 0.2, 0),
    new_col(0, 0, 0, 0)
 );
  Palette *cols4=new_pal2(
    new_col(0, 0, 0, 0),
    new_col(-1+3*sin(t*pii*0.25), 0, 0.2, 0)
 );
*/


  //Palette *cols8=new_pals2(8, glass, green);
  Palette *cols8=new_pals3(glass, 7, new_col(0, 0, 0, 0.3), green);

  Palette *cols5=new_pal2(new_col(.7, .7, .7, 0), new_col(0.5, .6, .0, 0));
  Palette *cols5_1=new_pal2(new_col(0.5, .6, .0, 0),new_col(.7, .7, .7, 0));
  Palette *cols5_2=new_pal2(new_col(0.5, .6, .0, 0),new_col(.7, .7, .7, 0));

  Palette *cols5b=new_pal2(black, white);
  Palette *cols5b_1=new_pal2(white,black);
  Palette *cols5b_2=new_pal2(white,black);

//  Palette *cols10=new_pals2(glass, red);


  //pal_alpha2(cols5, 0.5);
  pal_alpha2(cols5b, 0.5);
  pal_alpha2(cols5b_1, 0.5);
  pal_alpha2(cols5b_2, 0.5);

  pal_alpha(cols2, 0);
//  pal_alpha(cols_ani , 0.9125);

  //t*=1/16.0;
//  pal_rotate(cols2,  t*0.1, 5);
//  pal_rotate(cols3,  t*0.1+0.7, 5);
/*
  pal_rotate(cols5, 0.1, 6);
  pal_rotate(cols8, 0.2*sin(t*0.123), 8);
  pal_rotate(cols4, 0, 4+4*sin(t*pii*0.5));
  pal_rotate(cols4b, 0, 4+4*sin(t*pii*0.5));
*/




  if (btw(t,12*32,24*32)) {
    float syn=fmod(t-12*32, 128);
    if (syn>=0&&syn<10 || syn>=0+32&&syn<10+32 || syn>=0+64&&syn<10+64 || syn>=0+96&&syn<10+96) {
      if (syn>=-2+96&&syn<1+96) {
        float t2=t*4;
//        int syn=fmod(t2, 277);
        pal_rotate(cols5, 0.5*fsin2(t*.25), .9);
      } else {
        if ((int)(t*1)%4==0 || (int)(t*1)%4==1) {
          float t2=t*4;
//          int syn=fmod(t2, 277);
          pal_rotate(cols5, 0.5*fsin2(t*.25), .9);
        }
      }
    }
    if (btw(t,20*32,24*32)) {
      pal_rotate(cols5, 0.5*fsin2(t*.125*0.25), .9);
    }
  } else if (btw(t,28*32,30*32)) {
    pal_rotate(cols5, 0.125+0.375*((t/32-28)*0.5)*fsin2(t*.125), .9);
  } else if (btw(t,30*32,31*32)) {
    pal_rotate(cols5, 0.5*fsin2(t*.5), .9);
    pal_reverse(cols5);
    pal_reverse(cols5b);
  } else if (btw(t,31*32,31.5*32)) {
    pal_rotate(cols5, 0.5*fsin2(t*1), .9);
    pal_reverse(cols5);
    pal_reverse(cols5b);
  } else if (btw(t,31.5*32,32*32)) {
    pal_rotate(cols5, 0.5*fsin2(t*2), .9);
  } else if (btw(t,32*32,36*32)) {
    pal_rotate(cols5, 0.5*fsin2(t*.25), .9);
  } else if (btw(t,36*32,40*32)) {
    pal_rotate(cols5, (0.0+0.5*( (t/32-36)*0.25*(t/32-36)*0.25) )*fsin2(t*.25), .9);
  } else if (btw(t,40*32,44*32)) {
    pal_rotate(cols5, 0.5*fsin2(t*.25), .9);
  } else if (btw(t,52*32,56*32)) {
    pal_rotate(cols5, 0.125*fsin2(t*.125), .9);
  } else if (btw(t,56*32,58*32)) {
    pal_rotate(cols5, 0.125+0.375*((t/32-56)*0.5)*fsin2(t*.125), .9);
  } else if (btw(t,58*32,60*32)) {
    pal_rotate(cols5, 0.5*fsin2(t*.5), .9);
  }



  pal_rotate(cols10, 0+0.05*sin(t), 1.25+0.5*sin(t*pii*0.5) );





/*  obu2d_draw(new_obu2d_morph(mfx, dose, .5), new_layer(cols5), 0, 
    new_m2_roto(0, 0, 14.0+((t/32-16.00)*0.3), -(1-(t/32-16))*0.2), &m2d_ident);*/



  /*Alla hyv„ diskospiraali „l„ sie l„„pi sit„*/



         /*
               blob_lisaatuneli(b, 0.05, tt*3.0,
                 v_create(new_v(), sin(tt*1.3)*0.4, sin(tt*1.5)*0.3, 0.4));
               blob_draw(b, new_layer(cols2));
         */
               //blob_lisaa2xtuneli(b, 0.10, tt*3.0,
                 //v_create(new_v(), sin(tt*1.3)*5*(0.4+0.1*sin(tt*0.1)), sin(tt*1.5)*(0.3+0.1*sin(tt*0.1))*5, 0.4),
                 //v_create(new_v(), -sin(tt*1.3)*5*(0.4+0.1*sin(tt*0.1)), -sin(tt*1.5)*(0.3+0.1*sin(tt*0.1))*5, 0.4)
                 //);
               //blob_draw(b, new_layer(cols3));

               //jarnon h„r”spiraali alla...


  if (btw(t, 8*32, 16*32) || btw(t, 44*32, 52*32) ) {
    if (btw(t, 44*32, 52*32) ) {
      if ((int)(t*1)%4==2 || (int)(t*1)%4==3) {
        Blob *b=new_blob(0.00324234234234);
        Blob *b2=new_blob(0.00324234234234);
        blob_lisaatuneli(b, 0.10, (t*2)*2.3-fsin2(t*2*.25)*.9,
          v_create(new_v(), sin(tt*1.3)*(0.04+0.01*sin(tt*2*0.1)), sin(tt*1.5)*(0.03+0.01*sin(tt*2*0.1)), 0.6));
        blob_draw(b, new_layer(cols5));
        blob_lisaatuneli(b, 0.10, (t*2+.5)*2.3-fsin2((t*2+.5)*.25)*.9,
          v_create(new_v(), sin(tt*2*1.3)*(0.04+0.01*sin(tt*2*0.1)), sin(tt*1.5)*(0.03+0.01*sin(tt*2*0.1)), 0.6));
        blob_draw(b, new_layer(cols5b));
      } else {
        Blob *b=new_blob(0.00324234234234);
        blob_lisaa2xtuneli(b, 0.15, tt*28,
          v_create(new_v(), (fsin2(t*.125*0.5)*0.45+.05)     , (fsin2(t*.125*0.5+.3)*0.35-.04)     , 0.35),
          v_create(new_v(), (fsin2(t*.125*0.5)*0.45+.05)*-1.1, (fsin2(t*.125*0.5+.3)*0.35-.04)*-1.1, 0.4)
         );
        blob_draw(b, new_layer(cols5b));
      }
    } else {
      Blob *b=new_blob(0.00324234234234);
      blob_lisaa2xtuneli(b, 0.15, tt*28,
        v_create(new_v(), (fsin2(t*.125*0.5)*0.45+.05)     , (fsin2(t*.125*0.5+.3)*0.35-.04)     , 0.35),
        v_create(new_v(), (fsin2(t*.125*0.5)*0.45+.05)*-1.1, (fsin2(t*.125*0.5+.3)*0.35-.04)*-1.1, 0.4)
       );
      blob_draw(b, new_layer(cols5));
    }
  } else if ( btw(t, 36*32, 40*32) || btw(t, 52*32, 56*32) ) {
    Blob *b=new_blob(0.00324234234234);
    Blob *b2=new_blob(0.00324234234234);
    blob_lisaatuneli(b, 0.10, (t)*2.3-fsin2(t*.25)*.9,
      v_create(new_v(), sin(tt*1.3)*(0.04+0.01*sin(tt*0.1)), sin(tt*1.5)*(0.03+0.01*sin(tt*0.1)), 0.6));
    blob_draw(b, new_layer(cols5));
    blob_lisaatuneli(b, 0.10, (t+.5)*2.3-fsin2((t+.5)*.25)*.9,
      v_create(new_v(), sin(tt*1.3)*(0.04+0.01*sin(tt*0.1)), sin(tt*1.5)*(0.03+0.01*sin(tt*0.1)), 0.6));
    blob_draw(b, new_layer(cols5));

  } else {
    Blob *b=new_blob(0.00324234234234);
        //Vanha "marimekko" -spiraali. ei saa koskea!
    blob_lisaatuneli(b, 0.10, tt*3.0,
      v_create(new_v(), sin(tt*1.3)*(0.4+0.1*sin(tt*0.1)), sin(tt*1.5)*(0.3+0.1*sin(tt*0.1)), 0.6));
    blob_draw(b, new_layer(cols5));
    blob_lisaatuneli(b, 0.20, tt*3.0,
      v_create(new_v(), sin(tt*1.3)*(0.3+0.2*sin(tt*0.1)), sin(tt*1.5)*(0.2+0.2*sin(tt*0.1)), 0.6));
    blob_draw(b, new_layer(cols5));
  }






  //ALLA OLEVA ON KUNNOSSA!!!!!!!!!!!
  //logo(t)
  if (btw(t, 4.00*32, 20.05*32)) {
    //if ((int)(t*1)%4==0 || (int)(t*1)%4==1) {

    if (btw(t, 7.5*32, 8*32)) {
      obu2d_draw(new_obu2d_melt(mfx, 0.65+0.5*sin((t+2-7.5*32)*.5*pii)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0, 0), &m2d_ident);
    } else if (btw(t, 0*32, 8*32 )) {
      if ((int)(t*1)%4==2 || (int)(t*1)%4==3) {
        float x=t*0.1;
        obu2d_draw(new_obu2d_melt(mfx, 0.65+0.5*sin((t+2-7.5*32)*.5*pii)), new_layer(cols1), 0,
          new_m2_roto(0, 0, 14.0, 0), &m2d_ident);
      }
    } else if (btw(t, 8*32, 12*32)) {
      if ((int)(t*0.5)%4==0 || (int)(t*0.5)%4==1) {
        float x=t*0.1;
        obu2d_draw(new_obu2d_melt(dose, 0.65+0.5*sin((t+2-7.5*32)*.5*pii)), new_layer(cols1), 0,
          new_m2_roto(0, 0, 14.0, 0), &m2d_ident);
      } else {
        obu2d_draw(new_obu2d_melt(d2, 0.65+0.5*sin((t+2-7.5*32)*.5*pii)), new_layer(cols1), 0,
          new_m2_roto(0, 0, 14.0, 0), &m2d_ident);
      }
    } else if (btw(t, 12*32, 16*32)) {
      if ((int)(t*1)%4==2 || (int)(t*1)%4==3) {
        if ((int)(t*0.5)%4==0 || (int)(t*0.5)%4==1) {
          float x=t*0.1;
          obu2d_draw(new_obu2d_melt(dose, 0.65+0.5*sin((t+2-7.5*32)*.5*pii)), new_layer(cols1), 0,
            new_m2_roto(0, 0, 14.0, 0), &m2d_ident);
        } else {
          obu2d_draw(new_obu2d_melt(d2, 0.65+0.5*sin((t+2-7.5*32)*.5*pii)), new_layer(cols1), 0,
            new_m2_roto(0, 0, 14.0, 0), &m2d_ident);
        }
      }
    } else {
      if ((int)(t*1)%4==2 || (int)(t*1)%4==3) {
        if ((int)(t*0.5)%4==0 || (int)(t*0.5)%4==1) {
          float x=t*0.1;
          obu2d_draw(new_obu2d_melt(dose, 0.65+0.5*sin((t+2-7.5*32)*.5*pii)), new_layer(cols1), 0,
            new_m2_roto(0, 0, 14.0+((t/32-16.00)*0.3), -(1-(t/32-16))*0.2), &m2d_ident);
        } else {
          obu2d_draw(new_obu2d_melt(d2, 0.65+0.5*sin((t+2-7.5*32)*.5*pii)), new_layer(cols1), 0,
            new_m2_roto(0, 0, 14.0+((t/32-16.00)*0.3), +(1-(t/32-16))*0.2), &m2d_ident);
        }
      }
    }
  }














  /*Strobot, ok*/
  strobo(t, 0.00, 0.1, 1);
  //strobo(t, 1.00, 0.1, 1);
  //strobo(t, 2.00, 0.1, 1);
  //strobo(t, 3.00, 0.1, 1);
  strobo(t, 4.00, 0.25, 2);
  strobo(t, 8.00, 0.1, 1);
  strobo(t, 12.00, 0.1, 1);
  strobo(t, 16.00, 0.1, 1);
  strobo(t, 20.00, 0.25, 2);
  strobo(t, 24.00, 0.1, 1);
  strobo(t, 28.00, 0.1, 1);
  strobo(t, 30.00, 0.1, 1);
  strobo(t, 31.00, 0.1, 1);
  strobo(t, 31.50, 0.1, 1);
  strobo(t, 31.625, 0.1, 1);
  strobo(t, 31.75, 0.1, 1);
  strobo(t, 32.00, 0.25, 2);
  strobo(t, 36.00, 0.25, 2);
  strobo(t, 40.00, 0.1, 1);
  strobo(t, 44.00, 0.1, 1);
  strobo(t, 48.00, 0.1, 1);
  strobo(t, 52.00, 0.25, 2);
  strobo(t, 54.00, 0.1, 1);
  strobo(t, 56.00, 0.1, 1);
  strobo(t, 58.00, 0.1, 1);
  strobo(t, 60.00, 0.1, 1);
  strobocont(t, 4, 7.5, 0.1, 1, 4);
  strobocont(t, 8, 11.87, 0.1, 1, 4);
  strobocont(t, 12, 20, 0.1, 1, 4);
  strobocont(t, 31.875, 31.875+0.0625, 1.5, 10, 0.5);//nopea snarenosto
  strobocont(t, 32, 36, 0.1, 1, 2);
  strobocont(t, 40, 43.8749, 0.025, 1, 4);
  strobocont(t, 43.875, 43.875+0.0625, 1.5, 10, 0.5);//nopea snarenosto

  strobocont2(t, 44, 51.8749, 0.0, 0.5, 1);
  //con-=1;

  strobocont(t, 51.875, 51.875+0.0625, 1.5, 10, 0.5);//nopea snarenosto



  //ALLA OLEVA ON KUNNOSSA!!!!!!!!!!!
  //t„h„n on tarkoitus pist„„ jorailup„tki„
  if (btw(t, 12.0*32, 24.0*32)) {

    //float syn=fmod(t-11.5*32, 128);
    float syn=fmod(t-12*32, 128);

    //if (syn>=14&&syn<24 || syn>=14+32&&syn<24+32 || syn>=14+64&&syn<24+64 || syn>=12+96&&syn<24+96) {
    if (syn>=0&&syn<10 || syn>=0+32&&syn<10+32 || syn>=0+64&&syn<10+64 || syn>=0+96&&syn<10+96) {
      if (syn>=-2+96&&syn<1+96) {
        float t2=t*4;
        int syn=fmod(t2, 277);
          /*
                      Obu2d *obuh;
                       obu2d_draw(anim1[syn], new_layer(acols4n), 0,
                         new_m2_roto(0, 0, 14.5, 0), &m2d_ident);
          */
        obu2d_draw(luigi, new_layer(cols1), 0,
          new_m2_roto(0, 0, 14.0, 0),
          &m2d_ident);
      } else {

        if ((int)(t*1)%4==0 || (int)(t*1)%4==1) {
          float t2=t*4;
          int syn=fmod(t2, 277);

          //Obu2d *obuh;
           //obu2d_draw(anim1[syn], new_layer(cols1), 0,
             //new_m2_roto(0, 0, 14.5, 0), &m2d_ident);

          obu2d_draw(luigi, new_layer(cols1), 0,
            new_m2_mul(new_m2_roto(0, 0, 13.4+0.2*((int)(t/4+0)%4), 0), (int)(t*0.25)%2==0?&m2d_xflip:&m2d_ident ), &m2d_ident);

        }
      }
    }
  }
                  //jorailup„tk„ part 2 & 3
            /*
                  if (t>=32.0*32 && t<36.0*32 || t>=44*32 && t<52*32) {

            //        float syn=fmod(t-11.5*32, 128);
                    float syn=fmod(t-12*32, 128);

            //        if (syn>=14&&syn<24 || syn>=14+32&&syn<24+32 || syn>=14+64&&syn<24+64 || syn>=12+96&&syn<24+96) {
            //        if (syn>=0&&syn<24 || syn>=0+32&&syn<24+32 || syn>=0+64&&syn<24+64 || syn>=0+96&&syn<24+96) {
            //            if ((int)(t*1)%4==2 || (int)(t*1)%4==3) {
                    if ((int)(t*2)%4<2 ) {//|| (int)(t*2)%4==1) {
                      //obu2d_draw(luigi, new_layer(cols1), 10,
                        //new_m2_roto(0, 0, 13.4+0.2*((int)(t/4+0)%4), 0), &m2d_ident);
                    }
            //        }
                  }
            */












  //Aminaatiotesti hu†noo goodii
  if (btw(t, 44*32, 52*32) || btw(t, 32*32, 36*32)) {
    float t2=t*4;
    int syn=fmod(t2, 267);

    Obu2d *obuh;
    if ((int)(t*0.25)%4==0 ) {
      obu2d_draw(anim1[syn], new_layer(cols_ani ), 0,
        new_m2_roto(0, 0, 14.5, 0), &m2d_ident);
    } else if ((int)(t*0.25)%4==1 ) {
      obu2d_draw(anim1[syn], new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.5, 0), &m2d_ident);
    } else if ((int)(t*0.25)%4==2 ) {
      obu2d_draw(anim1[syn], new_layer(acols4bn), 0,
        new_m2_roto(0, 0, 14.5, 0), &m2d_ident);
    } else if ((int)(t*0.25)%4==3 ) {
      obu2d_draw(anim1[syn], new_layer(acols4n), 0,
        new_m2_roto(0, 0, 14.5, 0), &m2d_ident);
    }
  }






  //diskospiraali loppu ny
  //N„ „ blobit tekee vitun  akeita roll-over effuja. „l„ sie n„it„k„„ l„„pi

  if (t<4.0*32 || t>=32*32 && t<36*32 /*|| t>=44*32 && t<52*32 || t>60*32*/) {
    Blob *b=new_blob(-1);
    Blob *c=new_blob(-1);
    Layer *l;

    if (btw(t, 3.5*32, 8*32)) {
      open=(t-3.5*32)/10;
    } else {
      open=0;
    }

    for (i=0; i<14; i++) {
      blob_addcomp(c, 20+cos(tt*(1-i*0.03)-i)*15, 15+sin(tt*(1-i*0.05)+i)*15, 1+open*4, 0.05);
      blob_addcomp(c, 20+sin(tt*(1+i*0.05)-i)*15, 15-cos(tt*(1-i*0.04)+i)*15, -3.3+13.3*sin(tt), 0.9);
    }
    if (btw(t, 32*32, 44*32)) {
      l=new_layer(cols4bn);
    } else {
      l=new_layer(cols4b);
    }
    blob_draw(c, l);
    for (i=0; i<13; i++) {
      blob_addcomp(b, 20+cos(tt*(1-i*0.03)-i)*15, 15+sin(tt*(1-i*0.05)+i)*15, 1-(open*open*open), 0.05);
      blob_addcomp(b, 20+sin(tt*(1+i*0.05)-i)*15, 15-cos(tt*(1-i*0.04)+i)*15, -3, 0.7);
    }
    //if (btw(t, 32*32, 44*32)) {
      //l=new_layer(cols4n);
    //} else {
      //l=new_layer(cols4);
    //}
    blob_draw(b, l);
  }

  //rollover blobit loppuu







  //kuvat spiikkeihin:

  // Eka spiikkiosuus. ravintolasta saa vain yhden leiv„n

  //EKA SPIIKKIOSUUS VALMIS. ŽLŽ KOSKE!!!!!!!!!!!!!!!!!!!!!!!

  if (btw(t, 24*32, 32*32)) {


    //one day i go to malta
    if (btw(t, 24.5*32, 25*32)) {
      obu2d_draw(new_obu2d_melt(malta, .5+.5*sin((t-26.1*32)*0.3)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0, 0), &m2d_ident);
    //to a big hotel
    } else if (btw(t, 25.0*32, 25.3*32)) {
      obu2d_draw(new_obu2d_melt(hotel, .75+.25*sin(t*2)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0, 0), &m2d_ident);

    //in the  orning i go eat breakfast
    } else if (btw(t, 25.5*32, 26.3*32)) {
      Layer *valih=new_layer(cols1);
      if (btw(t, 25.50*32, 26.05*32)) {
        obu2d_draw(luigi, valih, 0,
          new_m2_roto(+1200000-(t/32-25.60)*500000*2, 0, 13.65, 0), &m2d_ident);
      } else if (btw(t, 26.05*32, 26.3*32)) {
        obu2d_draw(luigi, valih, 0,
          new_m2_roto(+1200000-(26.05-25.60)*500000*2, 0, 13.65, 0), &m2d_ident);
      }
      obu2d_draw(table, valih, 0,
        new_m2_roto(-500000, 400000, 13.65, 0), &m2d_ident);

    //i tell the waitress
    } else if (btw(t, 26.3*32, 26.5*32)) {
      obu2d_draw(new_obu2d_melt(waitress, .75+.25*sin(t*2)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0, 0), &m2d_ident);
    // i want two pieces of toast
    } else if (btw(t, 26.5*32, 27.05*32)) {
      obu2d_draw(new_obu2d_melt(tupiss, .75+.25*sin(t*2)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0, 0), &m2d_ident);
    // but she brings  e only
    } else if (btw(t, 27.05*32, 27.25*32)) {
      obu2d_draw(new_obu2d_melt(waitress, .75+.25*sin(t*2)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0, 0), &m2d_ident);
    // one piece
    } else if (btw(t, 27.25*32, 27.675*32)) {
      obu2d_draw(new_obu2d_melt(onepiss, .8+.2*sin(t*1)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0, (t-(27.25*32))*0.02 ), &m2d_ident);
    // i tell her i want
    } else if (btw(t, 27.675*32, 28.00*32)) {
      Layer *valih=new_layer(cols1);
      obu2d_draw(new_obu2d_melt(luigi, .8+.2*sin(t*2)), valih, 0,
        new_m2_roto(750000, 0, 14.0, 0), &m2d_ident);
      //her...
      if (btw(t, 27.8*32, 28.00*32)) {
        obu2d_draw(new_obu2d_melt(waitress, .75+.25*sin(t*2)), valih, 0,
          new_m2_roto(-750000, 0, 14.0, 0), &m2d_ident);
      }
    //two pieces on the plate
    } else if (btw(t, 28.00*32, 28.45*32)) {
      obu2d_draw(new_obu2d_melt(tupiss, .6+.4*sin(t*2)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0, -(t-(28.0*32))*0.03), &m2d_ident);
    //she says go to the toilet....
    } else if (btw(t, 28.5*32, 28.65*32)) {
        obu2d_draw(new_obu2d_melt(waitress, .75+.25*sin(t*2)), new_layer(cols1), 0,
          new_m2_roto(-750000, 0, 14.0, 0), &m2d_ident);
    } else if (btw(t, 28.65*32, 28.95*32)) {
        obu2d_draw(new_obu2d_melt(toilet, .75+.25*sin(t*2)), new_layer(cols1), 0,
          new_m2_roto(0, 0, 14.0, 0), &m2d_ident);

    //ja jotain  uuta lyriikkaa
    //INSERT JOTAIN HERE

    //i say you don't understand
    } else if (btw(t, 28.95*32, 29.35*32)) {
      obu2d_draw(new_obu2d_melt(luigi, .8+.2*sin(t*2)), new_layer(cols1), 0,
        new_m2_roto(750000, 0, 14.0, 0), &m2d_ident);

    //i want two pieces on  y plate
    } else if (btw(t, 29.35*32, 29.9*32)) {
      obu2d_draw(new_obu2d_melt(tupiss, .6+.4*sin(t*2)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0+0.05*fsin2(t*0.5), 0), &m2d_ident);

    //she says you better
    } else if (btw(t, 29.9*32, 30.125*32)) {
      obu2d_draw(new_obu2d_melt(waitress, .7+.3*sin(t*2)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0, 0), &m2d_ident);
    //not piss on the plate
    } else if (btw(t, 30.125*32, 30.50*32)) {
      obu2d_draw(new_obu2d_melt(plate, .5+.5*sin(t*2)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0+0.075*fsin2(t*0.5), 0), &m2d_ident);

    //you son of a bitch
    } else if (btw(t, 30.50*32, 30.8*32)) {
      Obu2d *obu=new_obu2d_morph2(son, bitch, 0.5+0.5*fpow2(fsin2((t-1.0)*0.1), 0.2));
      obu2d_draw(obu, new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0, 0), &m2d_ident);
    } else if (btw(t, 31.05*32, 31.625*32)) {
      Layer *valih=new_layer(cols1);
      //i dont even know the lady and
      if (btw(t, 31.05*32, 31.35*32)) {
        obu2d_draw(new_obu2d_melt(luigi, .5+.5*sin((t-4)*0.5)), valih, 0,
          new_m2_roto(750000, 0, 14.0, 0), &m2d_ident);
      } else {
        obu2d_draw(new_obu2d_melt(luigi, .7+.25*sin((t)*2)), valih, 0,
          new_m2_roto(750000, 0, 14.0, 0), &m2d_ident);
      }
      //she calls  e
      if (btw(t, 31.30*32, 31.625*32)) {
        obu2d_draw(new_obu2d_melt(waitress, .6+.4*sin(t*2)), valih, 0,
          new_m2_roto(-750000, 0, 14.0, 0), &m2d_ident);
      }
    } else if (btw(t, 31.625*32, 31.75*32)) {
      //a son of a bitch
      Obu2d *obu=new_obu2d_morph2(son, bitch, 0.5+0.5*fpow2(fsin2((t+5)*0.1), 0.2));
      obu2d_draw(obu, new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0, 0), &m2d_ident);
    }

  }





  // Toka spiikkiosuus. ravintolasta saa vain veitsen ja lusikan

  //TOKA SPIIKKIOSUUS VALMIS. ŽLŽ KOSKE!!!!!!!!!!!!!!!!!!!!!!!

  if (btw(t, 36*32, 44*32)) {

    //alkuosaan fancy restaurant kuva...

    if (btw(t, 36.5*32, 37.0*32)) {
      obu2d_draw(new_obu2d_melt(resto, .0+1.0*sin((t-(36.5*32))*pii*.0435)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0+((t/32-37)*1.5), (1-(t/32-37))*0.45 ), &m2d_ident);

    } else if (btw(t, 37.0*32, 37.5*32)) {
      if (btw(t, 37.25*32, 37.5*32)) {
        pal_alpha2(cols1, (1-(t/32-37.25)*4) );
      }
      obu2d_draw(new_obu2d_melt(resto, .75+.25*sin(t*.5)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0+((t/32-37)*1.5), (1-(t/32-37))*0.45 ), &m2d_ident);

    //the waitress brings  e
    } else if (btw(t, 37.5*32, 37.9*32)) {
      obu2d_draw(new_obu2d_melt(waitress, .75+.25*sin(t*.5)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0, 0), &m2d_ident);
    } else if (btw(t, 37.9*32, 38.25*32)) {
      Layer *aterimet=new_layer(cols1);
      //a spoon and
      //strobo(t, 37.9, 0.1, 1);
      //strobo(t, 38.075, 0.1, 1);
      if (btw(t, 37.9*32, 38.25*32)) {
        obu2d_draw(new_obu2d_melt(spoon, .75+.25*sin(t*.5)), aterimet, 0,
          new_m2_roto(0, -500000, 13.8, -90), &m2d_ident);
      }
      //a knife but
      if (btw(t, 38.075*32, 38.25*32)) {
        obu2d_draw(new_obu2d_melt(knife, .75+.25*sin(t*.5)), aterimet, 0,
          new_m2_roto(1000000, 0, 13.8, 0), &m2d_ident);
      }
    } else if (btw(t,38.25*32, 38.5*32)) {
      //no fok!
      strobo(t, 38.25, 0.1, 1);
      //if (btw(t, 38.25*32, 38.5*32)) {
        obu2d_draw(new_obu2d_melt(fok, .75+.25*sin(t*pii*0.5)), new_layer(cols10), 0,
          new_m2_roto(0, 0, 14.0, 0), &m2d_ident);
      //}

    //i tell her i wanna
    } else if (btw(t, 38.75*32, 39.00*32)) {
      obu2d_draw(new_obu2d_melt(luigi, .75+.25*sin(t*.5)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0, 0), &m2d_ident);
    //fok
    } else if (btw(t, 39.00*32, 39.125*32)) {
      obu2d_draw(new_obu2d_melt(fok, .75+.25*sin(t*.5)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.05, 0), &m2d_ident);

    //she tells  e everyone wants to
    } else if (btw(t, 39.35*32, 39.75*32)) {
      obu2d_draw(new_obu2d_melt(waitress, .75+.25*sin(t*.5)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0, 0), &m2d_ident);

    //fok
    } else if (btw(t, 39.75*32, 39.90*32)) {
      obu2d_draw(new_obu2d_melt(fok, .75+.25*sin(t*.5)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.1, 0), &m2d_ident);

    //i say you don't understand i wanna
    } else if (btw(t, 40.00*32, 40.75*32)) {
      obu2d_draw(new_obu2d_melt(luigi, .75+.25*sin(t*.5)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0, 0), &m2d_ident);

    //fok
    } else if (btw(t, 40.75*32, 40.875*32)) {
      obu2d_draw(new_obu2d_melt(fok, .75+.25*sin(t*.5)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.15, 0), &m2d_ident);
    //on the table
    } else if (btw(t, 40.875*32, 41.1*32)) {
      obu2d_draw(new_obu2d_melt(table, .5+.5*sin(t*.5)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0, +(t-(28.0*32))*0.03), &m2d_ident);

    //she says
    } else if (btw(t, 41.2*32, 41.375*32)) {
      obu2d_draw(new_obu2d_melt(waitress, .75+.25*sin(t*.5)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0, 0), &m2d_ident);
    //you better not
    } else if (btw(t, 41.375*32, 41.5*32)) {
      obu2d_draw(new_obu2d_melt(luigi, .75+.25*sin(t*.5)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0, 0), &m2d_ident);
    // fok
    } else if (btw(t, 41.5*32, 41.625*32)) {
      obu2d_draw(new_obu2d_melt(fok, .75+.25*sin(t*.5)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.20, 0), &m2d_ident);

    //on the table
    } else if (btw(t, 41.625*32, 41.80*32)) {
      obu2d_draw(new_obu2d_melt(table, .5+.5*sin(t*.5)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0+((t/32-41.625)*0.8), -(t-(41.625*32))*0.01), &m2d_ident);

    //you son of a bitch
    } else if (btw(t, 41.7*32, 42.05*32)) {
      Obu2d *obu=new_obu2d_morph2(son, bitch, 0.5+0.5*fpow2(fsin2((t-1.0)*0.1), 0.2));
      obu2d_draw(obu, new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0, 0), &m2d_ident);

    //i dont even know the lady and
    } else if (btw(t, 42.675*32, 43.00*32)) {
      obu2d_draw(new_obu2d_melt(luigi, .75+.25*sin((t-4)*0.5)), new_layer(cols1), 0,
        new_m2_roto(750000, 0, 14.0, 0), &m2d_ident);

    //she calls  e
    } else if (btw(t, 43.00*32, 43.2*32)) {
      obu2d_draw(new_obu2d_melt(waitress, .6+.4*sin(t*2)), new_layer(cols1), 0,
        new_m2_roto(-750000, 0, 14.0, 0), &m2d_ident);

    //a son of a bitch
    } else if (btw(t, 43.25*32, 43.50*32)) {
      Obu2d *obu=new_obu2d_morph2(son, bitch, 0.5+0.5*fpow2(fsin2((t+2)*0.1), 0.2));
      obu2d_draw(obu, new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0, 0), &m2d_ident);
    }

  }






  // Kol as spiikkiosuus. ei lakanaa

  if (btw(t, 52*32, 60*32)) {

    //so i go back to  y roo

    if (btw(t, 52.5*32, 53.0*32)) {
      obu2d_draw(new_obu2d_melt(hotel, .0+1.0*sin((t-(52.5*32))*pii*.0435)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.2+((t/32-53.00)*0.8), -(1-(t/32-53.00))*0.45 ), &m2d_ident);

    } else if (btw(t, 53.00*32, 53.5*32)) {
      if (btw(t, 53.25*32, 53.5*32)) {
        pal_alpha2(cols1, (1-(t/32-53.25)*4) );
      }
      obu2d_draw(new_obu2d_melt(hotel, .75+.25*sin(t*.5)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.2+((t/32-53.00)*0.8), -(1-(t/32-53.00))*0.45 ), &m2d_ident);
    //in the hotel
               /*

                     } else if (btw(t, 53.25*32, 53.75*32)) {//VALMIS
                       obu2d_draw(new_obu2d_melt(hotel, .75+.25*sin(t*.5)), new_layer(cols1), 0,
                         new_m2_roto(0, 0, 14.0, 0), &m2d_ident);
               */
    //and there is no sheets
    } else if (btw(t, 53.75*32, 54.00*32)) {//VALMIS
      obu2d_draw(new_obu2d_melt(sheets, .5+.5*sin(t*.5)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0, -(t-(53.75*32))*0.01), &m2d_ident);
    //on the bed
    } else if (btw(t, 54.00*32, 54.20*32)) {//VALMIS
      obu2d_draw(new_obu2d_melt(bed, .75+.25*sin(t*.5)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0, +(t-(53.75*32))*0.02), &m2d_ident);
    //call the
    } else if (btw(t, 54.50*32, 54.575*32)) {
      obu2d_draw(new_obu2d_melt(phone, .75+.25*sin(t*.5)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0, 0), &m2d_ident);
    // anager
    } else if (btw(t, 54.575*32, 54.825*32)) {
      obu2d_draw(new_obu2d_melt(manager, .75+.25*sin(t*.5)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0, 0), &m2d_ident);

    //and tell hi

    //i want a sheet
    } else if (btw(t, 55.00*32, 55.25*32)) {//VALMIS
      obu2d_draw(new_obu2d_melt(sheets, .5+.5*sin(t*.5)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0+(t-55.0*32)*0.02,  0), &m2d_ident);

    //he tells  e to go to the toilet
    } else if (btw(t, 55.25*32, 55.5*32)) {
      obu2d_draw(new_obu2d_melt(manager, .75+.25*sin(t*.5)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0, 0), &m2d_ident);
    } else if (btw(t, 55.5*32, 55.75*32)) {
      obu2d_draw(new_obu2d_melt(toilet, .75+.25*sin(t*.5)), new_layer(cols1), 0,
        //new_m2_roto(0, 0, 14.0,  -(t-(28.0*32))*0.03), &m2d_ident);
        new_m2_mul(new_m2_roto(0, 0, 14.0+(t-55.5*32)*0.02,  -(t-(55.5*32))*0.01), &m2d_xflip), &m2d_ident);

    //i tell hi
    } else if (btw(t, 55.75*32, 56.2*32)) {
      Layer *valih=new_layer(cols1);
      obu2d_draw(new_obu2d_melt(luigi, .8+.2*sin(t*2)), valih, 0,
        new_m2_roto(750000, 0, 14.0, 0), &m2d_ident);
      if (btw(t, 55.875*32, 56.2*32)) {
        obu2d_draw(new_obu2d_melt(manager, .75+.25*sin(t*2)), valih, 0,
          new_m2_roto(-750000, 0, 14.0, 0), &m2d_ident);
      }
    //a sheet on  y
    } else if (btw(t, 56.2*32, 56.325*32)) {//VALMIS last was .4
      obu2d_draw(new_obu2d_melt(sheets, .5+.5*sin((t+12)*.5)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0+(t-56.2*32)*0.01,  -(t-(56.2*32))*0.005), &m2d_ident);
    //bed
    } else if (btw(t, 56.325*32, 56.6*32)) {//VALMIS, first was .4
      obu2d_draw(new_obu2d_melt(bed, .75+.25*sin(t*1)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0+(t-56.2*32)*0.01,  +(t-(56.2*32))*0.005), &m2d_ident);

    // he says you better not
    } else if (btw(t, 56.70*32, 57.0*32)) {
      obu2d_draw(new_obu2d_melt(manager, .75+.25*sin(t*.5)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0, 0), &m2d_ident);
    //sheet on the bed you
    } else if (btw(t, 57.00*32, 57.125*32)) {//VALMIS
      obu2d_draw(new_obu2d_melt(sheets, .5+.5*sin((t+12)*.5)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0+(t-56.2*32)*0.01,  +(t-(57.0*32))*0.005), &m2d_ident);
    } else if (btw(t, 57.125*32, 57.25*32)) {//VALMIS, vaihda vaan kuva
      obu2d_draw(new_obu2d_melt(bed, .5+.5*sin((t+12)*0.5)), new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0+(t-56.2*32)*0.01,  -(t-(57.125*32))*0.005), &m2d_ident);

    //son of a bitch
    } else if (btw(t, 57.25*32, 57.50*32)) {//VALMIS
      Obu2d *obu=new_obu2d_morph2(son, bitch, 0.5+0.5*fpow2(fsin2((t+4)*0.1), 0.2));
      obu2d_draw(obu, new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0, 0), &m2d_ident);

    //i go to the check out and the  an at the desk says
    } else if (btw(t, 58.00*32, 59.30*32)) {//VALMIS
      Layer *valih=new_layer(cols1);
      if (btw(t, 58.00*32, 58.625*32)) {//VALMIS
        obu2d_draw(new_obu2d_melt(man1, .75+.25*sin(t*.5)), valih, 0,
          new_m2_roto(-500000, 0, 14.0, 0), &m2d_ident);
      //peace on you
      } else if (btw(t, 58.625*32, 59.30*32)) {//VALMIS
        obu2d_draw(new_obu2d_melt(man2, .75+.25*sin(t*.5)), valih, 0,
          new_m2_roto(-500000, 0, 14.0, 0), &m2d_ident);
      }
      //i say to hi  piss on you too you
      if (btw(t, 59.00*32, 59.30*32)) {//VALMIS
        obu2d_draw(new_obu2d_melt(luigi, .8+.2*sin(t*2)), valih, 0,
          new_m2_roto(750000, 0, 14.0, 0), &m2d_ident);
      }
    //son of a bitch
    } else if (btw(t, 59.30*32, 59.55*32)) {//VALMIS
      Obu2d *obu=new_obu2d_morph2(son, bitch, 0.5+0.5*fpow2(fsin2((t-2)*0.1), 0.2));
      obu2d_draw(obu, new_layer(cols1), 0,
        new_m2_roto(0, 0, 14.0, 0), &m2d_ident);
    }

  }


  if (btw(t, 60*32, 62*32)) {
    Layer *valih=new_layer(cols1);
    obu2d_draw(new_obu2d_melt(italy, .8+.2*sin(t*2)), valih, 0,
      new_m2_roto(-750000, 0, 14.0, 0), &m2d_ident);
    obu2d_draw(new_obu2d_melt(luigi, .8+.2*sin(t*0.1)), valih, 0,
      new_m2_roto(750000-(t/32-60)*900000, -(t/32-60)*400000, 14.0-(t/32-60)*2, -(t-(60.0*32))*(0.03+(t/32-60)*0.03)), &m2d_ident);
  }
  if (t>62.1*32) { extern int stopnow; stopnow++; }

//  if (t>12.1*32) { extern int stopnow; stopnow++; }



            /*
                if (t>0*64 && t<2*64) {
            //      float tt=t/8.0, ttt;
            //      if (t<6*64) ttt=0; else ttt=0;//(t-6*64)/4.0;
            //      Obu2d *obu=new_obu2d_morph2(son, bitch, 0.5+0.5*fpow2(fsin2(tt), 0.3));
                  obu2d_draw(new_obu2d_melt( fx, .5+.5*sin(t*.05)), new_layer(cols1), 0,
                    new_m2_roto(0, 0, 14.0, 0), &m2d_ident);
                }
                if (t>2*64 && t<4*64) {
                  Obu2d *obu=new_obu2d_morph2(son, bitch, 0.5+0.5*fpow2(fsin2(tt), 0.1));
                  obu2d_draw(obu, new_layer(cols1), 0,
                    new_m2_roto(0, 0, 14.0, 0), &m2d_ident);
                }
                if (t>4*64 && t<5*64) {
            //      Obu2d *obu=new_obu2d_morph2(bitch, luigi, 0.5+0.5*fpow2(fsin2(tt), 0.3));
                  obu2d_draw(new_obu2d_melt(hotel, .5+.5*sin(t*.05)), new_layer(cols1), 0,
                    new_m2_roto(0, 0, 14.0, 0), &m2d_ident);
                }
                if (t>5*64 && t<6*64) {
            //      Obu2d *obu=new_obu2d_morph2(bitch, luigi, 0.5+0.5*fpow2(fsin2(tt), 0.3));
                  obu2d_draw(new_obu2d_melt(luigi, .5+.5*sin(t*.05)), new_layer(cols1), 0,
                    new_m2_roto(0, 0, 14.0, 0), &m2d_ident);
                }
                if (t>6*64 && t<7*64) {
            //      Obu2d *obu=new_obu2d_morph2(bitch, luigi, 0.5+0.5*fpow2(fsin2(tt), 0.3));
                  obu2d_draw(new_obu2d_melt(waitress, .5+.5*sin(t*.05)), new_layer(cols1), 0,
                    new_m2_roto(0, 0, 14.0, 0), &m2d_ident);
                }
            */




                //obu2d_draw(son, new_layer(cols5), 0x55,
                  //new_m2_roto(0, 0, 14.8+1.7*sin(fmod(t+0.2, 1)*2*3.14159265358979323), t*2));


              /*  obu2d_draw(son, new_layer(4, cols3), 0x0b,
                  new_m2_roto(0, 0, 14.8+1.7*sin(fmod(t+0.4, 1)*2*3.14159265358979323), t*3));
                obu2d_draw(son, new_layer(4, cols4), 0x0b,
                  new_m2_roto(0, 0, 14.8+1.7*sin(fmod(t+0.6, 1)*2*3.14159265358979323), t*4));*/
  ot=t;
}
