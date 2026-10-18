/* 24-bit RGB in hardware 32-bit XRGB pixels. GPL-2.0-or-later. */
#ifndef ME_RGB24_H
#define ME_RGB24_H
long CDECL c32_write_pixel(Virtual *,MFDB *,long,long,long);
long CDECL c32_read_pixel(Virtual *,MFDB *,long,long);
long CDECL c32_line_draw(Virtual *,long,long,long,long,long,long,long);
long CDECL c32_fill_area(Virtual *,long,long,long,long,short *,long,long,long);
long CDECL c32_expand_area(Virtual *,MFDB *,long,long,MFDB *,long,long,long,long,long,long);
long CDECL c32_blit_area(Virtual *,MFDB *,long,long,MFDB *,long,long,long,long,long);
long CDECL c32_mouse_draw(Workstation *,long,long,Mouse *);
long CDECL c32_get_colour(Virtual *,long);
void CDECL c32_get_colours(Virtual *,long,unsigned long *,unsigned long *);
void CDECL c32_set_colours(Virtual *,long,long,unsigned short *,Colour []);
void CDECL c32_initialize_palette(Virtual *,long,long,short[][3],Colour []);
#endif
