/* RGB888 rendering using the public-domain C primitives, with full-width
 * palette and overlap-safe raster operations. New glue GPL-2.0-or-later. */
#define PIXEL long
#define c_write_pixel c32_write_pixel
#define c_read_pixel c32_read_pixel
#define c_line_draw c32_line_draw
#define c_fill_area c32_fill_area
#define c_expand_area c32_expand_area
#define c_mouse_draw c32_mouse_draw
#define c_get_colour c32_get_colour
#define c_get_colours c32_get_colours
#define c_set_colours c32_set_colours
#define c_initialize_palette c32_initialize_palette
#include "../16_bit/16b_exp.c"
#include "../16_bit/16b_line.c"
#include "../16_bit/16b_fill.c"
#include "../16_bit/16b_scr.c"
#include "../16_bit/16b_mouse.c"
#include "rgb24.h"

void CDECL c32_get_colours(Virtual *v,long colour,unsigned long *fg,unsigned long *bg)
{
    Colour *local=v->palette, *global=v->real_address->screen.palette.colours;
    Colour *f,*b;
    if(local && !((long)local&1)) f=b=local;
    else {
        local=(Colour *)((long)local&~1L);
        f=local && (short)colour<0 ? local : global;
        b=local && (colour>>16)<0 ? local : global;
    }
    *fg=*(unsigned long *)&f[(short)colour].real;
    *bg=*(unsigned long *)&b[colour>>16].real;
}
long CDECL c32_get_colour(Virtual *v,long colour)
{ unsigned long fg,bg; c32_get_colours(v,colour,&fg,&bg); return fg; }
static void me32_palette(long start,long entries,unsigned short *requested,Colour palette[],int extended)
{
    long i; unsigned long r,g,b;
    for(i=0;i<entries;i++) {
        if(extended) {
            requested++;
            r=*requested++ >> 8; g=*requested++ >> 8; b=*requested++ >> 8;
            palette[start+i].vdi.red=r*1000/255;
            palette[start+i].vdi.green=g*1000/255;
            palette[start+i].vdi.blue=b*1000/255;
        } else {
            unsigned short rr=*requested++,gg=*requested++,bb=*requested++;
            if(rr>1000) rr=1000; if(gg>1000) gg=1000; if(bb>1000) bb=1000;
            palette[start+i].vdi.red=rr; palette[start+i].vdi.green=gg; palette[start+i].vdi.blue=bb;
            r=(rr*255UL+500)/1000; g=(gg*255UL+500)/1000; b=(bb*255UL+500)/1000;
        }
        palette[start+i].hw.red=(r*1000+128)/255;
        palette[start+i].hw.green=(g*1000+128)/255;
        palette[start+i].hw.blue=(b*1000+128)/255;
        *(unsigned long *)&palette[start+i].real=(r<<16)|(g<<8)|b;
    }
}
void CDECL c32_set_colours(Virtual *v,long start,long entries,unsigned short *requested,Colour palette[])
{ int extended=(long)requested&1; (void)v; me32_palette(start,entries,(unsigned short *)((long)requested&~1L),palette,extended); }
void CDECL c32_initialize_palette(Virtual *v,long start,long entries,short requested[][3],Colour palette[])
{ (void)v; me32_palette(start,entries,(unsigned short *)requested,palette,0); }

static unsigned long me32_rop(unsigned long s,unsigned long d,long op)
{
    switch(op) {
    case 0:return 0; case 1:return s&d; case 2:return s&~d; case 3:return s;
    case 4:return ~s&d; case 5:return d; case 6:return s^d; case 7:return s|d;
    case 8:return ~(s|d); case 9:return ~(s^d); case 10:return ~d;
    case 11:return s|~d; case 12:return ~s; case 13:return ~s|d;
    case 14:return ~(s&d); default:return ~0UL;
    }
}
long CDECL c32_blit_area(Virtual *v,MFDB *s,long sx,long sy,MFDB *d,long dx,long dy,long w,long h,long op)
{
    Workstation *wk;
    unsigned char *sb,*db;
    long ss,ds,row,col,first,last,step;
    if((long)v&1) return 0;
    if(w<=0 || h<=0) return 1;
    wk=v->real_address;
    if(!s || !s->address || s->address==wk->screen.mfdb.address) { sb=(unsigned char *)wk->screen.mfdb.address; ss=wk->screen.wrap; }
    else { if(s->standard || s->bitplanes!=32) return 0; sb=(unsigned char *)s->address; ss=(long)s->wdwidth*64; }
    if(!d || !d->address || d->address==wk->screen.mfdb.address) { db=(unsigned char *)wk->screen.mfdb.address; ds=wk->screen.wrap; }
    else { if(d->standard || d->bitplanes!=32) return 0; db=(unsigned char *)d->address; ds=(long)d->wdwidth*64; }
    sb+=sy*ss+sx*4; db+=dy*ds+dx*4;
    /* GEM uses one raster layout for screen overlap. Reverse both axes when
     * destination follows source; use CPU fallback even for tiny rectangles. */
    first=0; last=h; step=1;
    if((unsigned long)db>(unsigned long)sb && (unsigned long)db<(unsigned long)sb+(h-1)*ss+w*4) { first=h-1;last=-1;step=-1; }
    for(row=first;row!=last;row+=step) {
        unsigned long *sp=(unsigned long *)(sb+row*ss), *dp=(unsigned long *)(db+row*ds);
        if(step<0) for(col=w-1;col>=0;--col) dp[col]=me32_rop(sp[col],dp[col],op);
        else for(col=0;col<w;++col) dp[col]=me32_rop(sp[col],dp[col],op);
    }
    return 1;
}
